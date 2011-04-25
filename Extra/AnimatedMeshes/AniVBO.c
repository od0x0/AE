#include "AniVBO.h"

AEAniModelVBOClass* AEAniModelVBOClassNew(char* filename){
	if(NULL==filename) return NULL;
	AEAniModelVBOClass* self=calloc(1,sizeof(AEAniModelVBOClass));
	self->model=AEAniModelLoadMD5(filename);
	self->refcount++;

	AEVAInit(& self->texCoordArray);
	self->texCoordArray.format.textureCoordsPerVertex=1;
	AEVAInit(& self->indexArray);
	self->indexArray.format.indexType=true;
	
	AEVec2* ts=AEVAMap(&self->texCoordArray,self->model->meshes[0].vertexCount*2,GL_READ_WRITE);
	for(size_t i=0;i < self->model->meshes[0].vertexCount;i++){
		ts[i].x=self->model->meshes[0].vertices[i].u;
		ts[i].y=self->model->meshes[0].vertices[i].v;
	}
	AEVAUnmap(&self->texCoordArray);
	
	unsigned int* is=AEVAMap(&self->indexArray,self->model->meshes[0].indexCount,GL_READ_WRITE);
	for(size_t i=0;i < self->model->meshes[0].indexCount;i++) is[i]=self->model->meshes[0].indices[i];
	AEVAUnmap(&self->indexArray);
	
	return self;
}

void AEAniModelVBOClassDelete(AEAniModelVBOClass* self){
	if(self==NULL) return;
	self->refcount--;
	if(self->refcount > 0) return;
	
	AEVADeinit(&self->texCoordArray);
	AEVADeinit(&self->indexArray);
	AEAniModelDelete(self->model);
	free(self);
}

AEAniModelVBO* AEAniModelVBOLoad(char* filename){
	if(NULL==filename) return NULL;
	AEAniModelVBO* self=calloc(1,sizeof(AEAniModelVBO));
	self->shared=AEAniModelVBOClassNew(filename);
	
	AEVAInit(& self->vertexArray);
	self->vertexArray.format.hasVertices=true;
	
	return self;
}

AEAniModelVBO* AEAniModelVBOClone(AEAniModelVBO* self){
	if(self==NULL) return NULL;
	AEAniModelVBO* clone=calloc(1,sizeof(AEAniModelVBO));
	clone->shared=self->shared;
	self->shared->refcount++;
	return clone;
}

void AEAniModelVBOPlayAnimation(AEAniModelVBO* self,AEAniAnimation* animation){
	self->currentAnimation=animation;
}

void AEAniModelVBOPrepareVerts(AEAniModelVBO* self){
	AEVec3* vs=AEVAMap(&self->vertexArray,self->shared->model->meshes[0].vertexCount*3,GL_WRITE_ONLY);
	AEAniMeshCalculateVertices(self->shared->model->meshes+0,self->shared->model->baseSkeleton,vs);
	AEVAUnmap(&self->vertexArray);
}

void AEAniModelVBOStep(AEAniModelVBO* self,float step){
	if(self->currentAnimation==NULL){
		AEAniModelVBOPrepareVerts(self);
		return;
	}
	//Incomplete
	/*
	int maxFrames = self->currentAnimation->frameCount;

	self->state.timeLeft -= step;

	if (self->state.timeLeft < 0){
		self->state.current++;
		self->state.next++;
		self->state.timeLeft = 0.0;

		if (self->state.current > maxFrames) self->state.current = 0;
		if (self->state.next > maxFrames) self->state.next = 0;
    }
	*/
	AEAniModelVBOPrepareVerts(self);
}

void AEAniModelVBODraw(AEAniModelVBO* self){
	/*AEVABindInterleaved(&self->vertexArray);
	AEVABindInterleaved(&self->shared->texCoordArray);
	AEVABindIndices(&self->shared->indexArray);
	AEVADraw(& self->shared->indexArray);*/
}

void AEAniModelVBODelete(AEAniModelVBO* self){
	if(self==NULL) return;
	AEAniModelVBOClassDelete(self->shared);
	
	AEVADeinit(&self->vertexArray);
	free(self);
}
