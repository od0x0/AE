#pragma once
#include <stdlib.h>
#include <string.h>

//Do not access directly
#define AEArray(Type) struct{size_t length, allocated, typeSize; Type* data;}

void AEArrayDeinit(void* array);
void AEArrayResize(void* array,size_t length);
void* AEArrayCheck(void* array);
void AEArraySweep(void* array);
void AEArrayAddBytes(void* array,void* bytes);
size_t AEArrayFindIndexOfBytes(void* array,void* bytes);
size_t AEArrayAddBytesUnique(void* array,void* data);
void AEArrayRemoveDuplicates(void* array,void* _outarray,void* _indices);
void AEArrayRemoveBytes(void* array,void* bytes);

#define AEArrayInitForTypeOfSize(array,size) {memset((array),0,sizeof(*(array))); (array)->typeSize=(size);}

#define AEArrayInit(array) AEArrayInitForTypeOfSize((array),sizeof((array)->data[0]))

#define AEArrayLength(array) ((array)->length)

#define AEArrayTypeSize(array) (sizeof((array)->data[0]))
//((array)->typeSize)

#define AEArrayLengthInBytes(array) (AEArrayLength(array)*AEArrayTypeSize(array))

#define AEArrayLengthInSizeofType(array,type) (AEArrayLengthInBytes(array)/sizeof(type))

#define AEArrayAsCArray(array) ((array)->data)

#define AEArrayAdd(array,value) {AEArrayCheck((array)); AEArrayAsCArray((array))[AEArrayLength((array))]=(value);}