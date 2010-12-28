#pragma once
#include "EUMemory.h"
#include "AE.h"

typedef struct ECOperation ECOperation;
typedef void (*ECOperationFunc)(ECOperation* operation, void* target);

struct ECOperation{
	char* name;
	void* userdata;
	ECOperationFunc function;
};

static inline void ECOperationExecute(ECOperation* self, void* target){
	self->function(self, target);
}
typedef struct{
	AEArray(ECOperation) operations;
}ECOperationList;

void ECOperationListInit(ECOperationList* self);
void ECOperationListDeinit(ECOperationList* self);
void ECOperationListSortAlphabetically(ECOperationList* self);
void ECOperationListSortBySearch(ECOperationList* self, const char* name);
void ECOperationListAdd(ECOperationList* self, const char* name, ECOperationFunc function, void* userdata);
