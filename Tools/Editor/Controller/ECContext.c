#include "ECContext.h"

void ECContextInit(ECContext* self){
	memset(self, 0, sizeof(ECContext));
	self->operationListTable=AETableNew();
}

static void ECContextDeinit_deleteOperationLists(const char* name, void* value, void* arg){
	EUOperationListDeinit(value);
	EUMemoryFree(value);
}

void ECContextDeinit(ECContext* self){
	AETableEach(self->operationListTable, ECContextDeinit_deleteOperationLists, NULL);
	AETableDelete(self->operationListTable);
}

void ECContextAddOperation(ECContext* self, const char* listName, const char* name, EUOperationFunc function, void* userdata){
	EUOperationList* list=AETableGet(self->operationListTable, listName);
	if(list==NULL){
		list=EUMemoryAllocate(sizeof(EUOperationList));
		EUOperationListInit(list);
	}
	EUOperationListAdd(list, name, function, userdata);
	AETableSet(self->operationListTable, listName, list);
}