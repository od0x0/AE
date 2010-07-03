#pragma once
#include <stdlib.h>

typedef struct{
	size_t length,allocated,typeSize;
	void* data;
}AEList;

inline static AEList* AEListNewWithTypeSize(size_t typeSize){
	AEList* list=calloc(1,sizeof(AEList));
	list->typeSize=typeSize;
	list->length=0;
	list->allocated=0;
	list->data=NULL;
	return list;
}

#define AEListNew(type) AEListNewWithTypeSize(sizeof(type))

#define AEListLength(list) (((AEList*)list)->length)

#define AEListTypeSize(list) (((AEList*)list)->typeSize)

#define AEListLengthInBytes(list) (AEListLength(list)*AEListTypeSize(list))

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
#define AEListAdd(list,type,element) ((*((type*)AEListCheck(list)))=(element))

inline static void AEListDelete(AEList* list){
	free(list->data);
	free(list);
}

inline static void AEListAddBytes(AEList* list,void* bytes){
	list->length++;
	if(list->length>list->allocated){
		list->allocated=list->length+30;
		list->data=list->data=realloc(list->data,list->allocated*list->typeSize);
	}
	void* pointer= list->data+list->typeSize*(list->length-1);
	memcpy(pointer,bytes,list->typeSize);
}


//Private
static inline size_t AEList_private_LinearSearch(void* value,void* array,size_t length,size_t size){
	if(array==NULL || value==NULL) return 0;
	for(size_t i=0;i<length;i++)
		if(memcmp(((char*)array+(i*size)),value,size)==0) return i+1;
	
	return 0;
}

inline static int AEList_private_memeql(char* p,char* p2,size_t size){
	for(size_t i=0;i<size;i++){
		if(p[i]!=p2[i]) return 0;
	}
	return 1;
}

inline static size_t AEListAddBytesUnique(AEList* list,void* data){
	size_t length=AEListLength(list);
	char* listdata=AEListAsArray(list,char);
	
	size_t index=0;//AEList_private_LinearSearch(data,listdata,length,AEListTypeSize(list));
	
	/*for(size_t i=0;i<length;i++){
		//if(memcmp(listdata+i*AEListTypeSize(list),data,AEListTypeSize(list))==0){
		if(AEList_private_memeql(listdata+i*AEListTypeSize(list),data,AEListTypeSize(list))) return i;
	}*/
	
	AEListAddBytes(list,data);
	index=AEListLength(list);
	
	return index-1;
}

inline static AEList* AEListRemoveDuplicatesAndReturnIndices(AEList* list){
	AEList* newlist=AEListNewWithTypeSize(list->typeSize);
	AEList* indices=AEListNew(size_t);
	size_t length=AEListLength(list);
	
	for(size_t i=0;i<length;i++){
		void* data=AEListAsArray(list,void)+AEListTypeSize(list)*i;
		size_t index=AEListAddBytesUnique(newlist,data);
		AEListAdd(indices,size_t,index);
	}
	
	free(list->data);
	*list=*newlist;
	
	return indices;
}