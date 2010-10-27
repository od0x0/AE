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

#define AEArrayInitWithTypeOfSize(array,size) {memset((array),0,sizeof(*(array))); (array)->typeSize=(size);}

#define AEArrayInit(array) AEArrayInitWithTypeOfSize((array),sizeof((array)->data[0]))

//<Oddity007 being thoughtful and thinking of a certain graveyard monster that doesn't understand what lvalues are, and yet still uses pointers. >.> >
static inline void* AEArrayNewWithTypeOfSize(size_t typesize){
	AEArray(void*)* self=malloc(sizeof(AEArray(void*)));
	AEArrayInitWithTypeOfSize(self, typesize);
	return (void*)self;
}
#define AEArrayNew(type) AEArrayNewWithTypeOfSize(sizeof(type))
static inline void AEArrayDelete(void* array){
	if(array==NULL) return;
	AEArrayDeinit(array);
	free(array);
}
//</Oddity007 being thoughtful>

#define AEArrayLength(array) ((array)->length)

#define AEArrayTypeSize(array) ((array)->typeSize)

#define AEArrayLengthInBytes(array) (AEArrayLength(array)*AEArrayTypeSize(array))

#define AEArrayLengthInSizeofType(array,type) (AEArrayLengthInBytes(array)/sizeof(type))

#define AEArrayAsCArray(array) ((array)->data)

#define AEArrayAdd(array,value) {AEArrayCheck((array)); AEArrayAsCArray((array))[AEArrayLength((array))-1]=(value);}