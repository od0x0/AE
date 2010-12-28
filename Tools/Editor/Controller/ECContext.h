#pragma once
#include "AE.h"
#include "EUMemory.h"
#include "EUOperationList.h"

typedef struct{
	AETable* operationListTable;
}ECContext;

void ECContextInit(ECContext* self);
void ECContextAddOperation(ECContext* self, const char* listName, const char* name, EUOperationFunc function, void* userdata);
void ECContextDeinit(ECContext* self);