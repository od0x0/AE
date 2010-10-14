#pragma once
#include <stdlib.h>

typedef struct{
	void* space;
	size_t used, max;
	void* lastBlockAllocated;
}AEAllocator;

void AEAllocatorInit(AEAllocator* self);
void AEAllocatorDeinit(AEAllocator* self);
void AEAllocatorAllocate(AEAllocator* self,void** pointer,size_t size);
void AEAllocatorFree(AEAllocator* self,void** pointer);
void AEAllocatorCompact(AEAllocator* self);