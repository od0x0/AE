#pragma once
#include "AEVA.h"
#include "AEImage.h"
#include "AECamera.h"

typedef struct A2RMapCell A2RMapCell;
typedef struct A2RMapTrigger A2RMapTrigger;
typedef struct A2RMapCellGroup A2RMapCellGroup;

uint64_t A2RMapTriggerIDGet(A2RMapTrigger* self);

A2RMapCellGroup* A2RMapCellGroupNew(void (*triggerFunc)(A2RMapCellGroup*, A2RMapTrigger*));
void A2RMapCellGroupRender(A2RMapCellGroup* self);
void A2RMapCellGroupCameraPositionSet(A2RMapCellGroup* self, float x, float y, float z);
void A2RMapCellGroupLoadFromFILE(A2RMapCellGroup* self, FILE* file);
void A2RMapCellGroupDelete(A2RMapCellGroup* self);

void A2RMapCellGroupUserdataSet(A2RMapCellGroup* self, void* userdata);
void* A2RMapCellGroupUserdataGet(A2RMapCellGroup* self);
