
#pragma once
#include "Physics.h"
#include "../Node.h"

struct AENode{
	char* name;
	void* aux;
	ENode* enode;//For binding to Enough
	unsigned char refcount;//Eases the problem of memory managment
	unsigned char type;
};

typedef struct{
	AENode super;
	union{
		AEVBO* vbo;
		////////////      <><><><><><><><><><><><> To do when animated vbos come
	}data;
	unsigned int start,end;//Range
	unsigned char type;//Animated, Static, etc, unused for now
}AENodeMesh;

typedef struct{
	AENode super;
	unsigned int* textures;
	unsigned int* textureUniforms;
	unsigned char texUnitCount;
	unsigned int program,vshader,fshader;
	//unsigned char color[4];  //unneeded?
}AENodeMaterial;

typedef struct{
	AENode super;
	dWorldID odeworld;
	dSpaceID staticSpace, dynamicSpace;
	//int (*staticAndDynamicSpacesCollide)(dGeomID,dGeomID,AEdSurface*,void* userdata);
	AEList/*<AENode*>*/* children;
}AENodeWorld;

typedef struct{
	AENode super;
	AENodeWorld* world;
	struct{
		AEVec3 position;
		AEQuat rotation;
	}transform;
	AENode* material;
	AENode* mesh;
	AEList/*<AENode*>*/* children;
	float radius;
	dBodyID body;
	struct{
		float friction;
		float bounciness;
		float bounceVelocity;
		char immovable;
	}bodyProperties;
	//AEPBody* body;
}AENodeEnt;

void AENodeMeshDelete(AENode* node);
void AENodeMeshRender(AENode* node);
void AENodeMeshStep(AENode* node,float stepInSeconds);

void AENodeMaterialDelete(AENode* node);
void AENodeMaterialRender(AENode* node);
void AENodeMaterialStep(AENode* node,float stepInSeconds);

void AENodeEntDelete(AENode* node);
void AENodeEntRender(AENode* node);
void AENodeEntStep(AENode* node,float stepInSeconds);


void AENodeWorldInit(AENode* node);
void AENodeWorldDelete(AENode* node);
void AENodeWorldRender(AENode* node);
void AENodeWorldStep(AENode* node,float stepInSeconds);