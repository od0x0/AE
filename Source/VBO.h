#pragma once
#include "Core.h"
#include "HeaderBegin.h"
#include "FastMath3d.h"
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


//Helper functions
//result is compounded on the input
static inline void AEVBOCalculateAABB(AEVBO* vbo,AEVec3* min,AEVec3* max){
	int skip=AEVBOVertexTypeSize(vbo);
	unsigned int vcount=vbo->va.length/vbo->floatsPerVertex;
	int offset=vbo->texUnitCount*2+vbo->hasNormals*3;
	void* data=AEVAMap(&(vbo->va),vbo->va.length/vbo->floatsPerVertex,GL_READ_ONLY);
	for(unsigned int i=0;i<vcount;i++){
		AEVec3* v=data+offset*sizeof(float)+skip*i;
		*min=AEVec3Min(*min,*v);
		*max=AEVec3Max(*max,*v);
	}
	AEVAUnmap(&(vbo->va));
}

static inline AEVec3 AEAABBCalculateSize(AEVec3 min,AEVec3 max){
	AEVec3 size=AEVec3Sub(max,min);//size is the difference of the two
	return AEVec3Sub(size,AEVec3Mul(AEVec3FromSingle(0.5),size));//Center it
}

static inline AEVec3 AEAABBCalculateCenter(AEVec3 min,AEVec3 max){
	return AEVec3Mul(AEVec3Add(max,min),AEVec3FromSingle(0.5));//The average
}

static inline void AEVBOTranslate(AEVBO* vbo,AEVec3 move){
	int skip=AEVBOVertexTypeSize(vbo);
	unsigned int vcount=vbo->va.length/vbo->floatsPerVertex;
	int offset=vbo->texUnitCount*2+vbo->hasNormals*3;
	void* data=AEVAMap(&(vbo->va),vbo->va.length/vbo->floatsPerVertex,GL_READ_ONLY);
	for(unsigned int i=0;i<vcount;i++){
		AEVec3* v=data+offset*sizeof(float)+skip*i;
		*v=AEVec3Add(*v,move);
	}
	AEVAUnmap(&(vbo->va));
}

#include "HeaderEnd.h"