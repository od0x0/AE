#pragma once
#include "A2RATNodeTexture.h"
#include "A2RATNodeShader.h"

A2RATNodeType* A2RATNodeTypeMaterial(void);

void A2RATNodeMaterialTextureListSet(A2RATNode* self, AEList* textureList);
AEList* A2RATNodeMaterialTextureListGet(A2RATNode* self);

void A2RATNodeMaterialVertexShaderListSet(A2RATNode* self, AEList* shaderList);
AEList* A2RATNodeMaterialVertexShaderListGet(A2RATNode* self);

AEList* A2RATNodeMaterialFragmentShaderListGet(A2RATNode* self);
void A2RATNodeMaterialFragmentShaderListSet(A2RATNode* self, AEList* shaderList);