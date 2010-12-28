#include "EUMemory.h"

size_t EUMemoryAllocationsTotal;
size_t EUMemoryAllocationBytesTotal;

void* EUMemoryAllocate(size_t size){
	if(size==0) return NULL;//Standard is undefined on this, so I guarrantee it.
	void* pointer = calloc(1, size);
	if(pointer == NULL){
		fprintf(stderr, "%s(%lu): Memory allocation failed, you probably ran out of memory.\n", __func__,(unsigned long)size);
		abort();
	}
	EUMemoryAllocationBytesTotal+=size;
	EUMemoryAllocationsTotal++;
	return pointer;
}

void* EUMemoryReallocate(void* old, size_t size){
	if(size==0) return NULL;//Standard is undefined on this, so I guarrantee it.
	void* pointer = realloc(old, size);
	if(pointer == NULL){
		fprintf(stderr, "%s(%p, %lu): Memory allocation failed, you probably ran out of memory.\n", __func__, old, (unsigned long)size);
		abort();
	}
	return pointer;
}
void* EUMemoryCopy(void* data, size_t size){
	return memcpy(EUMemoryAllocate(size), data, size);
}
