#include "../VBO.h"
#include "../SOIL/SOIL.h"
#include "../RawMesh.h"
#include <math.h>
#include <string.h>

// This code could have been cleaner, it's like this mainly because I'm simply trying to get it to work

AEVBO* AEVBONew(char hasNormals,char tcount,char usesIndices,char* type){
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	AEVBO* vbo=(AEVBO*)calloc(1,sizeof(AEVBO));
	
	vbo->indices.vbotype=AEVAVBOTypeStatic;
		//vbo->indices.vbotype=0;
	
	if(strstr(type,"static")!=NULL) vbo->va.vbotype=AEVAVBOTypeStatic;
	else if(strstr(type,"dynamic")!=NULL) vbo->va.vbotype=AEVAVBOTypeDynamic;
	else if(strstr(type,"stream")!=NULL) vbo->va.vbotype=AEVAVBOTypeStream;
	else vbo->va.vbotype=0;
	
	vbo->usesIndices=usesIndices;
	vbo->elementSize=1*3+tcount*2+hasNormals*3;
	vbo->hasNormals=hasNormals;
	vbo->texUnitCount=tcount;
	
	printf("VBO 1 verts, %i normals, %i texcoords, %i indices, type %i\n",hasNormals,tcount,usesIndices,(int)vbo->va.vbotype);
	
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
	unsigned int range=vbo->va.length/vbo->elementSize;
	if(vbo->usesIndices) range=vbo->indices.length;
	AEVADraw(0,range);
}

size_t AEVBOVertexTypeSize(AEVBO* vbo){
	return vbo->elementSize*sizeof(float);
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

AEVBO* AEVBOLoad(char* filename,char hasNormals,char tcount,char usesIndices,char* type){
	AEVBO* vbo=AEVBONew(hasNormals, tcount, usesIndices, type);
	
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