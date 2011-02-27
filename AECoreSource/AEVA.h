#pragma once
#include "AECore.h"

//Somewhat based off of pursuade's idea since it pwn3d'd my own

//Always interleaved as 4CUB 2TF* 3NF 3VF
typedef struct{
	uint32_t//since this is guarranteed to be 32 bits.
		isAnIndexArray :1,//2 possible values
		storageType :2,//4 possible values
		textureCoordsPerVertex :4,//16 possible
		hasNormals :1,//2 possible values
		hasColors :1,//2 possible
		hasVertices :1,
		unused :32-10;
}AEVAFormat;

static inline uint64_t AEVAFormatTo64Bits(AEVAFormat* self){
	uint64_t bits=0;
	bits |= self->isAnIndexArray << 0;
	bits |= self->storageType << 1;
	bits |= self->textureCoordsPerVertex << 1+2;
	bits |= self->hasNormals << 1+2+4;
	bits |= self->hasColors << 1+2+4+1;
	bits |= self->hasVertices << 1+2+4+1+1;
	return bits;
}

static inline void AEVAFormatFrom64Bits(AEVAFormat* self, uint64_t bits){
	self->isAnIndexArray = (bits >> 0) & 1;
	self->storageType = (bits >> 1) & 2;
	self->textureCoordsPerVertex = (bits >> 1+2) & 4;
	self->hasNormals = (bits >> 1+2+4) & 1;
	self->hasColors = (bits >> 1+2+4+1) & 1;
	self->hasVertices = (bits >> 1+2+4+1+1) & 1;
}

//Should be alright to read from, it's pretty much standard now
typedef struct AEVA{
	GLuint length;
	union{
		void* data;
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
#define AEVAVBOTypeNone 0
#define AEVAVBOTypeStream 1
#define AEVAVBOTypeDynamic 2
#define AEVAVBOTypeStatic 3

size_t AEVABytesPerVertex(AEVA* self);

void AEVAInit(AEVA* self);
void AEVADeinit(AEVA* self);
void AEVAInitCopy(AEVA* self,AEVA* from);

void* AEVAMap(AEVA* self, unsigned int length,unsigned int writereadmode);
void AEVAUnmap(AEVA* self);

void AEVADraw(AEVA* va, AEVA* ia);
void AEVADrawRange(AEVA* va, AEVA* ia, unsigned long start, unsigned long end);
