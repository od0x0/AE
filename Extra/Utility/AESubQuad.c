#include "AESubQuad.h"

void AESubQuadInit(AESubQuad* self){
	memset(self, 0, sizeof(AESubQuad));
}

void AESubQuadSubdivide(AESubQuad* self){
	if(self->children) return;
	self->children = calloc(4, sizeof(AESubQuad));
	AEVec3 center={0,0,0};
	for (int i=0; i<4; i++) {
		center.x+=self->vertices[i].x;
		center.y+=self->vertices[i].y;
		center.z+=self->vertices[i].z;
	}
	center.x*=0.25;
	center.y*=0.25;
	center.z*=0.25;
	for (int i=0; i<4; i++) {
		AESubQuad* child=self->children+i;
		child->vertices[0]=center;
		child->vertices[1]=AEVec3Lerp(self->vertices[(i+4-1)%4], self->vertices[(i+4)%4], 0.5);
		child->vertices[2]=self->vertices[i];
		child->vertices[3]=AEVec3Lerp(self->vertices[(i+4)%4], self->vertices[(i+4+1)%4], 0.5);
	}
}

void AESubQuadEach(AESubQuad* self, AESubQuadEachFunc func, void* arg){
	if(self->children) 
		for (int i=0; i<4; i++) 
			AESubQuadEach(self->children+i, func, arg);
	func(self, arg);
}

void AESubQuadDeinit(AESubQuad* self){
	if(self->children) 
		for (int i=0; i<4; i++) 
			AESubQuadDeinit(self->children+i);
	free(self->children);
}
