#pragma once
#include "AECore.h"

//Somewhat based off of pursuade's idea since it pwn3d'd my own

//Always interleaved as 2TF* 4CUB? 3NF? 3VF or an array of 32 bit unsigned int indices
typedef struct{
	uint32_t
		indexType :2,//4 possible values
		storageType :2,//4 possible values
		textureCoordsPerVertex :4,//16 possible (0-15)
		hasNormals :1,//2 possible values
		hasColors :1,//2 possible
		hasVertices :1,//yes or no
		isQuads :1,//yes or no
		unused :32-12;
}AEVAFormat;

#define AEVAFormatIndexTypeNone 0
#define AEVAFormatIndexType16Bit 2
#define AEVAFormatIndexType32Bit 3

static inline size_t AEVAFormatByteSize(const AEVAFormat* self){
	size_t size=0;
	size+=self->textureCoordsPerVertex*sizeof(GLfloat[2]);
	if(self->hasNormals) size+=sizeof(GLfloat[3]);
	if(self->hasColors) size+=sizeof(GLubyte[4]);
	if(self->hasVertices) size+=sizeof(GLfloat[3]);
	switch (self->indexType) {
		case AEVAFormatIndexType16Bit:
			size+=sizeof(GLushort);
			break;
		case AEVAFormatIndexType32Bit:
			size+=sizeof(GLuint);
			break;
	}
	return size;
}

//Should be alright to read from, it's pretty much standard now
typedef struct AEVA{
	GLuint elementCount;
	union{
		void* pointer;
		GLuint vbo;
	}data;
	/*unsigned char isAnIndexArray;
	unsigned char vbotype;
	unsigned char tunit;
	unsigned char dataformat;*/
	AEVAFormat format;
}AEVA;
/*
#define AEVADataFormat3VF 0
#define AEVADataFormat3NF3VF 1
#define AEVADataFormat4CUB3VF 2
#define AEVADataFormat4CUB3NF3VF 3
*/
#define AEVAFormatStorageTypeNone 0
#define AEVAFormatStorageTypeStream 1
#define AEVAFormatStorageTypeDynamic 2
#define AEVAFormatStorageTypeStatic 3

void AEVAInit(AEVA* self);
void AEVADeinit(AEVA* self);
void AEVAInitCopy(AEVA* self,AEVA* from);

void* AEVAMap(AEVA* self, unsigned int elementCount,unsigned int writereadmode);
void AEVAUnmap(AEVA* self);

//
//void AEVADraw(const AEVA* va, const AEVA* ia);
void AEVADrawRange(const AEVA* va, const AEVA* ia, GLuint start, GLuint count);
