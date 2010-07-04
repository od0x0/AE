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
	char floatsPerVertex;
	char usesIndices;
}AEVBO;

//extern unsigned char AEVBOAddAllocateFreq;

AEVBO* AEVBONew(char* format,char* type);
size_t AEVBOVertexTypeSize(AEVBO* vbo);
void AEVBOCompileVertexList(AEVBO* vbo,AEList* list);
void AEVBOBind(AEVBO* vbo);
void AEVBODraw(AEVBO* vbo);
void AEVBODelete(AEVBO* vbo);
//Format: "ttnvi"
AEVBO* AEVBOLoad(char* filename,char* format,char* type);

#include "HeaderEnd.h"