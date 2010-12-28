#pragma once
#include "EUMemory.h"
#include "AE.h"

typedef struct EUOperation EUOperation;
typedef void (*EUOperationFunc)(EUOperation* operation, void* target);

struct EUOperation{
	char* name;
	void* userdata;
	EUOperationFunc function;
};

static inline void EUOperationExecute(EUOperation* self, void* target){
	self->function(self, target);
}
typedef struct{
	AEArray(EUOperation) operations;
}EUOperationList;

void EUOperationListInit(EUOperationList* self);
void EUOperationListDeinit(EUOperationList* self);
void EUOperationListSortAlphabetically(EUOperationList* self);
void EUOperationListSortBySearch(EUOperationList* self, const char* name);
void EUOperationListAdd(EUOperationList* self, const char* name, EUOperationFunc function, void* userdata);
