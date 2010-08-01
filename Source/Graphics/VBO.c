#include "../VBO.h"
#include "../SOIL/SOIL.h"
#include "../RawMesh.h"
#include <math.h>
#include <string.h>

// This code could have been cleaner, it's like this mainly because I'm simply trying to get it to work
//Format: "ttnvi"
//char hasNormals,char tcount,char usesIndices
AEVBO* AEVBONew(char* format,char* type){
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	AEVBO* vbo=(AEVBO*)calloc(1,sizeof(AEVBO));
	
	vbo->indices.vbotype=AEVAVBOTypeStatic;
		//vbo->indices.vbotype=0;
	
	if(strstr(type,"static")!=NULL) vbo->va.vbotype=AEVAVBOTypeStatic;
	else if(strstr(type,"dynamic")!=NULL) vbo->va.vbotype=AEVAVBOTypeDynamic;
	else if(strstr(type,"stream")!=NULL) vbo->va.vbotype=AEVAVBOTypeStream;
	else vbo->va.vbotype=0;
	
	/*vbo->usesIndices=usesIndices;
	vbo->floatsPerVertex=1*3+tcount*2+hasNormals*3;
	vbo->hasNormals=hasNormals;
	vbo->texUnitCount=tcount;*/
	vbo->texUnitCount=vbo->hasNormals=vbo->usesIndices=0;
	size_t length=strlen(format);
	for(size_t i=0;i<length;i++){
		switch(format[i]){
			case 't':
				vbo->texUnitCount++;
				break;
			case 'n':
				vbo->hasNormals=1;
				break;
			case 'i':
				vbo->usesIndices=1;
				break;
		}
	}
	
	vbo->floatsPerVertex=1*3+vbo->texUnitCount*2+vbo->hasNormals*3;
	
	//printf("VBO 1 verts, %i normals, %i texcoords, %i indices, type %i\n",vbo->hasNormals,vbo->texUnitCount,vbo->usesIndices,(int)vbo->va.vbotype);
	
	return vbo;
}

unsigned char AEVBOAddAllocateFreq=100;


void AEVBOBind(AEVBO* vbo){
	if(vbo==NULL){
		AEVABindIndices(NULL);
	}
	if(vbo->hasNormals) AEVABindInterleavedTNV(&(vbo->va));
	else AEVABindInterleavedTV(&(vbo->va));
	void* indices=NULL;
	if(vbo->usesIndices) indices=&(vbo->indices);
	AEVABindIndices(indices);
}

void AEVBODraw(AEVBO* vbo){
	unsigned int range=vbo->va.length/vbo->floatsPerVertex;
	if(vbo->usesIndices) range=vbo->indices.length;
	AEVADraw(0,range);
}

size_t AEVBOVertexTypeSize(AEVBO* vbo){
	return vbo->floatsPerVertex*sizeof(float);
}

void AEVBOCompileVertexList(AEVBO* vbo,AEList* list){
	if(vbo==NULL) return;
	
	//vbo->indices.length=AEListLength(list)/3;
	
	if(vbo->usesIndices){
		AEList* indicesList=AEListNew(size_t);
		AEListRemoveDuplicates(list,indicesList);
		vbo->indices.isAnIndexArray=1;
		volatile unsigned int* data=AEVAMap(&(vbo->indices),AEListLength(indicesList),GL_WRITE_ONLY);
		size_t* indices=AEListAsArray(indicesList,size_t);
		for(size_t i=0;i<AEListLength(indicesList);i++)
			data[i]=indices[i];
		
		AEVAUnmap(&(vbo->indices));
		AEListDelete(indicesList);
	}//else vbo->indices.length=AEListLength(list)/3;

	float* data=AEVAMap(&(vbo->va),AEListLengthInSizeofType(list,float),GL_READ_WRITE);
	memcpy(data,AEListAsArray(list,void),AEListLengthInSizeofType(list,char));
	/*float* vdata=AEListAsArray(list,float);
	for(size_t i=0;i<AEListLength(list);i++)
			data[i]=vdata[i];*/
	AEVAUnmap(&(vbo->va));
	
	AEListDelete(list);
}

void AEVBODelete(AEVBO* vbo){
	if(vbo==NULL) return;
	AEVAClear(&(vbo->va));
	AEVAClear(&(vbo->indices));
	free(vbo);
}

AEVBO* AEVBOLoad(char* filename,char* format,char* type){
	AEVBO* vbo=AEVBONew(format, type);
	
	AEList* vlist=AEListNewWithTypeSize(AEVBOVertexTypeSize(vbo));
	
	AERawMesh* m=AERawMeshLoad(filename);
	for(unsigned int i=0;i < m->count.f;i++){
		AERawMeshFace* face=m->f+i;
		for(int j=0;j<3;j++){
			float* v=alloca(AEVBOVertexTypeSize(vbo));
			int size=0;
			for(int k=0;k<(vbo->texUnitCount*2);k+=2){
				v[size++]=m->t[face->t[j]].x;
				v[size++]=m->t[face->t[j]].y;
			}
			if(vbo->hasNormals){
				v[size++]=m->n[face->n[j]].x;
				v[size++]=m->n[face->n[j]].y;
				v[size++]=m->n[face->n[j]].z;
			}
			v[size++]=m->v[face->v[j]].x;
			v[size++]=m->v[face->v[j]].y;
			v[size++]=m->v[face->v[j]].z;
			AEListAddBytes(vlist,v);
			
			/*printf("Face(%i) %i Size(%i):  ",i,j,size);
			for(int k=0;k<size;k++){
				printf("%f ",v[k]);
			}
			puts("");*/
		}
	}
	AERawMeshDelete(m);

	AEVBOCompileVertexList(vbo,vlist);
	return vbo;
}

void AEVBOCalculateAABB(AEVBO* vbo,AEVec3* min,AEVec3* max){
	int skip=AEVBOVertexTypeSize(vbo);
	unsigned int vcount=vbo->va.length/vbo->floatsPerVertex;
	int offset=vbo->texUnitCount*2+vbo->hasNormals*3;
	void* data=AEVAMap(&(vbo->va),vbo->va.length/vbo->floatsPerVertex,GL_READ_ONLY);
	for(unsigned int i=0;i<vcount;i++){
		AEVec3* v=data+offset*sizeof(float)+skip*i;
		*min=AEVec3Min(*min,*v);
		*max=AEVec3Max(*max,*v);
	}
	AEVAUnmap(&(vbo->va));
}

AEVec3 AEAABBCalculateSize(AEVec3 min,AEVec3 max){
	AEVec3 size=AEVec3Sub(max,min);//size is the difference of the two
	return AEVec3Sub(size,AEVec3Mul(AEVec3FromSingle(0.5),size));//Center it
}

AEVec3 AEAABBCalculateCenter(AEVec3 min,AEVec3 max){
	return AEVec3Mul(AEVec3Add(max,min),AEVec3FromSingle(0.5));//The average
}

void AEVBOTranslate(AEVBO* vbo,AEVec3 move){
	int skip=AEVBOVertexTypeSize(vbo);
	unsigned int vcount=vbo->va.length/vbo->floatsPerVertex;
	int offset=vbo->texUnitCount*2+vbo->hasNormals*3;
	void* data=AEVAMap(&(vbo->va),vbo->va.length/vbo->floatsPerVertex,GL_READ_ONLY);
	for(unsigned int i=0;i<vcount;i++){
		AEVec3* v=data+offset*sizeof(float)+skip*i;
		*v=AEVec3Add(*v,move);
	}
	AEVAUnmap(&(vbo->va));
}