#pragma once
#include "Core.h"
#include "HeaderBegin.h"
#include "VA.h"
#include "List.h"
#include "RawMesh.h"

#include <stdlib.h>

typedef struct{
	AEVA va;
	AEVA indices;
	char hasNormals;
	char texUnitCount;
	char elementSize;
	char usesIndices;
}AEVBO;

//extern unsigned char AEVBOAddAllocateFreq;

AEVBO* AEVBONew(char hasNormals,char tcount,char usesIndices,char* type);
size_t AEVBOVertexTypeSize(AEVBO* vbo);
void AEVBOCompileVertexList(AEVBO* vbo,AEList* list);
void AEVBOBind(AEVBO* vbo);
void AEVBODraw(AEVBO* vbo);
void AEVBODelete(AEVBO* vbo);
AEVBO* AEVBOLoad(char* filename,char hasNormals,char tcount,char usesIndices,char* type);

#include "HeaderEnd.h"