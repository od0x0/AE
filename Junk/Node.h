#include "Core.h"

enum{
	AENodeTypeNone=0,
	//AENodeTypeTransform,
	//Defines an orientation in space
	AENodeTypeEnt,
	//Defines an entity that links the other node types
	//AENodeTypeBody,
	//Defines collision properties
	AENodeTypeMesh,
	//Defines a mesh
	AENodeTypeMaterial,
	//Defines a material that will be bound before the mesh is rendered
	//AENodeTypeCluster,
	//Defines a group of Entities
	AENodeTypeCamera,
	//Defines a camera
	AENodeTypeLight,
	//Defines a light
	AENodeTypeLastPredefined=AENodeTypeEntLight
};

struct AENode;
typedef struct AENode AENode;

unsigned int AENodeNew(unsigned char type,...);
unsigned int AENodeRetain(unsigned int nodeID);
void AENodeDelete(unsigned int nodeID);

void AENodeConnect(unsigned int nodeID,unsigned int node2ID);
void AENodeDisconnect(unsigned int nodeID,unsigned int node2ID);

unsigned char AENodeTypeGet(unsigned int nodeID);

typedef struct{
	unsigned int seconds,fraction;
}AENodeTime;
//void AENodeTimeCurrentGet(AENodeTime* timeValue);
//void AENodeTimeCurrentSet(AENodeTime* timeValue);

void AENodeTimeUpdate(float stepInSeconds);

//Ent Specific, may also apply to others
void AENodeEntPositionGet(unsigned int nodeID,AEVec3* v3);
void AENodeEntPositionSet(unsigned int nodeID,float x,float y,float z);
void AENodeEntVelocityGet(unsigned int nodeID,AEVec3* v3);
void AENodeEntVelocitySet(unsigned int nodeID,float x,float y,float z);
void AENodeEntAccelerationGet(unsigned int nodeID,AEVec3* v3);
void AENodeEntAccelerationSet(unsigned int nodeID,float x,float y,float z);
void AENodeEntPullGet(unsigned int nodeID,AEVec3* v3);
void AENodeEntPullSet(unsigned int nodeID,float x,float y,float z);

	//Radial (Rotational) versions, obviously these have more correct names, just that this is more consistent

void AENodeEntRotationGet(unsigned int nodeID,AEQuat* q4);
void AENodeEntRotationSet(unsigned int nodeID,float x,float y,float z,float w);
void AENodeEntRadialVelocityGet(unsigned int nodeID,AEQuat* q4);
void AENodeEntRadialVelocitySet(unsigned int nodeID,float x,float y,float z,float w);
void AENodeEntRadialAccelerationGet(unsigned int nodeID,AEQuat* q4);
void AENodeEntRadialAccelerationSet(unsigned int nodeID,float x,float y,float z,float w);
void AENodeEntRadialPullGet(unsigned int nodeID,AEQuat* q4);
void AENodeEntRadialPullSet(unsigned int nodeID,float x,float y,float z,float w);

//Bounding sphere for occlusion culling
float AENodeRadiusGet(unsigned int nodeID);
void AENodeRadiusSet(unsigned int nodeID,float radius);

void AENodeRenderAction(unsigned int nodeID);

//Mesh Specific
void AENodeMeshVBOSet(unsigned int nodeID,AEVBO* vbo);
AEVBO* AENodeMeshVBOGet(unsigned int nodeID);

//Material Specific
void AENodeMaterialShaderSet(unsigned int nodeID,char* vshader,char* fshader);
void AENodeMaterialTextureSet(unsigned int nodeID,char slot,char* name,int w,int h,void* pixels);

//Cluster Specific

//Light Specific
void AENodeLightColorSet(unsigned int nodeID,float r,float g,float b,float a);
	//a=intensity
void AENodeBoundsSet(unsigned int nodeID,float fov,float near,float far);

//Body Specific
void AENodeBodyBoundsSet(unsigned int nodeID,float mass,float w,float h,float d);
void AENodeBodyCenterOfGravitySet(unsigned int nodeID,float x,float y,float z);
void AENodeBodyOffsetSet(unsigned int nodeID,float x,float y,float z);
void AENodeBodyCollisionCallbackSet(unsigned int nodeID,int (*callback)(unsigned int,unsigned int));