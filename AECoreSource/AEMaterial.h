#pragma once
#include "AEImage.h"

#warning AEMaterial is deprecated.

//A very unstable multitexture/shader material system, don't expect it to remain here.

//EXT means that it's not guarranteed to stay in here, be warned when using it

//struct AEMaterial;
typedef struct AEMaterial AEMaterial;

bool AEMaterialsShadersEnabledGet(void);
void AEMaterialsShadersEnabledSet(bool on);

float AEMaterialsTimeGet(void);
void AEMaterialsTimeSet(float seconds);

AEMaterial* AEMaterialNew(void);

AEMaterial* AEMaterialRetain(AEMaterial* material);
//(deprecated)

void AEMaterialDelete(AEMaterial* material);

void AEMaterialBind(AEMaterial* material);
void AEMaterialBindLightmapEXT(AEMaterial* material,AETexture lightmap);
void AEMaterialBindShaderOnlyEXT(AEMaterial* material);

//Does not store a copy of the shader text!
//Calling it with both texts as NULL links the shaders and generates a program
void AEMaterialShaderSet(AEMaterial* material,const char* vshadertext,const char* fshadertext);
//Null terminated array of Null terminated strings
void AEMaterialShaderSetMulti(AEMaterial* material,const char** vshadertextarray,const char** fshadertextarray);

void AEMaterialTextureSet(AEMaterial* material,const char* name,AETexture texture);