#pragma once
#include "VBO.h"
#include "Material.h"

struct A2RVMEnt;
typedef struct A2RVMEnt A2RVMEnt;

A2RVMEnt* A2RVMEntNew(void);
A2RVMEnt* A2RVMEntRetain(A2RVMEnt* self);
void A2RVMEntDelete(A2RVMEnt* self);

void A2RVMEntParentSet(A2RVMEnt* self, A2RVMEnt* parent);
A2RVMEnt* A2RVMEntParentGet(A2RVMEnt* self);

void A2RVMEntNameSet(A2RVMEnt* self, char* name);
char* A2RVMEntNameGet(A2RVMEnt* self);

void A2RVMEntPositionSet(A2RVMEnt* self, float x, float y, float z);
void A2RVMEntPositionGet(A2RVMEnt* self, AEVec3* v3);

void A2RVMEntRotationSet(A2RVMEnt* self, float x, float y, float z);
void A2RVMEntRotationGet(A2RVMEnt* self, AEVec3* v3);

void A2RVMEntCullableSet(A2RVMEnt* self, bool value);
bool A2RVMEntCullableGet(A2RVMEnt* self);

void A2RVMEntStaticSet(A2RVMEnt* self, bool value);
bool A2RVMEntStaticGet(A2RVMEnt* self);

void A2RVMEntIsRelativeToCameraSet(A2RVMEnt* self, bool value);
bool A2RVMEntIsRelativeToCameraGet(A2RVMEnt* self);

AEVA* A2RVMEntVAGet(A2RVMEnt* self);
AEVA* A2RVMEntIAGet(A2RVMEnt* self);

void A2RVMEntVFCGen(A2RVMEnt* self);

#define A2RVMEntLightmapTextureSlot (-1)
void A2RVMEntTextureSet(A2RVMEnt* self, int slot, AEImage* image);