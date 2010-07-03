#include "../VBO.h"
#include "../SOIL/SOIL.h"
#include "../RawMesh.h"
#include <math.h>
#include <string.h>

// This code could have been cleaner, particularly with an array "class"

inline unsigned int AEVBOVertexCount(AEVBO* vbo){return AEListLength(vbo->vlist)/AEVBONumberOfFloatsVerVertex(vbo);}

/*AEVBO* AEVBONew(char* format){
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	AEVBO* vbo=(AEVBO*)calloc(1,sizeof(AEVBO));
	int vcount=1;
	int ncount=0;
	int tcount=1;
	int usesIndices=1;
	char buffer[1024]={0};
	//sscanf(format,"v%i n%i t%i i%i type:%s",&vcount,&ncount,&tcount,&usesIndices,buffer);
	
	char* str=strstr(format,"v:");
	printf("%p\n",str);
	if(str) sscanf(str,"%i",&vcount);
	
	str=strstr(format,"t:");
	if(str) sscanf(str,"%i",&tcount);
	
	str=strstr(format,"n:");
	printf("%i\n",(int)str-(int)format);
	if(str) sscanf(str,"%i",&ncount);
	
	str=strstr(format,"i:");
	if(str){
		vbo->indices.vbotype=AEVAVBOTypeStatic;
		sscanf(str,"%i",&usesIndices);
	}else{
		vbo->indices.vbotype=0;
	}
	
	if(strstr(format,"static")==NULL) vbo->va.vbotype=AEVAVBOTypeStatic;
	else if(strstr(format,"dynamic")==NULL) vbo->va.vbotype=AEVAVBOTypeDynamic;
	else if(strstr(format,"stream")==NULL) vbo->va.vbotype=AEVAVBOTypeStream;
	else vbo->va.vbotype=0;
	*//*str=strstr(format,"type:");
	if(str){
		sscanf(str,"%s",&buffer);
	
		if(strstr(buffer,"none")==NULL) vbo->va.vbotype=0;
		else if(strstr(buffer,"static")==NULL) vbo->va.vbotype=AEVAVBOTypeStatic;
		else if(strstr(buffer,"dynamic")==NULL) vbo->va.vbotype=AEVAVBOTypeDynamic;
		else if(strstr(buffer,"stream")==NULL) vbo->va.vbotype=AEVAVBOTypeStream;
	
	}*//*
	
	vbo->usesIndices=usesIndices;
	vbo->elementSize=(vcount!=0)*3+tcount*2+ncount*3;
	vbo->hasNormals=ncount!=0;
	vbo->texUnitCount=tcount;
	
	printf("VBO %i verts, %i normals, %i texcoords, %i indices, type %i\n",vcount,ncount,tcount,usesIndices,(int)vbo->va.vbotype);
				
	vbo->vlist=AEListNew(char);
	vbo->vlist->typeSize=vbo->elementSize*sizeof(float);
	if(vbo->usesIndices) vbo->ilist=AEListNew(unsigned int);
	*//*AEVBOMap(&(vbo->va),vbo->elementSize*length,GL_WRITE_ONLY);
	if(vbo->usesIndices) AEVBOMap(&(vbo->indices),length,GL_WRITE_ONLY);*//*
	
	return vbo;
}
*/

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
				
	vbo->vlist=AEListNew(char);
	vbo->vlist->typeSize=vbo->elementSize*sizeof(float);
	if(vbo->usesIndices) vbo->ilist=AEListNew(unsigned int);
	
	return vbo;
}

unsigned char AEVBOAddAllocateFreq=100;

////  Really ugly code, this is the part where I say, "It works"
static unsigned int AEVBOAddVert(AEVBO* vbo,float* v){
	unsigned int length=AEVBOVertexCount(vbo);
	
	if(v==NULL){//  Resize the array to fit
		AEListResize(vbo->vlist,length);
		return 0xbaadf00d;//Junk return, you shouldn't do anything with this anyway
	}
	
	//Check for pre-existing, return it if it is
	unsigned int index=0;
	void* data=AEListAsArray(vbo->vlist,float);
	
	for(unsigned int i=0;i<length;i++){
		if(memcmp(data+vbo->vlist->typeSize*i,v,vbo->vlist->typeSize)==0){
			index=i+1;
			break;
		}
	}
	
	if(index) return index-1;*/
	//Add if it doesn't exist
	for(int i=0;i<vbo->elementSize;i++){
		AEListAdd(vbo->vlist,float,v[i]);
		printf("%f\t",v[i]);
	}
	//index=AEVBOVertexCount(vbo)-1;
	puts("");
	/*void** pointer=AEListCheck(vbo->vlist);
	memcpy(*pointer,v,vbo->vlist->typeSize);*/
	
	return index;
}

void AEVBOAdd(AEVBO* vbo,void* v){//Pretty much the same concept as above, just for indices, and wraps the above
	if(vbo==NULL) return;
	if(v==NULL){
		if(vbo->usesIndices) AEListResize(vbo->ilist,AEListLength(vbo->ilist));
		AEVBOAddVert(vbo,v);
		return;
	}
	unsigned int index=AEVBOAddVert(vbo,v);
	if(vbo->usesIndices) AEListAdd(vbo->ilist,unsigned int,index);
}

/*
typedef struct{
	AEVA va;
	AEVA indices;
	AEList* vlist;
	AEList* ilist;
	char hasNormals;
	char texUnitCount;
	char elementSize;
	char usesIndices;
}AEVBO;
*/

void AEVBOBind(AEVBO* vbo){
	if(vbo->hasNormals) AEVABindInterleavedTNV(&(vbo->va));
	else AEVABindInterleavedTV(&(vbo->va));
	void* indices=NULL;
	if(vbo->usesIndices) indices=&(vbo->indices);
	AEVABindIndices(indices);
}

void AEVBODraw(AEVBO* vbo){
	AEVADraw(0,0);
}

void AEVBOCompile(AEVBO* vbo){
	if(vbo==NULL) return;
	AEVBOAdd(vbo,NULL);//Implicit
	
	void* data=NULL;
	
	data=AEVAMap(&(vbo->va),jahsdkjfhasjkdgfhal,GL_WRITE_ONLY);
	memcpy(data,vbo->vlist->data,sizeof(float)*vbo->vlist->length);
	AEVAUnmap(&(vbo->va));
	
	data=AEVAMap(&(vbo->indices),AEListLength(vbo->ilist),GL_WRITE_ONLY);
	memcpy(data,vbo->ilist->data,vbo->ilist->typeSize*AEListLength(vbo->ilist));
	AEVAUnmap(&(vbo->indices));
}

void AEVBODelete(AEVBO* vbo){
	if(vbo==NULL) return;
	AEVAClear(&(vbo->va));
	AEVAClear(&(vbo->indices));
	AEListDelete(vbo->vlist);
	if(vbo->usesIndices) AEListDelete(vbo->ilist);
	free(vbo);
}

void AEVBOLoadInto(AEVBO* vbo,char* filename){
	AERawMesh* m=AERawMeshLoad(filename);
	printf("RawMeshes's vcount is %i\n",m->count.f*3);
	for(unsigned int i=0;i < m->count.f;i++){
		AERawMeshFace* face=m->f+i;
		for(int j=0;j<3;j++){
			float* v=alloca(vbo->vlist->typeSize);
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
			AEVBOAdd(vbo,v);
		}
	}
	AERawMeshDelete(m);
}