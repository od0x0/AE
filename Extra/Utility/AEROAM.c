#include "AEROAM.h"

void AEROAMTriangleInit(AEROAMTriangle* self, AEVec3 a, AEVec3 b, AEVec3 c){
	memset(self, 0, sizeof(AEROAMTriangle));
	self->vertices[0]=a;
	self->vertices[1]=b;
	self->vertices[2]=AEVec3Lerp(a, b, 0.5);
}

void AEROAMTriangleSplit(AEROAMTriangle* self){
	if(not self->children)
		self->children=calloc(2, sizeof(AEROAMTriangle));
	const AEVec3 a=self->vertices[0];
	const AEVec3 b=self->vertices[1];
	const AEVec3 c=self->vertices[2];
	const AEVec3 d=AEVec3Lerp(a, b, 0.5);
	AEROAMTriangleInit(self->children+0, c, a, d);
	AEROAMTriangleInit(self->children+1, b, c, d);
}

void AEROAMTriangleDeinit(AEROAMTriangle* self){
	if(self->children){
		for (size_t i=0; i<2; i++) 
			AEROAMTriangleDeinit(self->children+i);
		free(self->children);
	}
	memset(self, 0, sizeof(AEROAMTriangle));
}

#define AEROAMTriangleSampleThreshold 10.0f

void AEROAMTriangleSample(AEROAMTriangle* self, AEROAMHeightSampleFunc func, void* arg){
	float height[3];
	float average = 0;
	for (size_t i=0; i<3; i++) {
		height[i] = func(self->vertices+i,arg);
		self->vertices[i].y=height[i];
		average += height[i];
	}
	average /= 3.0f;
	float totalDifference = 0;
	for (int i=0; i<3; i++) 
		totalDifference += fabsf(height[i]-average);
	
	const AEVec3 u=self->vertices[0];
	const AEVec3 v=self->vertices[1];
	const AEVec3 w=self->vertices[2];
	totalDifference *= AETriangleCalculateArea(u,v,w);
	
	if(totalDifference > AEROAMTriangleSampleThreshold) {
		AEROAMTriangleSplit(self);
		for (int i=0; i<2; i++) AEROAMTriangleSample(self->children+i,func,arg);
	}
}

void AEROAMTriangleEach(AEROAMTriangle* self, AEROAMTriangleEachFunc func, void* arg){
	func(self, arg);
	if(self->children) for (int i=0; i<2; i++) AEROAMTriangleEach(self->children+i, func, arg);
}
