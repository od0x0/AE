#pragma once
#include "AE.h"

typedef struct AESubQuad AESubQuad;
struct AESubQuad{
	AEVec3 vertices[4];
	AESubQuad* children;
};

typedef void (*AESubQuadEachFunc)(AESubQuad* self, void* arg);

void AESubQuadInit(AESubQuad* self);
void AESubQuadSubdivide(AESubQuad* self);
void AESubQuadEach(AESubQuad* self, AESubQuadEachFunc func, void* arg);
void AESubQuadDeinit(AESubQuad* self);
