/*
 *  Sky.h
 *  AEED
 *
 *  Created by Oliver Daids on 8/25/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once
#include "AECore.h"

typedef struct{
	AETexture front, back, left, right, top, bottom;
	AEVec3 size;
}AESkybox;

void AESkyboxRender(AESkybox* skybox);