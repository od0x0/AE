#pragma once
#include "Core.h"
#include "HeaderBegin.h"
#include "VA.h"
#include "List.h"
#include "RawMesh.h"

extern unsigned char AEVBOAddAllocateFreq;

#pragma pack(push, 1)
/*
typedef struct{//YadaYadaYada Packing
	AEVec2 t;
	AEVec3 v;
}AEVBOVert;

typedef struct{//YadaYadaYada Packing
	AEVec2 t;
	AEVec3 n,v;
}AEVBOVertWithNormal;
*/
#pragma pack(pop)

#include <stdlib.h>

typedef struct{
	AEVA va;
	AEVA indices;
	float* vdata;
	size_t vcount;
	size_t vallocated;
	AEList* ilist;
	char hasNormals;
	char texUnitCount;
	char elementSize;
	char usesIndices;
}AEVBO;

//extern unsigned char AEVBOAddAllocateFreq;

AEVBO* AEVBONew(char hasNormals,char tcount,char usesIndices,char* type);
void AEVBOAdd(AEVBO* vbo,void* v);
void AEVBOBind(AEVBO* vbo);
void AEVBODraw(AEVBO* vbo);
void AEVBOCompile(AEVBO* vbo);
void AEVBODelete(AEVBO* vbo);

void AEVBOLoadInto(AEVBO* vbo,char* filename);

inline AEVBO* AEVBOLoad(char* filename,char hasNormals,char tcount,char usesIndices,char* type){
	AEVBO* vbo=AEVBONew(hasNormals, tcount, usesIndices, type);
	AEVBOLoadInto(vbo,filename);
	return vbo;
}

#include "HeaderEnd.h"