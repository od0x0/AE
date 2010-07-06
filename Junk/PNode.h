#pragma once
#include "../FastMath3D.h"
#include "../List.h"

typedef struct AEPNode{
	AEVec3 center;
	float radius;
	AEVec3* triangles;
	unsigned short triangleCount;
	unsigned char childCount,isActive;
	void* data;
	struct AEPNode* children;
}AEPNode;


AEPNode* AEPNodeChildren(AEPNode* node,unsigned char count);
void AEPNodeConstructFromPoints(AEPNode* node,unsigned short count,AEVec3* points);
void AEPNodesDelete(AEPNode* nodes,int count)
int AEPNodeCollidesWith(AEPNode* node,AEPNode* node2,AEVec3* correction);
int AEPNodeTestRay(AEPNode* node,AEVec3* location);