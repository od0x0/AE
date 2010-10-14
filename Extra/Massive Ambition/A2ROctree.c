#include "A2ROctree.h"

struct A2ROctree{
	AEVec3 min, max;
	float radius;
	//A2RVM* rvm;
	A2ROctree* children[8];
	AEVA va;
	AEVA ia;
	AETexture texture;
	AETexture normalmap;
};

A2ROctree* A2ROctreeNew(void){
	A2ROctree* self=calloc(1, sizeof(A2ROctree));
	return self;
}

A2ROctree** A2ROctreeChildrenGet(A2ROctree* self){
	return self->children;
}

bool A2ROctreeHasChildren(A2ROctree* self){
	return self->children[0] 
	or self->children[1] 
	or self->children[2] 
	or self->children[3] 
	or self->children[4] 
	or self->children[5] 
	or self->children[6] 
	or self->children[7];
}

bool A2ROctreeContainsPoint(A2ROctree* self, float x, float y, float z){
	AEVec3 v={x,y,z};
	if(not AEVec3IsBetween(v, self->min, self->max)) return false;
	//A quick/messy check for checking if this octree has no children
	if(not A2ROctreeHasChildren(self)) return true;
	for (int i=0; i<8; i++) {
		A2ROctree* child=self->children[i];
		if(child and A2ROctreeContainsPoint(child,x,y,z)) return true;
	}
	return false;
}

/*void A2ROctreeOpsSet(A2ROctree* self,A2RVMOplist* ops){
	self->rvm=A2RVMOplistCompileToRVM(ops);
	A2RVMOplistDelete(ops);
}*/

void A2ROctreeRender(A2ROctree* self){
	if(self->radius){
		const AEVec3 center=AEVec3Mul(AEVec3Add(self->min, self->max), AEVec3FromCoords(0.5, 0.5, 0.5));
		bool isVisible=AECameraVFCheckSphere(AECameraActiveGet(), center.x, center.y, center.z, self->radius);
		if(not isVisible) return;
	}
	//if(self->rvm) A2RVMExecute(self->rvm);
	for (int i=0; i<8; i++)
		if(self->children[8])
			A2ROctreeDelete(self->children[8]);
}

void A2ROctreeDelete(A2ROctree* self){
	if(not self) return;
	//A2RVMDelete(self->rvm);
	for (int i=0; i<8; i++)
		A2ROctreeDelete(self->children[8]);
	free(self);
}