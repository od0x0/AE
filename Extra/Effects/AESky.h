#pragma once
#include "AECore.h"

typedef struct{
	AETexture front, back, left, right, top, bottom;
	AEVec3 size;
}AESkybox;

void AESkyboxRender(AESkybox* skybox);
