#pragma once
#include "Core.h"
#include "Shader.h"

#pragma pack(push, 1)

//Map Geometry Classes

typedef struct{
	unsigned int indexStart,indexEnd;
}AEGeoSegment;

void AEGeoSegmentDraw(AEGeoSegment seg);

typedef struct{
	float x,y,z;
	AEVec3 n;
	AEVec2 t,lt;
}AEGeoVert;

typedef struct{
	unsigned int vbo,ibo,vcount,vallocated,*indices,icount,iallocated;
	AEGeoVert* verts;
}AEGeo;

extern AEGeo* AEGeoActive;

void AEGeoBind(AEGeo* geo,AEShader* shader);
AEGeo* AEGeoLoad(char* path);
void AEGeoDelete(AEGeo* geo);
unsigned int AEGeoAddVert(AEGeo* geo,AEGeoVert* vert);
void AEGeoCompile(AEGeo* geo,unsigned int isStreamed);

#pragma pack(pop)