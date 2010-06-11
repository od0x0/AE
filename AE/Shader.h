#pragma once
#include "Core.h"

typedef struct{
	unsigned int program,vshader,fshader;
	int* textureUniforms;
}AEShader;

AEShader* AEShaderLoad(char* filename);
void AEShaderBind(AEShader* shader,unsigned char textureUnitCount,unsigned int* textures);
void AEShaderDelete(AEShader* shader);
