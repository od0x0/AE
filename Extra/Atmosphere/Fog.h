#pragma once
#include "Core.h"

typedef struct{
	GLfloat start, end;
	GLfloat color[4];
	GLfloat density;
	char mode;
}AEFog;

#define AEFogModeLinear 1
#define AEFogModeExp 0
#define AEFogModeExp2 2

void AEFogBind(AEFog* fog);
const char* AEFogFragmentShaderGet(AEFog* fog);
//Exposes vec4 AEFogMixWith(vec4 colorIn); to the GLSL shader