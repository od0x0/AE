#pragma once
#include "Core.h"

typedef struct{
	AEVBO* vbo;
	AEVec2* texcoords;
	unsigned int tbo;
	unsigned char textureUnitCount;
}AEMTVBO;

AEMTVBO* AEMTVBOLoad(const char* filename,int isStatic,int hasNormals,unsigned char textureUnitCount);
void AEMTVBOAdd(AEMTVBO* vbo,AEVBOVertWithNormal* v,AEVec2* auxTexCoords);
void AEMTVBODraw(AEMTVBO* vbo);
void AEMTVBOCompile(AEMTVBO* vbo,unsigned int* usages);
void AEMTVBODelete(AEMTVBO* vbo);