	#pragma once
#include "AE.h"

typedef struct{
	AEVec3 position;
	AEVec3 normal;
	AEVec2 texcoord;
}AEMeshVertex;

static inline AEMeshVertex AEMeshVertexLerp(AEMeshVertex a, AEMeshVertex b, float t){
	AEMeshVertex r;
	r.position=AEVec3Lerp(a.position, b.position, t);
	r.normal=AEVec3Lerp(a.normal, b.normal, t);
	r.texcoord=AEVec3AsVec2(AEVec3Lerp(AEVec2AsVec3(a.texcoord), AEVec2AsVec3(b.texcoord), t));
	return r;
}

typedef struct{
	AEMeshVertex vertices[3];
}AEMeshTriangle;

void AEMeshTriangleSubdivide4(AEMeshTriangle* self, AEMeshTriangle* t0, AEMeshTriangle* t1, AEMeshTriangle* t2, AEMeshTriangle* t3);
//And what would you know!  This also happens to be the number of triangles it yields!
#define AEMeshTriangleSplitCaseEdgeEdge 3
#define AEMeshTriangleSplitCaseEdgeVertex 2
#define AEMeshTriangleSplitCaseCoplanar 1
#define AEMeshTriangleSplitCaseNone 0
int AEMeshTriangleSplit(AEMeshTriangle* self, AEVec4 plane, AEMeshTriangle* t0, AEMeshTriangle* t1, AEMeshTriangle* t2);

typedef struct{
	AEArray(AEMeshTriangle) triangles;
}AEMesh;

void AEMeshInit(AEMesh* self);
void AEMeshDeinit(AEMesh* self);