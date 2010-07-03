#pragma once
#include <stdlib.h>

typedef struct{
	size_t length,allocated,typeSize;
	void* data;
}AEList;

inline static AEList* AEListNewWithTypeSize(size_t typeSize){
	AEList* list=calloc(1,sizeof(AEList));
	list->typeSize=typeSize;
	return list;
}
#define AEListNew(type) AEListNewWithTypeSize(sizeof(type))
#define AEListLength(list) (((AEList*)list)->length)
#define AEListAsArray(list,type) ((type*)(((AEList*)list)->data))
inline static void AEListResize(AEList* list,size_t length){
	list->allocated=length;
	if(list->allocated<list->length) list->length=length;
	list->data=realloc(list->data,list->allocated*list->typeSize);
}
inline static void* AEListCheck(AEList* list){
	list->length++;
	if(list->length>list->allocated){
		list->allocated=list->length+30;
		list->data=list->data=realloc(list->data,list->allocated*list->typeSize);
	}
	return list->data+list->typeSize*(list->length-1);
}
inline static void AEListSweep(AEList* list){
	AEListResize(list,list->length);
}

#define AEListTypeSize(list) (((AEList*)list)->typeSize)
#define AEListLengthInBytes(list) (AEListLength(list)*AEListTypeSize(list))

inline static void AEListAddBytes(AEList* list,void* bytes){
	list->length++;
	if(list->length>list->allocated){
		list->allocated=list->length+30;
		list->data=list->data=realloc(list->data,list->allocated*list->typeSize);
	}
	void* pointer= list->data+list->typeSize*(list->length-1);
	memcpy(pointer,bytes,list->typeSize);
}

#define AEListAdd(list,type,element) ((*((type*)AEListCheck(list)))=(element))
inline static void AEListDelete(AEList* list){
	free(list->data);
	free(list);
}