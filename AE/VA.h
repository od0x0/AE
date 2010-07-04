#pragma once
#include "Core.h"
#include "HeaderBegin.h"

//Somewhat based off of pursuade's idea since it pwn3d'd my own

typedef struct{
	unsigned int length;
	union{
		void* data;
		unsigned int vbo;
	}data;
	char isAnIndexArray;
	char vbotype;
}AEVA;
#define AEVAVBOTypeNone 0
#define AEVAVBOTypeStream 1
#define AEVAVBOTypeDynamic 2
#define AEVAVBOTypeStatic 3

AEVA AEVANew(int isAnIndexArray,int vbotype);
void* AEVAMap(AEVA* va, unsigned int length,unsigned int writereadmode);
void AEVAUnmap(AEVA* va);

void AEVAClear(AEVA* va);
void AEVABindVertex(AEVA* va);
void AEVABindTexcoord(AEVA* va);
void AEVABindNormal(AEVA* va);
void AEVABindInterleavedTNV(AEVA* va);
void AEVABindInterleavedTV(AEVA* va);
void AEVABindIndices(AEVA* va);
void AEVADraw(unsigned long start, unsigned long end);

#include "HeaderEnd.h"