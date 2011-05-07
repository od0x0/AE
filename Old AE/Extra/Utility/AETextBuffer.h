#pragma once
#include <string.h>
#include <stdlib.h>


//History tracking was added later, so this isn't as clean as it could be.
typedef struct AETextBufferEdit AETextBufferEdit;
struct AETextBufferEdit{
	AETextBufferEdit* lastEdit;
	size_t start, lengthOfDeletion;//Length of deletion
	char text[1];//Variable length array hack (for C++ compatibility) It also gives us a benefit of having the nullbyte already allocated.
};

AETextBufferEdit* AETextBufferEditNew(AETextBufferEdit* lastEdit, const char* text, size_t start, size_t lengthOfDeletion);
AETextBufferEdit* AETextBufferEditIntersection(AETextBufferEdit* self, AETextBufferEdit* other);
void AETextBufferEditDelete(AETextBufferEdit* self);

typedef struct{
	char* text;
	size_t length,allocated;
}AETextBuffer;

AETextBuffer* AETextBufferNew(void);
void AETextBufferInsert(AETextBuffer *self,size_t offset,const char *text);
void AETextBufferBackspace(AETextBuffer *self,size_t offset,size_t length);
void AETextBufferDelete(AETextBuffer* self);
char* AETextBufferGetText(AETextBuffer* self);
size_t AETextBufferGetLength(AETextBuffer* self);
void AETextBufferApplyEditChain(AETextBuffer* self, AETextBufferEdit* edit);
