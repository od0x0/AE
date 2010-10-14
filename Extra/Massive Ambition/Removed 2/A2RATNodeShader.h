#pragma once
#include "A2RATNode.h"

A2RATNodeType* A2RATNodeTypeShader(void);

void A2RATNodeShaderTextSet(A2RATNode* self, int detaillevel, char* text);
char* A2RATNodeShaderTextGet(A2RATNode* self, int detaillevel);

typedef enum{
	A2RATNodeShaderTypeNone,
	A2RATNodeShaderTypeVertex,
	A2RATNodeShaderTypeFragment
}A2RATNodeShaderType;

void A2RATNodeShaderTypeSet(A2RATNode* self, A2RATNodeShaderType type);
A2RATNodeShaderType A2RATNodeShaderTypeGet(A2RATNode* self);