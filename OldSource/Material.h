#pragma once
#include "Core.h"
#include "Shader.h"
#include "List.h"

typedef struct{
	unsigned int textures[2];
	unsigned char count;
	AEShader* shader;
}AEMaterial;

extern AEList* AEMaterials;

unsigned int AEMaterialLoad(char* path);
void AEMaterialBind(unsigned int material,unsigned int additionalTexture);
void AEMaterialDelete(unsigned int material);