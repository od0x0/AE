#include "SDL.h"
#include "../AE/Core.h"
#include "../AE/VBO.h"
#include "../AE/AniMesh.h"

AETexture Texture=0;
AEVA VertexArray, TexCoordArray, IndexArray;
AEAniModel* Model=NULL;
AEVBO* VBO;

typedef struct{
	int refcount;
	AEAniModel* model;
	AEVA texCoordArray, indexArray;
	AEList* animations;
}AEAniModelVBOClass;

AEAniModelVBOClass* AEAniModelVBOClassNew(char* filename){
	if(NULL==filename) return NULL;
	AEAniModelVBOClass* self=calloc(1,sizeof(AEAniModelVBOClass));
	self->model=AEAniModelLoadMD5(filename);
	self->refcount++;
	
	self->texCoordArray=AEVANew(false,AEVAVBOTypeDynamic);
	self->indexArray=AEVANew(true,AEVAVBOTypeDynamic);
	
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
	
	AEVAClear(&self->texCoordArray);
	AEVAClear(&self->indexArray);
	AEAniModelDelete(self->model);
	free(self);
}

typedef struct{
	AEAniModelVBOClass* shared;
	AEVA vertexArray;
	AEAniAnimation* currentAnimation;
	AEAniAnimationInfo state;
}AEAniModelVBO;

AEAniModelVBO* AEAniModelVBOLoad(char* filename){
	if(NULL==filename) return NULL;
	AEAniModelVBO* self=calloc(1,sizeof(AEAniModelVBO));
	self->shared=AEAniModelVBOClassNew(filename);
	
	self->vertexArray=AEVANew(false,AEVAVBOTypeDynamic);
	
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
	AEVABindVertex(&self->vertexArray);
	AEVABindTexcoord(&self->shared->texCoordArray);
	AEVABindIndices(&self->shared->indexArray);
	AEVADraw(0,self->shared->indexArray.length);
}

void AEAniModelVBODelete(AEAniModelVBO* self){
	if(self==NULL) return;
	AEAniModelVBOClassDelete(self->shared);
	
	AEVAClear(&self->vertexArray);
	free(self);
}

AEAniModelVBO* AniVBO=NULL;

void perframe(float step){
	AETextureBind(Texture);
	static float delta=0;
	delta+=10*step;
	glRotatef(delta,0,1,0);
	glRotatef(-90,1,0,0);
	AEAniModelVBOStep(AniVBO,step);
	AEAniModelVBODraw(AniVBO);
	//glRotatef(45,1,0,0);
	//glColor4f(1,1,0,1);
	/*AEVABindVertex(&VertexArray);
	AEVABindTexcoord(&TexCoordArray);
	AEVABindIndices(&IndexArray);
	AEVADraw(0,IndexArray.length);*/
	
	/*AEVec3* vertices = calloc(1, sizeof(AEAniVertex)*Model->meshes[0].vertexCount);
	AEAniMeshCalculateVertices(Model->meshes+0, Model->baseSkeleton, vertices);
	glBegin(GL_TRIANGLES);
		for(size_t meshID=0; meshID < Model->meshCount; meshID++){
			AEAniMesh* mesh = Model->meshes+meshID;
			for(size_t indexID = 0; indexID < mesh->indexCount; indexID++){
				size_t i = mesh->indices[indexID];
				glTexCoord2f(mesh->vertices[i].u,mesh->vertices[i].v);
				//printf("TexCoord=(%.2f, %.2f)\n",mesh->vertices[i].u,mesh->vertices[i].v);
				glVertex3fv(vertices+i);
				//printf("Position=(%.2f, %.2f, %.2f)\n",vertices[i].x,vertices[i].y,vertices[i].z);
			}
		}
	glEnd();
	free(vertices);*/
	
	/*AEVBOBind(VBO);
	AEVBODraw(VBO);
	AEVBOBind(NULL);
	
	glBegin(GL_LINES);
		glVertex3f(0,0,0);
		glVertex3f(0,10,0);
	glEnd();*/
	/*AETextureBind(0);
	glClear(GL_DEPTH_BUFFER_BIT);
	glLineWidth(4);
	glBegin(GL_LINES);
		for(size_t i=0;i<Model->jointCount;i++){
			if(Model->baseSkeleton[i].parentID == -1) continue;
			AEVec3 parentposition=Model->baseSkeleton[Model->baseSkeleton[i].parentID].position;
			glVertex3f(parentposition.x,parentposition.y,parentposition.z);
			AEVec3 position=Model->baseSkeleton[i].position;
			glVertex3f(position.x,position.y,position.z);
		}
	glEnd();*/
	
	//fflush(stdout);
	//abort();
}

int main(int argc,char** argv){
	AECameraBoundsSet(AECameraActiveGet(),0,0,60);
	AEInit(argv[0],800,500);
	
	glClearColor(1,1,1,1);
	//Code here
	AECameraPositionSet(AECameraActiveGet(),0,0,30);
	Texture=AETextureLoad("Data/skin.jpg");
	
	AniVBO=AEAniModelVBOLoad("Data/ogro.md5mesh");
	
	//VBO=AEVBOLoad("Data/TexturedCube.obj","tvi","stream");
	
	/*VertexArray=AEVANew(false,AEVAVBOTypeDynamic);
	TexCoordArray=AEVANew(false,AEVAVBOTypeDynamic);
	IndexArray=AEVANew(true,AEVAVBOTypeDynamic);*/
	
	//Model=AEAniModelLoadMD5("Data/ogro.md5mesh");
	
	/*AEVec3* vs=AEVAMap(&VertexArray,Model->meshes[0].vertexCount*3,GL_READ_WRITE);
	
	AEAniMeshCalculateVertices(Model->meshes+0,Model->baseSkeleton,vs);
	
	AEVAUnmap(&VertexArray);*/
	/*
	AEVec2* ts=AEVAMap(&TexCoordArray,Model->meshes[0].vertexCount*2,GL_READ_WRITE);
	
	for(size_t i=0;i<Model->meshes[0].vertexCount;i++){
		ts[i].x=Model->meshes[0].vertices[i].u;
		ts[i].y=Model->meshes[0].vertices[i].v;
	}
	
	AEVAUnmap(&TexCoordArray);
	
	unsigned int* is=AEVAMap(&IndexArray,Model->meshes[0].indexCount,GL_READ_WRITE);
	
	for(size_t i=0;i<Model->meshes[0].indexCount;i++) is[i]=Model->meshes[0].indices[i];

	AEVAUnmap(&IndexArray);
	
	//AEAniModelDelete(Model);*/
	
	AEStart(perframe);
	return 1;
}