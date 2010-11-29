#pragma once
#include "AECore.h"

//Somewhat based off of pursuade's idea since it pwn3d'd my own
/*
typedef struct{
	unsigned char isAnIndexArray: 1; //2 possible values
	unsigned char vbotype: 2;//4 possible values
	unsigned char tunit:4;//256 possible
	unsigned char hasNormals: 1;//2 possible values
	unsigned char hasColor: 1;//2 possible
}AEVAFormat;
*/
//Should be alright to read from, it's pretty much standard now
typedef struct{
	unsigned int length;
	union{
		void* data;
		GLuint vbo;
	}data;
	unsigned char isAnIndexArray;
	unsigned char vbotype;
	unsigned char tunit;
	unsigned char dataformat;
}AEVA;

#define AEVADataFormat3VF 0
#define AEVADataFormat3NF3VF 1
#define AEVADataFormat4CUB3VF 2
#define AEVADataFormat4CUB3NF3VF 3

#define AEVAVBOTypeNone 0
#define AEVAVBOTypeStream 1
#define AEVAVBOTypeDynamic 2
#define AEVAVBOTypeStatic 3


size_t AEVABytesPerVertex(AEVA* va);

void AEVAIterationBegin(AEVA* va);
void AEVAIterationEnd(AEVA* va);

AEVec3* AEVAIteratorVertexNext(AEVA* va, AEVec3* old);
AEVec3* AEVAIteratorNormalNext(AEVA* va, AEVec3* old);
AEVec2* AEVAIteratorTexCoordNext(AEVA* va, int unit, AEVec2* old);
GLuint* AEVAIteratorIndexNext(AEVA* va, GLuint* old);

void AEVAInit(AEVA* va,int isAnIndexArray,int vbotype,int tuint, int dataformat);
void AEVADeinit(AEVA* va);
void AEVAInitCopy(AEVA* vato,AEVA* vafrom);

void* AEVAMap(AEVA* va, unsigned int length,unsigned int writereadmode);
void AEVAUnmap(AEVA* va);

void AEVABindVertex(AEVA* va);
void AEVABindTexcoord(AEVA* va);
void AEVABindNormal(AEVA* va);
void AEVABindInterleaved(AEVA* va,int tcount,bool hasNormals);
void AEVABindInterleavedTNV(AEVA* va);
void AEVABindInterleavedTV(AEVA* va);
void AEVABindIndices(AEVA* va);
void AEVABind(AEVA* va);

void AEVADraw(unsigned long start, unsigned long end);

//Deprecated, or may just wrap the one below
void AEVASerializeToFILE(AEVA* va,FILE* file);
void AEVAUnserializeFromFILE(AEVA* va,FILE* file);
//The new and improved version
void AEVASerializeToMBuffer(AEVA* va,AEMBuffer* mbuffer);
void AEVAUnserializeFromMBuffer(AEVA* va,AEMBuffer* mbuffer);

void AEVALoadFromObj(AEVA* va, AEVA* ia, const char* objfilename);