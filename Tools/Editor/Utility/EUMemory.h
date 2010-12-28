#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void* EUMemoryAllocate(size_t size);
void* EUMemoryReallocate(void* pointer, size_t size);
void* EUMemoryCopy(void* data, size_t size);
#define EUMemoryFree free
extern size_t EUMemoryAllocationsTotal;
extern size_t EUMemoryAllocationBytesTotal;
