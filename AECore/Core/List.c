#include "../List.h"

//Trust me, this all works

AEList* AEListNewWithTypeSize(size_t typeSize){
	AEList* list=calloc(1,sizeof(AEList));
	list->typeSize=typeSize;
	list->length=0;
	list->allocated=0;
	list->data=NULL;
	return AEListRetain(list);
}

AEList* AEListRetain(AEList* list){
	if(list) list->refcount++;
	return list;
}

AEList* AEListCopy(AEList* list){
	if(list==NULL) return NULL;
	AEList* newlist=AEListNewWithTypeSize(list->typeSize);
	newlist->data=malloc(list->typeSize*AEListLength(list));
	memcpy(newlist->data,AEListAsArray(list,void),list->typeSize*AEListLength(list));
	return newlist;
}

void AEListResize(AEList* list,size_t length){
	if(list==NULL) return;
	list->allocated=length;
	if(list->allocated<list->length) list->length=length;
	list->data=realloc(list->data,list->allocated*list->typeSize);
}

void* AEListCheck(AEList* list){
	if(list==NULL) return NULL;
	list->length++;
	if(list->length>list->allocated){
		//list->length*=(list->length > 5000 ? 1.2 : 2) +1;
		list->allocated=list->length + 3;
		//list->allocated*=(list->length > 5000 ? 1.2 : 2) +1;
		int scalefactor = 2;
		if(list->length<200) scalefactor=4;
		if(list->length<3000) scalefactor=1.5;
		if(list->length<5000) scalefactor=1;
		list->allocated=list->length * scalefactor + 1;
		list->data=list->data=realloc(list->data,list->allocated*list->typeSize);
	}
	return list->data+list->typeSize*(list->length-1);
}

void AEListSweep(AEList* list){
	if(list==NULL) return;
	AEListResize(list,list->length);
}

void AEListDelete(AEList* list){
	if(list==NULL) return;
	list->refcount--;
	if(list->refcount) return;
	free(list->data);
	free(list);
}

void AEListAddBytes(AEList* list,void* bytes){
	list->length++;
	if(list->length>list->allocated){
		list->allocated=list->length+30;
		list->data=list->data=realloc(list->data,list->allocated*list->typeSize);
	}
	void* pointer=list->data+list->typeSize*(list->length-1);
	memcpy(pointer,bytes,list->typeSize);
}

size_t AEList_private_LinearSearch(void* value,void* array,size_t length,size_t size){
	if(array==NULL || value==NULL) return 0;
	for(size_t i=0;i<length;i++)
		if(memcmp(((char*)array+(i*size)),value,size)==0) return i+1;
	
	return 0;
}

int AEList_private_memeql(char* p,char* p2,size_t size){
	for(size_t i=0;i<size;i++){
		if(p[i]!=p2[i]) return 0;
	}
	return 1;
}

size_t AEListAddBytesUnique(AEList* list,void* data){
	size_t length=AEListLength(list);
	char* listdata=AEListAsArray(list,char);
	
	size_t index=0;//AEList_private_LinearSearch(data,listdata,length,AEListTypeSize(list));
	
	for(size_t i=0;i<length;i++){
		if(memcmp(listdata+i*AEListTypeSize(list),data,AEListTypeSize(list))==0) return i;
		//if(AEList_private_memeql(listdata+i*AEListTypeSize(list),data,AEListTypeSize(list))) return i;
	}
	
	AEListAddBytes(list,data);
	index=AEListLength(list);
	
	return index-1;
}

size_t AEListFindIndexOfBytes(AEList* list,void* bytes){
	size_t length=AEListLength(list);
	char* listdata=AEListAsArray(list,char);
	
	//size_t index=0;//AEList_private_LinearSearch(data,listdata,length,AEListTypeSize(list));
	
	for(size_t i=0;i<length;i++){
		if(memcmp(listdata+i*AEListTypeSize(list),bytes,AEListTypeSize(list))==0) return i+1;
	}
	
	return 0;
}

/*void AEListRemoveDuplicates(AEList* list,AEList* indices){
	AEList* newlist=AEListNewWithTypeSize(list->typeSize);
	//AEList* indices=AEListNew(size_t);
	size_t length=AEListLength(list);
	
	for(size_t i=0;i<length;i++){
		void* data=AEListAsArray(list,void)+AEListTypeSize(list)*i;
		size_t index=AEListAddBytesUnique(newlist,data);
		//AEListAddBytes(indices,&index);
		if(indices) AEListAdd(indices,size_t,index);
	}
	
	free(list->data);
	*list=*newlist;
	
	//return indices;
}*/

void AEListRemoveDuplicates(AEList* list,AEList* outlist,AEList* outindices){
	//AEList* newlist=AEListNewWithTypeSize(list->typeSize);
	//AEList* indices=AEListNew(size_t);
	size_t length=AEListLength(list);
	
	if(AEListTypeSize(list) != AEListTypeSize(outlist))
		printf("%s(): outlist's typesize is not the same as list's\n",__func__);
	if(AEListTypeSize(outindices) != sizeof(size_t))
		printf("%s(): outindices's typesize is not sizeof(size_t)\n",__func__ );
	
	for(size_t i=0;i<length;i++){
		void* data=AEListAsArray(list,void)+AEListTypeSize(list)*i;
		size_t index=AEListAddBytesUnique(outlist,data);
		//AEListAddBytes(indices,&index);
		if(outindices) AEListAdd(outindices,size_t,index);
	}
	
	//free(list->data);
	//*list=*newlist;
	
	//return indices;
}

void AEListRemoveBytes(AEList* list,void* bytes){
	size_t index=AEListFindIndexOfBytes(list,bytes);
	if(index==0) return;
	
	index--;
	void* removed=index*AEListTypeSize(list)+list->data;
	void* last=(AEListLength(list)-1)*AEListTypeSize(list)+list->data;
	memcpy(removed,last,AEListTypeSize(list));
	list->length--;
	
	if(list->allocated*0.75 > list->length) AEListResize(list,list->length+1);
}