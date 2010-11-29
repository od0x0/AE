#pragma once
#include "AECore.h"

typedef struct{
	GLfloat start, end;
	AERGBA color;
	GLfloat density;
	char mode;
}AEFog;

#define AEFogModeLinear 1
#define AEFogModeExp 0
#define AEFogModeExp2 2

void AEFogBind(AEFog* fog);
const char* AEFogFragmentShaderGet(AEFog* fog);
//Exposes vec4 AEFogMixWith(vec4 colorIn); to the GLSL shader