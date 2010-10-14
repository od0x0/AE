#pragma once
#include "Core.h"
#include "Material.h"
#include "VA.h"

typedef struct{
	AEVec3* centers;
	AEVec3* sizes;
	AERGBAub* colors;
	AEVec3* vertices;
	float* texcoords;
	size_t grassQuadCount;
	
	char cylindrical;
}AEGrassPatch;

AEGrassPatch* AEGrassPatchNew(bool cylindrical, size_t grassQuadCount, AEVec3* centers, AEVec3* sizes, AERGBAub* colors/*Perface color, which is translated to pervertex*/);

void AEGrassPatchRender(AEGrassPatch* patch);

void AEGrassPatchDelete(AEGrassPatch* patch);

const char* AEGrassPatchVertexShaderGet(AEGrassPatch* patch);

void AEGrassPatchAlterImageToSetupForAlphaTest(AEImage* image);
