#pragma once
#include "AE.h"

typedef struct{
	AEVec3 position;
	size_t w, h, resolution;
	AEVA va, ia;//T2F V3F format
}AEWaterGrid;

void AEWaterGridInit(AEWaterGrid* self);
void AEWaterGridDeinit(AEWaterGrid* self);
void AEWaterGridRender(AEWaterGrid* self);
void AEWaterGridUpdate(AEWaterGrid* self, double seconds);