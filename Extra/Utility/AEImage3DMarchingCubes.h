#pragma once
#include "AEImage3D.h"

typedef struct {
   double x,y,z;
   double nx, ny, nz;
} AEMCVertex;

typedef struct {
	AEMCVertex vertices[3];
} AEMCTriangle;

typedef AEArray(AEMCTriangle) AEMCTriangleArray;

void AEImage3DMarchingCubes(AEImage3D* self, AEVec3i* step, double isovalue, AEMCTriangleArray* triangles);
