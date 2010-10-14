#pragma once
#include "VBO.h"
#include "Material.h"
//#include "A2RVM.h"

struct A2ROctree;
typedef struct A2ROctree A2ROctree;

A2ROctree* A2ROctreeNew(void);
A2ROctree** A2ROctreeChildrenGet(A2ROctree* self);
bool A2ROctreeHasChildren(A2ROctree* self);
bool A2ROctreeContainsPoint(A2ROctree* self, float x, float y, float z);
//void A2ROctreeOpsSet(A2ROctree* self,A2RVMOplist* ops);
void A2ROctreeRender(A2ROctree* self);
void A2ROctreeDelete(A2ROctree* self);