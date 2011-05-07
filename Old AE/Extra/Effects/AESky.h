#pragma once
#include "AE.h"

void AESkyUtilityRenderBoxWithCubemap(AEVec3 min, AEVec3 max, GLuint cubemap);
void AESkyUtilityRenderBoxWithSixTextures(AEVec3 min, AEVec3 max, GLuint* textures);//In the ogl cubemap face order
