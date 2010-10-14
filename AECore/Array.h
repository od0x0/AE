#pragma once
#include <stdlib.h>
#include <string.h>

//Work in progress replacement for AEList, allows more typechecking

//Do not access directly
#define AEArray(Type) struct{size_t length, allocated, typeSize; Type* data;}
//We don't actually need the typeSize field, but it is nice to have

void AEArrayDelete(void* array);
void AEArrayResize(void* array,size_t length);
void* AEArrayCheck(void* array);
void AEArraySweep(void* array);
void AEArrayAddBytes(void* array,void* bytes);
size_t AEArrayAddBytesUnique(void* array,void* data);
size_t AEArrayFindIndexOfBytes(void* array,void* bytes);
void AEArrayRemoveDuplicates(void* array,void* indices);
void AEArrayRemoveBytes(void* array,void* bytes);

#define AEArrayInitForTypeOfSize(array,size) ((array)->typeSize=(size))

#define AEArrayInit(array) AEArrayInitForTypeOfSize((array),(array)->data[0])

#define AEArrayLength(array) ((array)->length)

#define AEArrayTypeSize(array) (sizeof((array)->data[0]))
//((array)->typeSize)

#define AEArrayLengthInBytes(array) (AEArrayLength(array)*AEArrayTypeSize(array))

#define AEArrayLengthInSizeofType(array,type) (AEArrayLengthInBytes(array)/sizeof(type))

#define AEArrayAsCArray(array) ((array)->data)

#define AEArrayEnsure(array) AEArrayEnsureWithTypeSize((array),AEArrayTypeSize((array)))

#define AEArrayAdd(array,value) ((AEArrayAsCArray(AEArrayEnsure(array))[AEArrayLength(array)])=(value))

#define AEArrayRemove(array,type,element) do{type bytes=element;AEArrayRemoveBytes(array,&bytes);}while(0)