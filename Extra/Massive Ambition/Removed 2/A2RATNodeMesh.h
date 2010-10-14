#pragma once
#include "A2RATNode.h"

A2RATNodeType* A2RATNodeTypeMesh(void);

void A2RATNodeMeshVerticesSet(A2RATNode* self, AEList* vec3list);
AEList* A2RATNodeMeshVerticesGet(A2RATNode* self);

void A2RATNodeMeshNormalsSet(A2RATNode* self, AEList* vec3list);
AEList* A2RATNodeMeshNormalsGet(A2RATNode* self);

void A2RATNodeMeshTexCoordsSet(A2RATNode* self, size_t unit, AEList* vec2list);
AEList* A2RATNodeMeshTexCoordsGet(A2RATNode* self, int unit);

size_t A2RATNodeMeshTexUnitCountGet(A2RATNode* self);