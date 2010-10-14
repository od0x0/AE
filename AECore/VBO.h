#pragma once
#include "Core.h"
#include "Math3d.h"
#include "VA.h"
#include "List.h"
#warning VBO.h is near the point of deprecation, VA will probably soon replace it.

typedef struct{
	unsigned int refcount;
	AEVA va;
	AEVA indices;
	char texUnitCount;
	char hasNormals;
	char floatsPerVertex;
	char usesIndices;
}AEVBO;

//extern unsigned char AEVBOAddAllocateFreq;

void AEVBOSerializeToFILE(AEVBO* vbo,FILE* file);
void AEVBOUnserializeFromFILE(AEVBO* vbo,FILE* file);

//Format: "ttttnvi" t for every texcoord, n if it has normals, v if it has positions (required), i if it has indices
//type is "static" "dynamic" "stream" or "none"
AEVBO* AEVBONew(char* format,char* type);
AEVBO* AEVBORetain(AEVBO* vbo);
size_t AEVBOVertexTypeSize(AEVBO* vbo);
//These compile functions delete the lists that are passed into them
void AEVBOCompileVertexList(AEVBO* vbo,AEList* list);
void AEVBOCompileVertexListAndIndexList(AEVBO* vbo,AEList* vlist,AEList* ilist);
//
void AEVBOBind(AEVBO* vbo);
void AEVBODraw(AEVBO* vbo);
void AEVBODelete(AEVBO* vbo);
AEVBO* AEVBOLoad(char* filename,char* format,char* type);

//Helper functions

//result is compounded on the input
void AEVBOCalculateAABB(AEVBO* vbo,AEVec3* min,AEVec3* max);
AEVec3 AEAABBCalculateSize(AEVec3 min,AEVec3 max);
AEVec3 AEAABBCalculateCenter(AEVec3 min,AEVec3 max);
void AEVBOTranslate(AEVBO* vbo,AEVec3 move);