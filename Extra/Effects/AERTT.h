#pragma once
#include "AE.h"

typedef struct{
	//Read only
	AETexture texture;	
	int textureSize;
	
	//Internal use only
	GLint viewportBounds[4];
}AERTT;

void AERTTInit(AERTT* self, int textureSize);
void AERTTDeinit(AERTT* self);
void AERTTBeginCapture(AERTT* self);
void AERTTEndCapture(AERTT* self);