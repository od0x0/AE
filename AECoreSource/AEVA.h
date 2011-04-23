#pragma once
#include "AECore.h"

//Somewhat based off of pursuade's idea since it pwn3d'd my own

//Always interleaved as 2TF* 4CUB? 3NF? 3VF or an array of 32 bit unsigned int indices
typedef struct{
	uint32_t//since this is guarranteed to be 32 bits.
		isAnIndexArray :1,//2 possible values
		storageType :2,//4 possible values
		textureCoordsPerVertex :4,//16 possible (0-15)
		hasNormals :1,//2 possible values
		hasColors :1,//2 possible
		hasVertices :1,//yes or no
		unused :32-10;
}AEVAFormat;

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
#define AEVAFormatStorageTypeNone 0
#define AEVAFormatStorageTypeStream 1
#define AEVAFormatStorageTypeDynamic 2
#define AEVAFormatStorageTypeStatic 3

size_t AEVABytesPerVertex(AEVA* self);

void AEVAInit(AEVA* self);
void AEVADeinit(AEVA* self);
void AEVAInitCopy(AEVA* self,AEVA* from);

void* AEVAMap(AEVA* self, unsigned int length,unsigned int writereadmode);
void AEVAUnmap(AEVA* self);

void AEVADraw(AEVA* va, AEVA* ia);
void AEVADrawRange(AEVA* va, AEVA* ia, unsigned long start, unsigned long end);
