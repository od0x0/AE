#pragma once
#include "Core.h"

typedef struct AEOcta{
	float x,y,z,size;
	
	AEVBO** vbos;
	unsigned int* textures;
	unsigned char count;
	
	struct AEOcta* children;
}AEOcta;

AEOcta* AEOctaAddChildren(AEOcta* octa);
void AEOctaAddVBO(AEOcta* octa,unsigned int texture,AEVBO* vbo);
void AEOctaRender(AEOcta* octa);
void AEOctaDelete(AEOcta* octa);
AEOcta* AEOctaGetAt(AEOcta* octa,float x,float y,float z);
void AEOctaCompile(AEOcta* octa,unsigned int staticVBO,unsigned int staticIBO);