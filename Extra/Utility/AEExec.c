#include "AEExec.h"

struct AEExecItem{
	AEExecItemFunc func;
	void* arg;
	uint64_t tag;
	uint32_t priority;
};

AEExecItemFunc AEExecItemGetFunc(AEExecItem* self){
	return self->func;
}

void* AEExecItemGetArg(AEExecItem* self){
	return self->arg;
}

uint32_t AEExecItemGetTag(AEExecItem* self){
	return self->tag;
}

uint32_t AEExecItemGetPriority(AEExecItem* self){
	return self->priority;
}

struct AEExecQueue{
	/*AEExecItem* items;
	size_t allocated;*/
	AEArray(AEExecItem) itemArray;
	size_t startingIndex;
};

AEExecQueue* AEExecQueueNew(void){
	AEExecQueue* self=calloc(1, sizeof(AEExecQueue));
	/*self->items=calloc(10, sizeof(AEExecItem));
	self->allocated=10;*/
	AEArrayInit(& self->itemArray);
	return self;
}

void AEExecQueueDelete(AEExecQueue* self){
	if(not self) return;
	//free(self->items);
	AEArrayDeinit(& self->itemArray);
	free(self);
}

size_t AEExecQueueRun(AEExecQueue* self, size_t execLimit, bool shouldRemoveItemsAfterCompletion){
	/*
	size_t execCount=0;
	for (size_t i=0; i < self->allocated; i++) {
		AEExecItem* item=self->items+i;
		if(not item->func) continue;
		++execCount;
		if(execLimit and (execCount > execLimit)) return execCount;
		item->func(item);
		memset(item, 0, sizeof(AEExecItem));
	}*/
	static const AEExecItem blankItem;
	AEExecItem* items=AEArrayAsCArray(& self->itemArray);
	size_t itemCount=AEArrayLength(& self->itemArray);
	printf("Item Count: %i\n", (int)itemCount);
	if(itemCount==0) return 0;
	if(execLimit and itemCount > execLimit) itemCount=execLimit;
	for (size_t i=self->startingIndex; i < itemCount; i++) {
		AEExecItem* item=items+i;
		if(item->func) item->func(item);
		if(shouldRemoveItemsAfterCompletion) *item=blankItem;
	}
	if(shouldRemoveItemsAfterCompletion) self->startingIndex+=itemCount;
	/*if(self->startingIndex < AEArrayLength(& self->itemArray) and shouldRemoveItemsAfterCompletion){
		memset(items+self->startingIndex, 0, itemCount * sizeof(AEExecItem));
		self->startingIndex+=itemCount;
	}*/
	return itemCount;
}

void AEExecQueuePush(AEExecQueue* self, uint64_t tag, uint32_t priority, AEExecItemFunc func, const void* arg){
	/*for (size_t i=0; i < self->allocated; i++) {
		AEExecItem* item=self->items+i;
		if(item->func) continue;
		//If we get here, then we've found an empty slot
		//item->arg=arg;
		item->arg=arg;
		item->func=func;
		return;
	}
	const size_t oldallocated=self->allocated;
	self->allocated=self->allocated*1.25+2;
	self->items=realloc(self->items, sizeof(AEExecItem)*self->allocated);
	memset(self->items+oldallocated, 0, sizeof(AEExecItem)*(self->allocated-oldallocated));*/
	static AEExecItem blankItem={0,0,0,0};
	self->startingIndex=0;
	AEExecItem item;
	item.tag=tag;
	item.priority=priority+1;
	item.func=func;
	item.arg=(void*)arg;
	size_t index=AEArrayFindIndexOfBytes(& self->itemArray, & blankItem);
	if(not index) AEArrayAddBytes(& self->itemArray, & item);
	else AEArrayAsCArray(& self->itemArray)[index-1]=item;
}

static int CompareItems(const void* a, const void* b){
	int64_t priority1 = ((const AEExecItem*)a)->priority;
	int64_t priority2 = ((const AEExecItem*)b)->priority;
	return priority1 - priority2;
}

void AEExecQueueSort(AEExecQueue* self){
	size_t itemCount=AEArrayLength(&self->itemArray);
	AEExecItem* items=AEArrayAsCArray(&self->itemArray);
	qsort(items, itemCount, AEArrayTypeSize(&self->itemArray), CompareItems);
}

AEExecItem* AEExecQueueLookupItem(AEExecQueue* self, uint64_t tag){
	if(not tag) return NULL;
	for (size_t i=0; i<AEArrayLength(& self->itemArray); i++) {
		AEExecItem* item=AEArrayAsCArray(& self->itemArray)+i;
		if(item->tag==tag) return item;
	}
	return NULL;
}
