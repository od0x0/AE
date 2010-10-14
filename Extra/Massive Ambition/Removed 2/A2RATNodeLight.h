#pragma once
#include "A2RATNode.h"

A2RATNodeType* A2RATNodeTypeLight(void);

void A2RATNodeLightPositionSet(A2RATNode* self, float x, float y, float z);
void A2RATNodeLightPositionGet(A2RATNode* self, AEVec3* v3);

void A2RATNodeLightRotationSet(A2RATNode* self, float x, float y, float z);
void A2RATNodeLightRotationGet(A2RATNode* self, AEVec3* v3);

void A2RATNodeLightRadiusSet(A2RATNode* self, float radius);
float A2RATNodeLightRadiusGet(A2RATNode* self);

void A2RATNodeLightTrackingNameSet(A2RATNode* self, char* name);
char* A2RATNodeLightTrackingNameGet(A2RATNode* self);