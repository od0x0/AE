#include "Core.h"
#include "FastMath3D.h"
#include "VBO.h"
#include "List.h"
#include "Image.h"

#define int32 evint32
#define uint32 evuint32
#define int16 evint16
#define uint16 evuint16
#define int8 evint8
#define uint8 evuint8
#include "enough.h"
#undef int32
#undef uint32
#undef int16
#undef uint16
#undef int8
#undef uint8

#include "Physics.h"

#pragma once

struct AENode;
typedef struct AENode AENode;

enum{
	AENodeTypeEnt,
	AENodeTypeMesh,
	AENodeTypeMaterial
};

//This is private, do not access directly
struct AENode{
	char* name;
	unsigned char refcount;//Eases the problem of memory managment
	unsigned char type;
	ENode* enode;//For binding to Enough
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
	struct{
		AEVec3 position;
		AEQuat rotation;
	}transform;
	AENode* material;
	AENode* mesh;
	AEList/*<AENode*>*/* children;
	float radius;
	AEPBody* body;
}AENodeEnt;

unsigned char AENodeTypeGet(AENode* node);
AENode* AENodeRetain(AENode* node);
AENode* AENodeNew(unsigned char type,char* name);
void AENodeDelete(AENode* node);
void AENodeRender(AENode* node);
void AENodeStep(AENode* node,float stepInSeconds);

//Ent Specific, may also apply to others

void AENodeEntPositionGet(AENode* node,AEVec3* v3);
void AENodeEntPositionSet(AENode* node,float x,float y,float z);
void AENodeEntVelocityGet(AENode* node,AEVec3* v3);
void AENodeEntVelocitySet(AENode* node,float x,float y,float z);

	//Radial (Rotational) versions, obviously these have more correct names, just that this is more consistent

void AENodeEntRotationGet(AENode* node,AEQuat* q4);
void AENodeEntRotationSet(AENode* node,float x,float y,float z,float w);
void AENodeEntSpinGet(AENode* node,AEVec3* v3);
void AENodeEntSpinSet(AENode* node,float x,float y,float z);

//Bounding sphere for occlusion culling
float AENodeEntRadiusGet(AENode* node);
void AENodeEntRadiusSet(AENode* node,float radius);

void AENodeEntBodySet(AENode* node,AEPBody* body);
AEPBody* AENodeEntBodyGet(AENode* node);

void AENodeEntAddChild(AENode* node,AENode* node2);
//Mesh Specific
void AENodeMeshVBOSet(AENode* node,AEVBO* vbo);
AEVBO* AENodeMeshVBOGet(AENode* node);

//Material Specific
void AENodeMaterialShaderSet(AENode* node,char* vshader,char* fshader);
void AENodeMaterialTextureSet(AENode* node,char* name,AEImage* image);

//Private
AENode* AENodeMeshNew(char* name);
void AENodeMeshDelete(AENode* node);
void AENodeMeshRender(AENode* node);
void AENodeMeshStep(AENode* node,float stepInSeconds);

AENode* AENodeMaterialNew(char* name);
void AENodeMaterialDelete(AENode* node);
void AENodeMaterialRender(AENode* node);
void AENodeMaterialStep(AENode* node,float stepInSeconds);

AENode* AENodeEntNew(char* name);
void AENodeEntDelete(AENode* node);
void AENodeEntRender(AENode* node);
void AENodeEntStep(AENode* node,float stepInSeconds);