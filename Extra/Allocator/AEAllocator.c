#include "AEAllocator.h"
/*
typedef struct{
	void** pointer;
	void* next;
	size_t size;
}AEAllocatorBlockHeader;

void AEAllocatorInit(AEAllocator* self){
	if(not self) return;
	self->space=NULL;
	self->max=0;
	self->used=0;
	self->lastBlockAllocated=NULL;
}

void AEAllocatorDeinit(AEAllocator* self){
	if(not self) return;
	free(self->space);
}

void AEAllocatorAllocate(AEAllocator* self,void** pointer,size_t size){
	if(not self) return;
	
	size+=sizeof(AEAllocatorBlockHeader);
	self->used+=size;
	
	if(self->size < self->used){
		self->size=self->used*1.3;
		self->space=realloc(self->space, self->size);
	}
	
	void* data=self->space+(self->used-size);
	AEAllocatorBlockHeader* header=data-sizeof(AEAllocatorBlockHeader);
	
	if(not self->lastBlockAllocated) self->lastBlockAllocated=data;
	else{
		AEAllocatorBlockHeader* lastheader=self->lastBlockAllocated-sizeof(AEAllocatorBlockHeader);
		lastheader->next=data;
	}
	
	header->pointer=pointer;
	header->next=NULL;
	*pointer=data;
	self->lastBlockAllocated=data;
}

void AEAllocatorFree(AEAllocator* self,void** pointer){
	if(not self) return;
	
}

void AEAllocatorCompact(AEAllocator* self){
	
}*/