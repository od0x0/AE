#pragma once
#include "AE.h"

typedef struct AEROAMTriangle AEROAMTriangle;
struct AEROAMTriangle{
	AEVec3 vertices[3];
	AEROAMTriangle* children;
};

typedef float (*AEROAMHeightSampleFunc)(AEVec3* vertex, void* arg);
typedef void (*AEROAMTriangleEachFunc)(AEROAMTriangle* self, void* arg);

void AEROAMTriangleInit(AEROAMTriangle* self, AEVec3 a, AEVec3 b, AEVec3 c);
void AEROAMTriangleSplit(AEROAMTriangle* self);
void AEROAMTriangleDeinit(AEROAMTriangle* self);
void AEROAMTriangleSample(AEROAMTriangle* self, AEROAMHeightSampleFunc func, void* arg);
void AEROAMTriangleEach(AEROAMTriangle* self, AEROAMTriangleEachFunc func, void* arg);
