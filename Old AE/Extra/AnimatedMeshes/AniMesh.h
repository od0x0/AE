#include "AEVA.h"
#include "AEMath.h"
//Mostly converted from code from http://tfc.duke.free.fr/coding/md5-specs-en.html
//It'd be best if the user wrote a layer on top of this, this is just to ease the animation internals
//Format independent, but fits nicely over md5
//Largely incomplete, should really only be thought of as a utility

typedef struct{
	float bias;
	int jointID;
	AEVec3 offsetFromJoint;
}AEAniWeight;

typedef struct{
	char name[16];
	int parentID;
	AEVec3 position;
	AEQuat rotation;
}AEAniJoint;

typedef struct{
	float u,v;
	short unsigned int startingWeight;
	short unsigned int weightCount;
}AEAniVertex;

typedef struct{
	AEAniVertex* vertices;
	unsigned int* indices;
	AEAniWeight* weights;
	int weightCount,vertexCount,indexCount;
}AEAniMesh;

typedef struct{
	AEAniJoint* baseSkeleton;
	AEAniMesh* meshes;
	int meshCount, jointCount;
}AEAniModel;

typedef struct{
	char name[32];
	int frameCount, jointCount;
	AEAniJoint* *skeletons;
	char framerate;
}AEAniAnimation;

typedef struct{
	int current, next;
	float timeLeft;
}AEAniAnimationInfo;

//The t in r=a+t*(b-a)
void AEAniJointsInterpolate(AEAniJoint* outjoints, AEAniJoint* joints1, AEAniJoint* joints2, float t, int jointCount);
void AEAniMeshCalculateVertices(const AEAniMesh* mesh,const AEAniJoint* joints,AEVec3* vertices);

extern int AEAniMeshFlipVertexWinding;//Defaults to true
AEAniModel* AEAniModelLoadMD5(char* filename);
void AEAniModelDelete(AEAniModel* model);
