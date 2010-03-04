#include "../Core.h"
#include "../SOIL/SOIL.h"
#include <math.h>
#include <string.h>

// This code could have been cleaner, particularly with an array "class"

static AEVBO* AEVBONew(void){
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	return (AEVBO*)calloc(1,sizeof(AEVBO));
}

unsigned char AEVBOAddAllocateFreq=100;

////  Really ugly code, this is the part where I say, "It works"
static unsigned int AEVBOAddVert(AEVBO* vbo,AEVBOVertWithNormal* v){
	if(v==NULL){//  Resize the array to fit
		vbo->vallocated=0;
		if(vbo->hasNormals) vbo->n=realloc(vbo->n,sizeof(AEVec3)*vbo->vcount);
		vbo->verts=(AEVBOVert*)realloc(vbo->verts,sizeof(AEVBOVert)*vbo->vcount);
		return 0xbaadf00d;//Junk return, you shouldn't do anything with this anyway
	}
	
	//Check for pre-existing, return it if it is
	unsigned int index=0;
	AEVBOVert vert={v->t,v->v};
	/*index=AELinearSearch(&vert,vbo->verts,vbo->vcount);
	if(vbo->hasNormals){
		index=AELinearSearch(&(v->n),vbo->v,vbo->vcount);
	}*/
	for(unsigned int i=0;i<vbo->vcount;i++){
		if(memcmp(vbo->verts+i,&vert,sizeof(AEVBOVert))==0){
			
			if((!vbo->hasNormals)||memcmp(vbo->n+i,&(v->n),sizeof(AEVec3))==0){
				index=i+1;
				break;
			}
		}
	}
	
	
	if(index) return index-1;
	//Add if it doesn't exist
	//Resize array
	index=vbo->vcount++;
	if(vbo->vallocated==0 || vbo->vallocated <= (vbo->vcount+1)){//Allocates more than needed to avoid expensive realloc calls
		vbo->vallocated=vbo->vcount+AEVBOAddAllocateFreq;
		if(vbo->hasNormals) vbo->n=realloc(vbo->n,sizeof(AEVec3)*vbo->vallocated);
		vbo->verts=(AEVBOVert*)realloc(vbo->verts,sizeof(AEVBOVert)*vbo->vallocated);
	}
	//Make assignment
	if(vbo->hasNormals) vbo->n[index]=v->n;
	vbo->verts[index]=vert;
	
	return index;
}

void AEVBOAdd(AEVBO* vbo,AEVBOVertWithNormal* v){//Pretty much the same concept as above, just for indices, and wraps the above
	if(v==NULL){
		vbo->iallocated=0;
		vbo->indices=(unsigned int*)realloc(vbo->indices,sizeof(int)*vbo->icount);
		AEVBOAddVert(vbo,v);
		return;
	}
	
	if(vbo->iallocated==0 || vbo->iallocated <= (vbo->icount+1)){
		vbo->iallocated=vbo->icount+AEVBOAddAllocateFreq;
		vbo->indices=(unsigned int*)realloc(vbo->indices,sizeof(int)*vbo->iallocated);
	}
	
	vbo->indices[vbo->icount++]=AEVBOAddVert(vbo,v);
}

void AEVBODraw(AEVBO* vbo){
	if(!vbo) return;
	if(vbo->vbo) glBindBuffer(GL_ARRAY_BUFFER,vbo->vbo);
	
	if(vbo->hasNormals){
		const int stride=5*sizeof(float);
		const unsigned int offset=(unsigned int)(vbo->vbo?NULL:vbo->verts);
		glVertexPointer(3,GL_FLOAT,stride,(void*)(sizeof(float)*2+offset));
		glTexCoordPointer(2,GL_FLOAT,stride,(void*)offset);
		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER,vbo->nbo);
		glNormalPointer(GL_FLOAT,3*sizeof(float),0+(vbo->n?NULL:vbo->n));
	}
	else glInterleavedArrays(GL_T2F_V3F,0,vbo->vbo?NULL:vbo->verts);
	
	if(vbo->ibo) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbo->ibo);
	if(vbo->indices || vbo->ibo) glDrawElements(GL_TRIANGLES, vbo->icount, GL_UNSIGNED_INT, vbo->ibo?NULL:vbo->indices);
	else glDrawArrays(GL_TRIANGLES,vbo->vcount,GL_UNSIGNED_INT);
	
	if(vbo->hasNormals) glDisableClientState(GL_NORMAL_ARRAY);
}

void AEVBOCompile(AEVBO* vbo,unsigned int* usages){
	if(vbo->iallocated||vbo->vallocated) AEVBOAdd(vbo,NULL);//Implicit
	
	unsigned int defaultUsages[2]={GL_STATIC_DRAW,GL_STATIC_DRAW};
	if(usages==NULL) usages=defaultUsages;
	
	//Vertices
	glGenBuffers(1,(GLuint*)&vbo->vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo->vbo);
	glBufferData(GL_ARRAY_BUFFER,vbo->vcount*sizeof(AEVBOVert),vbo->verts,usages[0]);
	
	//free(vbo->verts);
	//vbo->verts=NULL;
	//vbo->vallocated=0;
	
	if(vbo->hasNormals){
		glGenBuffers(1,(GLuint*)&vbo->nbo);
		glBindBuffer(GL_ARRAY_BUFFER,vbo->nbo);
		glBufferData(GL_ARRAY_BUFFER,vbo->vcount*sizeof(AEVec3),vbo->n,usages[0]);
	}
	
	//Indices
	glGenBuffers(1,(GLuint*)&vbo->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,vbo->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,vbo->icount*sizeof(int),vbo->indices,usages[1]);
	
	//free(vbo->indices);
	//vbo->indices=NULL;
	//vbo->vallocated=0;
}

AEVBO* AEVBOLoad(const char* filename,int isStatic,int hasNormals){
	if(filename==NULL){
		AEVBO* vbo=AEVBONew();
		vbo->hasNormals=hasNormals;
		return vbo;
	}
	AEMesh* m=AEMeshLoad(filename);
	AEVBO* vbo=AEMeshAsVBO(m,isStatic,hasNormals);
	AEMeshDelete(m);
	return vbo;
}

void AEVBODelete(AEVBO* vbo){
	if(vbo==NULL) return;
	glDeleteBuffers(1,(GLuint*)&vbo->vbo);
	glDeleteBuffers(1,(GLuint*)&vbo->nbo);
	glDeleteBuffers(1,(GLuint*)&vbo->ibo);
	free(vbo->verts);
	free(vbo->indices);
	free(vbo->n);
	free(vbo);
}