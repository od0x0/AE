#pragma once
#include "../../AE/Core.h"

typedef struct{
	AEVec3 verts[6][4];
	unsigned int textures[6];
}GridCube;

typedef struct{
	AEVec3i cubeCount;
	AEVec3 sizeOfACube;
	GridCube* cubes;
}Grid;