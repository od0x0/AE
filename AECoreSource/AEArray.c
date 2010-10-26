#include "AEArray.h"
void AEArrayDeinit(void* array){
	if(array==NULL) return;
	AEArray(void)* self=array;
	free(self->data);
}
void AEArrayResize(void* array,size_t length){
	AEArray(void)* self=array;
	self->length=length;
	if(self->data && length > self->allocated){
		self->allocated=self->length * 1.2;
		self->data=realloc(self->data, self->allocated*self->typeSize);
	}
	if((self->data==NULL) || ((self->allocated*0.75) > length)){
		self->allocated=self->length;
		self->data=realloc(self->data, self->allocated*self->typeSize);
	}
}
void* AEArrayCheck(void* array){
	AEArray(void)* self=array;
	AEArrayResize(array,self->length+1);
	return self->data+(self->length-1)*self->typeSize;
}
void AEArraySweep(void* array){
	AEArrayResize(array,0);
}
void AEArrayAddBytes(void* array,void* bytes){
	AEArray(void)* self=array;
	AEArrayCheck(array);
	memcpy(self->data+self->typeSize*(self->length-1), bytes, self->typeSize);
}
size_t AEArrayFindIndexOfBytes(void* array,void* bytes){
	AEArray(void)* self=array;
	for(size_t i=0; i<self->length; i++){
		if(memcmp(self->data+self->typeSize*i, bytes, self->typeSize)==0) return i+1;
	}
	return 0;
}
size_t AEArrayAddBytesUnique(void* array,void* data){
	AEArray(void)* self=array;
	size_t index=AEArrayFindIndexOfBytes(array,data);	
	if(index==0){
		AEArrayAddBytes(array,data);
		index=self->length;
	}
	return index-1;
}
void AEArrayRemoveDuplicates(void* array,void* _outarray,void* _indices){
	AEArray(void)* self=array;
	AEArray(void)* outarray=_outarray;
	AEArray(size_t)* indices=_indices;
	
	for(size_t i=0; i<self->length; i++){
		void* data=self->data+i*self->typeSize;
		size_t index=AEArrayAddBytesUnique(outarray, data);
		if(indices) AEArrayAdd(indices, index);
	}
}
void AEArrayRemoveBytes(void* array,void* bytes){
	size_t index=AEArrayFindIndexOfBytes(array,bytes);
	if(index==0) return;
	
	AEArray(void)* self=array;
	index--;
	void* removed=index*self->typeSize+self->data;
	void* last=(self->length-1)*self->typeSize+self->data;
	memcpy(removed,last,self->typeSize);
	self->length--;
	
	if(self->allocated*0.75 > self->length) AEArrayResize(array,self->length);
}