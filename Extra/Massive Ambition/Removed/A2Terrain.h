#pragma once
#include "A2TerrainCell.h"
#include "Exec.h"

typedef struct{
	AEList* cellList;
	AEExecQueue* execQueue;
	AEVec3 lastCameraPosition;
	size_t activeCell;
	char cameraHasMoved;
	char readyToLoad;
	char unused1;
	char unused2;
	char* path;
}A2Terrain;

A2Terrain* A2TerrainNew(void);
void A2TerrainDelete(A2Terrain* self);
void A2TerrainRender(A2Terrain* self);
void A2TerrainStep(A2Terrain* self, float stepInSeconds);