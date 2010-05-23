#pragma once
#include <stdlib.h>

typedef struct{
	size_t length,allocated,typeSize;
	void* data;
}AEList;

static AEList* AEListNew_internal(size_t typeSize){
	AEList* list=calloc(1,sizeof(AEList));
	list->typeSize=typeSize;
	return list;
}
#define AEListNew(type) AEListNew_internal(sizeof(type))
#define AEListLength(list) (((AEList*)list)->length)
#define AEListAsArray(list,type) ((type*)(((AEList*)list)->data))
static void AEListResize(AEList* list,size_t length){
	list->allocated=length;
	if(list->allocated<list->length) list->length=length;
	list->data=realloc(list->data,list->allocated*list->typeSize);
}
static void* AEListCheck(AEList* list){
	list->length++;
	if(list->length>list->allocated){
		list->allocated=list->length+30;
		list->data=list->data=realloc(list->data,list->allocated*list->typeSize);
	}
	return list->data+list->typeSize*(list->length-1);
}
static void AEListSweep(AEList* list){
	AEListResize(list,list->length);
}
#define AEListAdd(list,type,element) ((*((type*)AEListCheck(list)))=(element))
static void AEListDelete(AEList* list){
	free(list->data);
	free(list);
}