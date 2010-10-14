#pragma once
#include "A2RATNode.h"

A2RATNodeType* A2RATNodeTypeEnt(void);

void A2RATNodeEntPositionSet(A2RATNode* self, float x, float y, float z);
void A2RATNodeEntPositionGet(A2RATNode* self, AEVec3* v3);

void A2RATNodeEntRotationSet(A2RATNode* self, float x, float y, float z);
void A2RATNodeEntRotationGet(A2RATNode* self, AEVec3* v3);

void A2RATNodeEntMeshSet(A2RATNode* self, A2RATNode* mesh);
A2RATNode* A2RATNodeEntMeshGet(A2RATNode* self);

void A2RATNodeEntTrackingNameSet(A2RATNode* self, char* name);
char* A2RATNodeEntTrackingNameGet(A2RATNode* self);