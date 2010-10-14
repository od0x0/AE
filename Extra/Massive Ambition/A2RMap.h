#pragma once
#include "VA.h"
#include "Image.h"
#include "Camera.h"

typedef struct A2RMapCell A2RMapCell;
typedef struct A2RMapTrigger A2RMapTrigger;
typedef struct A2RMap A2RMap;

uint64_t A2RMapTriggerIDGet(A2RMapTrigger* self);

A2RMap* A2RMapNew(void (*triggerFunc)(A2RMap*, A2RMapTrigger*));
void A2RMapRender(A2RMap* self);
void A2RMapCameraPositionSet(A2RMap* self, float x, float y, float z);
void A2RMapLoadFromFILE(A2RMap* self, FILE* file);
void A2RMapDelete(A2RMap* self);

void A2RMapUserdataSet(A2RMap* self, void* userdata);
void* A2RMapUserdataGet(A2RMap* self);
