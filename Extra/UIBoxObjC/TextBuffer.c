#include "TextBuffer.h"

AETextBuffer* AETextBufferNew(void){
	AETextBuffer* buffer=calloc(1,sizeof(AETextBuffer));
	buffer->text=malloc(1);
	buffer->text[0]=0;
	return buffer;
}

//Sorta ripped this from Enough, worked better that what I had, so it's probably under Open/FreeBSD (he doesn't leave any license info in his code)
void AETextBufferInsert(AETextBuffer *buffer,size_t offset,char *text){
	size_t	length;

	if(text == NULL)
		return;
	if(offset > buffer->length)
		offset = buffer->length;
	length = strlen(text);
	if(length == 0)
		return;

	if(buffer->length + length + 1 > buffer->allocated){
		buffer->allocated = buffer->length + length + 15;
		buffer->text = realloc(buffer->text, buffer->allocated);
	}
	memmove(buffer->text + offset + length, buffer->text + offset, buffer->length - offset);
	memcpy(buffer->text + offset, text, length);
	buffer->length += length;
	buffer->text[buffer->length] = '\0';
}

void AETextBufferBackspace(AETextBuffer *buffer,size_t offset,size_t length){
	if(offset > buffer->length)
		return;
	if(offset + length > buffer->length)
		length = buffer->length - offset;
	if(length == 0)
		return;
	memmove(buffer->text + offset, buffer->text + offset + length, buffer->length - (offset + length));
	buffer->length -= length;
	buffer->text[buffer->length] = '\0';
	
	//This code shrinks the buffer if you deleted more than a third
	if((buffer->length/3) < length){
		buffer->text=realloc(buffer->text,buffer->allocated-=(length-1));
		buffer->text[buffer->length] = '\0';//Pointless, but doesn't really hurt speed at all
	}
}

void AETextBufferDelete(AETextBuffer* buffer){
	free(buffer->text);
	free(buffer);
}

char* AETextBufferText(AETextBuffer* buffer){
	return buffer->text;
}

size_t AETextBufferLength(AETextBuffer* buffer){
	return buffer->length;
}