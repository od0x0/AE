#pragma once
#include "AECore.h"
#include "AEVA.h"
#include "AEMaterial.h"

typedef struct A2RMapEntity A2RMapEntity;

A2RMapEntity* A2RMapEntityNew(void);
A2RMapEntity* A2RMapEntityRetain(A2RMapEntity* self);
void A2RMapEntityDelete(A2RMapEntity* self);
void A2RMapEntityRender(A2RMapEntity* self);

AEVA* A2RMapEntityVAGetPointer(A2RMapEntity* self);
AEVA* A2RMapEntityIAGetPointer(A2RMapEntity* self);

void* A2RMapEntityUserdataGet(A2RMapEntity* self);
void A2RMapEntityUserdataSet(A2RMapEntity* self, void* data);

AEMaterial* A2RMapEntityMaterialGet(A2RMapEntity* self);
void A2RMapEntityMaterialSet(A2RMapEntity* self, AEMaterial* material);

void A2RMapEntityPositionGet(A2RMapEntity* self, AEVec3* v3);
void A2RMapEntityRotationGet(A2RMapEntity* self, AEVec3* v3);
void A2RMapEntityPositionSet(A2RMapEntity* self, float x, float y, float z);
void A2RMapEntityRotationSet(A2RMapEntity* self, float x, float y, float z);