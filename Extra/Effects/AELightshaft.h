#pragma once
#include "AE.h"

typedef struct{
	AERGBAub color;
	AEVec3 position;
}AELightshaftUtilityVertex;

void AELightshaftUtilityCreateVertices(AELightshaftUtilityVertex* outlineVertices, size_t outlineVertexCount, AELightshaftUtilityVertex* vertices, const AEVec3 direction, const float alphaFade);//Vertices is 4x the outline vertex count
void AELightshaftUtilityRender(AELightshaftUtilityVertex* vertices, size_t vertexCount);