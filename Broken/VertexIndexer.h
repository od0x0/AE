#pragma once
#include <stdlib.h>
#include <string.h>
#include "RawMesh.h"

typedef struct{
	unsigned int vcount,icount,vallocated,iallocated,*indices;
	float* verts;
	char hasNormals,texCoordCount,elementSize;
}AEVertexIndexerData;

// This code could have been cleaner, particularly with an array "class"

extern unsigned char AEVertexIndexerDataAddAllocateFreq;

void AEVertexIndexerDataAdd(AEVertexIndexerData* vid,float* v);

AEVertexIndexerData* AEVertexIndexerDataLoad(const char* filename,int texCoordCount,int hasNormals);

void AEVertexIndexerDataDelete(AEVertexIndexerData* vid);