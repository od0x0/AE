#pragma once
#include "Core.h"

typedef struct AEOcta{
	float x,y,z,size;
	void* meshData;
	struct AEOcta* children;
}AEOcta;

AEOcta* AEOctaAddChildren(AEOcta* octa);
void AEOctaDelete(AEOcta* octa);
AEOcta* AEOctaGetAt(AEOcta* octa,float x,float y,float z);
void AEOctaRender(AEOcta* octa);