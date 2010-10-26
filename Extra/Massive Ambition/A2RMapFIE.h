#pragma once
#include "AECore.h"
#include "AEVA.h"
#include "AEMaterial.h"

typedef struct{
	AEVA va;
	AEVA ia;
	AEMaterial* material;
}A2RMapFIE;

void A2RMapFIEInit(A2RMapFIE* self);
void A2RMapFIEDeinit(A2RMapFIE* self);
AEVA* A2RMapFIEVAGetPointer(A2RMapFIE* self);
AEVA* A2RMapFIEIAGetPointer(A2RMapFIE* self);
AEMaterial* A2RMapFIEMaterialGet(A2RMapFIE* self);
void A2RMapFIEMaterialSet(A2RMapFIE* self, AEMaterial* material);