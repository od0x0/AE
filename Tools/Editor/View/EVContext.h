#pragma once
#include "SDL.h"
#include "AE.h"
#include "EUExceptionCatcher.h"

typedef struct{
	AEVec2i windowSize;
	EUExceptionCatcher* exceptionCatcher;
}EVContext;

void EVContextInit(EVContext* self);
void EVContextFrameUpdate(EVContext* self, double seconds);
void EVContextFixedUpdate(EVContext* self, double seconds);
void EVContextDeinit(EVContext* self);