#pragma once
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//Do not access directly
typedef struct{
	size_t length,allocated;
	short typeSize, refcount;
	void* data;
}AEList;


AEList* AEListNewWithTypeSize(size_t typeSize);
AEList* AEListRetain(AEList* list);
AEList* AEListCopy(AEList* list);
void AEListDelete(AEList* list);
void AEListResize(AEList* list,size_t length);
void* AEListCheck(AEList* list);
void AEListSweep(AEList* list);
void AEListAddBytes(AEList* list,void* bytes);
size_t AEListAddBytesUnique(AEList* list,void* data);
size_t AEListFindIndexOfBytes(AEList* list,void* bytes);
void AEListRemoveDuplicates(AEList* list,AEList* outlist,AEList* outindices);
void AEListRemoveBytes(AEList* list,void* bytes);

#define AEListNew(type) (AEListNewWithTypeSize(sizeof(type)))
/*
#define AEListLength(list) (((AEList*)list)->length)

#define AEListTypeSize(list) (((AEList*)list)->typeSize)

#define AEListLengthInBytes(list) (AEListLength(list)*AEListTypeSize(list))

#define AEListLengthInSizeofType(list,type) (AEListLengthInBytes(list)/sizeof(type))

#define AEListAsArray(list,type) ((type*)(((AEList*)list)->data))

#define AEListAdd(list,type,element) ((*((type*)AEListCheck(list)))=(element))

#define AEListRemove(list,type,element) do{type bytes=element;AEListRemoveBytes(list,&bytes);}while(0)*/


#define AEListLength(list) ((list) ? ((list)->length) : 0)

#define AEListTypeSize(list) ((list)->typeSize)

#define AEListLengthInBytes(list) (AEListLength(list)*AEListTypeSize(list))

#define AEListLengthInSizeofType(list,type) (AEListLengthInBytes(list)/sizeof(type))

#define AEListAsArray(list,type) ((type*)((list)->data))

#define AEListAdd(list,type,element) ((*((type*)AEListCheck(list)))=(element))

#define AEListRemove(list,type,element) do{type bytes=element;AEListRemoveBytes(list,&bytes);}while(0)