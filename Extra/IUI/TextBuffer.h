#pragma once
#include <string.h>
#include <stdlib.h>

typedef struct{
	char* text;
	size_t length,allocated;
}AETextBuffer;

AETextBuffer* AETextBufferNew(void);
void AETextBufferInsert(AETextBuffer *buffer,size_t offset,char *text);
void AETextBufferBackspace(AETextBuffer *buffer,size_t offset,size_t length);
void AETextBufferDelete(AETextBuffer* buffer);
char* AETextBufferText(AETextBuffer* buffer);
size_t AETextBufferLength(AETextBuffer* buffer);