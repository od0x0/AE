#pragma once
#include "Core.h"
#include "Geo.h"
#include "List.h"

typedef struct{
	AEGeoSegment seg;
	unsigned int material,lightmap;
}AEOctaGeomSegment;

typedef struct AEOcta{
	float x,y,z,size;
	AEGeo* geo;
	AEList* segList;//Of AEOctaGeomSegment
	struct AEOcta* children;
}AEOcta;

AEOcta* AEOctaAddChildren(AEOcta* octa);
void AEOctaDeleteChildren(AEOcta* octa);
AEOcta* AEOctaGetAt(AEOcta* octa,float x,float y,float z);
void AEOctaRender(AEOcta* octa);
void AEOctaAddVBO(AEOcta* octa,unsigned int material,AEVBO* vbo);