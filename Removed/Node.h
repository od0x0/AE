#pragma once

#include "Core.h"
#include "Math3D.h"
#include "VBO.h"
#include "List.h"
#include "Image.h"
#include "Table.h"

struct AENode;
typedef struct AENode AENode;

typedef void* (*AENodeMessageFunc)(AENode* node,...);
/*
struct AENodeType;
typedef struct AENodeType AENodeType;

enum{
	AENodeTypeUnknown,
	AENodeTypeEnt,
	AENodeTypeMesh,
	AENodeTypeMaterial,
	AENodeTypeLastPredefined=AENodeTypeMaterial
	//,AENodeTypeWorld
};
*/

void AENodesInit(void);
void AENodesQuit(void);

//Ideally, xxxStep should be called with constant stepsize
//Also does additional work that is hidden that cannot be done on the node level
void AENodesStep(float stepInSeconds);
void AENodesRender(void);

void AENodesRootSet(AENode* node);
AENode* AENodesRootGet(void);

//AENode* AENodeNew(unsigned int type,char* name);
AENode* AENodeClone(AENode* node);
AENode* AENodeRetain(AENode* node);
void AENodeDelete(AENode* node);

//Allocates extra room for storage and returns pointer, this storage is deleted when the node is.  Returns NULL if it already exists
void* AENodeSlotNew(AENode* node,char* name,size_t size);
//The pointer returned may become invalid by the next (any to the same node) slot creation, it is best practice to add all first, then use AENodeSlotFind and then modify the data pointer passed back.
//Size is the size of the object when it was created
void* AENodeSlotFind(AENode* node,char* name/*,size_t* size*/);

AENodeMessageFunc AENodeMessageGet(AENode* node,char* name);
void AENodeMessageSet(AENode* node,char* name,AENodeMessageFunc fn);

void AENodeRender(AENode* node);
void AENodeStep(AENode* node,float stepInSeconds);

char* AENodeNameGet(AENode* node);
void AENodeNameSet(AENode* node,char* name);

//Not needed with AENodeSlot*()
/*void* AENodeAuxGet(AENode* node);
void AENodeAuxSet(AENode* node,void* data);*/

//Here for faster updating
//Untold horrors will happen for those who pass in that which is not of the type that is requested, no type checking is done, as the aim is speed.

void AENodeEntPositionGet(AENode* node,AEVec3* v3);
void AENodeEntPositionSet(AENode* node,float x,float y,float z);
void AENodeEntVelocityGet(AENode* node,AEVec3* v3);
void AENodeEntVelocitySet(AENode* node,float x,float y,float z);

void AENodeEntRotationGet(AENode* node,AEVec3* v3);
void AENodeEntRotationSet(AENode* node,float x,float y,float z);
void AENodeEntSpinGet(AENode* node,AEVec3* v3);
void AENodeEntSpinSet(AENode* node,float x,float y,float z);

void AENodeEntChildAdd(AENode* node,AENode* node2);
//Returns NULL if it does not have a child in that spot
AENode* AENodeEntChildGet(AENode* node,size_t i);
AENode* AENodeEntChildFind(AENode* node,char* name);
void AENodeEntChildRemove(AENode* node,AENode* node2);

//Render children are rendered within the ent's transform
//Functionally the same as the above
void AENodeEntRenderChildAdd(AENode* node,AENode* node2);
AENode* AENodeEntRenderChildGet(AENode* node,size_t i);
AENode* AENodeEntRenderChildFind(AENode* node,char* name);
void AENodeEntRenderChildRemove(AENode* node,AENode* node2);

//These two destroy their children
//Creates a Meshnode, adds all meshes/ents that are within distance and under threshold polygon count
void* AENodeEntOptimizationMergeChildren(AENode* node,float distance,int minthreshold,int maxthreshold);
//Converts all meshes/ents that are within distance and over threshold polygon count to instances
void* AENodeEntOptimizationInstanceChildren(AENode* node,float distance,int minthreshold,int maxthreshold);
//Non-destructive, but may actually slow thinsg down, 0 will disable
void* AENodeEntOptimizationVFC(AENode* node,float radius);

void AENodeMaterialShaderSet(AENode* node,char* vshader,char* fshader);
void AENodeMaterialTextureSet(AENode* node,char* name,AEImage* image);
extern bool AEShadersEnabled;

///Deprecated
void AENodeMeshVBOSet(AENode* node,AEVBO* vbo);
AEVBO* AENodeMeshVBOGet(AENode* node);

/*
Design decisions:

Q: What's the whole idea behind this?
A: The goal is to represent the game world as a tree of nodes which can be optimized/generated on-the-fly.

Q: Why aren't you exposing the internals like the rest of the engine does?
A: Because this is designed to hide its implementation as this is what many people will use to use the engine, exposing internals would make it very sensitive to breaking people's code.  While I can request that people not access internals, the rule follows that if they can, they will.  If they really want the internals, they can just modify the source, the overall design of the engine is to allow that to be done easily.

Q: What's with the lack of physics in here?
A: At one point, it was.  Physics is a really sensitive thing, some want ODE, some want Bullet, some want XXXX.  Some situations may require it to be run on the server, the client, both with various factors of influence, etc.  The best decision I could make is to just let the user handle that. 

Q: 
*/