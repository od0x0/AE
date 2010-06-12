#pragma once
#include "Core.h"
#include "HeaderBegin.h"

typedef struct{//YadaYadaYada Packing
	AEVec2 t;
	AEVec3 v;
}AEVBOVert;

typedef struct{//YadaYadaYada Packing
	AEVec2 t;
	AEVec3 n,v;
}AEVBOVertWithNormal;

typedef struct{
	unsigned int vbo,nbo,ibo,vcount,icount,vallocated,iallocated,*indices;
	AEVec3* n;
	AEVBOVert* verts;
	char hasNormals;
}AEVBO;

extern unsigned char AEVBOAddAllocateFreq;

AEVBO* AEVBOLoad(const char* filename,int isStatic,int hasNormals);
void AEVBOAdd(AEVBO* vbo,AEVBOVertWithNormal* v);
void AEVBODraw(AEVBO* vbo);
void AEVBOCompile(AEVBO* vbo,unsigned int* usages);
void AEVBODelete(AEVBO* vbo);

#include "HeaderEnd.h"