#pragma once
#include "A2RATNode.h"

A2RATNodeType* A2RATNodeTypeTexture(void);

void A2RATNodeTextureImageSet(A2RATNode* self, AEImage* image);
AEImage* A2RATNodeTextureImageGet(A2RATNode* self);