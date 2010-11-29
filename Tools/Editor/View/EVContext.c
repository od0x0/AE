#include "EVContext.h"

void EVContextInit(EVContext* self){
	memset(self, 0, sizeof(EVContext));
}

void EVContextFrameUpdate(EVContext* self, double seconds){
	
}

void EVContextFixedUpdate(EVContext* self, double seconds){
	
}

void EVContextDeinit(EVContext* self){
	memset(self, 0, sizeof(EVContext));
}