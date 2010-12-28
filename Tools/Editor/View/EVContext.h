#pragma once
#include "SDL.h"
#include "AE.h"
#include "ECOperationList.h"
#include "EVUIElements.h"

//Really messy, will probably be removed.

typedef struct{
	AEVec2i windowSize;
	
	EVUI* ui;
	AEFont* font;
	struct{
		char* searchText;
		size_t scrollOffset;
		AEVec2 scroll;
	}listBox;
	
	ECOperationList* currentOperationList;
	
}EVContext;

void EVContextInit(EVContext* self);
void EVContextFrameUpdate(EVContext* self, double seconds);
void EVContextFixedUpdate(EVContext* self, double seconds);
void EVContextDeinit(EVContext* self);

size_t EVContextUIDoOperationList(EVContext* self, ECOperationList* list, EVUIRect* bounds);