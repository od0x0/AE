#pragma once
#include "AE.h"
#include "VA.h"
#include "Table.h"

typedef struct A2RMapCell A2RMapCell;
/*
A2RMapCell* A2RMapCellNew(void);
A2RMapCell* A2RMapCellRetain(A2RMapCell* self);
void A2RMapCellDelete(A2RMapCell* self);
void A2RMapCellRender(A2RMapCell* self);
uint32_t* A2RMapCellChildrenGet(A2RMapCell* self);
uint64_t A2RMapCellChildCountGet(A2RMapCell* self);
void A2RMapCellSerializeToFILE(A2RMapCell* self,FILE* file);
void A2RMapCellUnserializeFromFILE(A2RMapCell* self,FILE* file, void(*childcellloader)(uint32_t cellID, void* arg), void* arg);
uint64_t A2RMapCellIDGet(A2RMapCell* self);
AEVec3* A2RMapCellPositionGet(A2RMapCell* self);
AEVec3* A2RMapCellSizeGet(A2RMapCell* self);
*/
typedef struct A2RMap A2RMap;
/*
A2RMap* A2RMapNew(void);
A2RMap* A2RMapRetain(A2RMap* self);
void A2RMapDelete(A2RMap* self);

void A2RMapRender(A2RMap* self);
void A2RMapStep(A2RMap* self, float stepInSeconds);*/