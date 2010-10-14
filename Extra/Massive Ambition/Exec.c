#include "Exec.h"

struct AEExecItem{
	AEExecItemFunc func;
	uint8_t arg[16];
};

AEExecItemFunc AEExecItemFuncGet(AEExecItem* self){
	return self->func;
}

void* AEExecItemArgGet(AEExecItem* self){
	return self->arg;
}

struct AEExecQueue{
	AEExecItem* items;
	size_t allocated;
};

AEExecQueue* AEExecQueueNew(void){
	AEExecQueue* self=calloc(1, sizeof(AEExecQueue));
	self->items=calloc(10, sizeof(AEExecItem));
	self->allocated=10;
	return self;
}

void AEExecQueueDelete(AEExecQueue* self){
	if(not self) return;
	free(self->items);
	free(self);
}

size_t AEExecQueueRun(AEExecQueue* self,size_t execLimit){
	size_t execCount=0;
	for (size_t i=0; i < self->allocated; i++) {
		AEExecItem* item=self->items+i;
		if(not item->func) continue;
		++execCount;
		if(execLimit and (execCount > execLimit)) return execCount;
		item->func(item);
		memset(item, 0, sizeof(AEExecItem));
	}
	return execCount;
}

void AEExecQueuePush(AEExecQueue* self,AEExecItemFunc func,void* arg,size_t argsize){
	for (size_t i=0; i < self->allocated; i++) {
		AEExecItem* item=self->items+i;
		if(item->func) continue;
		//If we get here, then we've found an empty slot
		//item->arg=arg;
		if(argsize > sizeof(item->arg)) return;
		if(arg) memcpy(item->arg, arg, argsize);
		item->func=func;
		return;
	}
	const size_t oldallocated=self->allocated;
	self->allocated=self->allocated*1.25+2;
	self->items=realloc(self->items, sizeof(AEExecItem)*self->allocated);
	memset(self->items+oldallocated, 0, sizeof(AEExecItem)*(self->allocated-oldallocated));
}