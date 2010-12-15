#include "AETextBuffer.h"

AETextBufferEdit* AETextBufferEditNew(AETextBufferEdit* lastEdit, const char* text, size_t start, size_t lengthOfDeletion){
	if(text==NULL) text="";
	AETextBufferEdit* self=calloc(1, sizeof(AETextBufferEdit)+strlen(text));
	
	self->lastEdit=lastEdit;
	strcpy(self->text, text);
	self->start=start;
	self->lengthOfDeletion=lengthOfDeletion;
	
	return self;
}

AETextBufferEdit* AETextBufferEditIntersection(AETextBufferEdit* self, AETextBufferEdit* other){
	if(self==NULL) return NULL;
	AETextBufferEdit* originalOther=other;
	while (other) {
		if(other==self) return self;
		other=other->lastEdit;
	}
	return AETextBufferEditIntersection(self->lastEdit, originalOther);
}

void AETextBufferEditDelete(AETextBufferEdit* self){
	if(self==NULL) return;
	AETextBufferEditDelete(self->lastEdit);
	free(self);
}

AETextBuffer* AETextBufferNew(void){
	AETextBuffer* buffer=calloc(1,sizeof(AETextBuffer));
	buffer->text=malloc(1);
	buffer->text[0]=0;
	return buffer;
}

//Sorta ripped this from Enough, worked better than what I had, so it's probably under Open/FreeBSD (he doesn't leave any license info in his code)
void AETextBufferInsert(AETextBuffer* self,size_t offset, char* text){
	size_t	length;

	if(text == NULL)
		return;
	if(text[0]==0) return;
	if(offset > self->length)
		offset = self->length;
	length = strlen(text);
	if(length == 0)
		return;

	if(self->length + length + 1 > self->allocated){
		self->allocated = self->length + length + 15;
		self->text = realloc(self->text, self->allocated);
	}
	memmove(self->text + offset + length, self->text + offset, self->length - offset);
	memcpy(self->text + offset, text, length);
	self->length += length;
	self->text[self->length] = '\0';
}

void AETextBufferBackspace(AETextBuffer* self,size_t offset,size_t length){
	if(offset > self->length)
		return;
	if(offset + length > self->length)
		length = self->length - offset;
	if(length == 0)
		return;
	memmove(self->text + offset, self->text + offset + length, self->length - (offset + length));
	self->length -= length;
	self->text[self->length] = '\0';
	
	//This code shrinks the buffer if you deleted more than a third
	if((self->length/3) < length){
		self->text=realloc(self->text,self->allocated-=(length-1));
		self->text[self->length] = '\0';//Pointless, but doesn't really hurt speed at all
	}
}

void AETextBufferDelete(AETextBuffer* self){
	if(self==NULL) return;
	free(self->text);
	free(self);
}

char* AETextBufferTextGet(AETextBuffer* self){
	return self->text;
}

size_t AETextBufferLengthGet(AETextBuffer* self){
	return self->length;
}

void AETextBufferApplyEditChain(AETextBuffer* self, AETextBufferEdit* edit){
	if(edit==NULL) return;
	AETextBufferApplyEditChain(self, edit->lastEdit);
	AETextBufferBackspace(self, edit->start, edit->lengthOfDeletion);
	AETextBufferInsert(self, edit->start, edit->text);
	
}
