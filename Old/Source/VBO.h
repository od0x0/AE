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
	char texUnitCount;
	char hasNormals;
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
void AEVBOCalculateAABB(AEVBO* vbo,AEVec3* min,AEVec3* max);
AEVec3 AEAABBCalculateSize(AEVec3 min,AEVec3 max);
AEVec3 AEAABBCalculateCenter(AEVec3 min,AEVec3 max);
void AEVBOTranslate(AEVBO* vbo,AEVec3 move);

/*
typedef struct{//Units are in bytes
	short skip,size;
	size_t currentLocation,length;
	void* data;
	AEVBO* vbo;
	AEVA* va;
}AEVBOIterator;

//component 0 = indices
static inline AEVBOIterator AEVBOIteratorStart(AEVBO* vbo,int component){
	short size=0;
	short skip=0;
	size_t length=0;
	void* data=NULL;
	AEVA* va=NULL;
	
	if(component >= 0){//If positive, then we're acessing components
		va=&(vbo->va.va);
		component--;
		//if(component < vbo->texUnitCount) skip+= vbo->texUnitCount - component;
		//if((component-vbo->texUnitCount) < vbo->hasNormals) skip += vbo->hasNormals;
		size=sizeof(float);
		length=vbo->va.length*size;
		data=AEVAMap(va,length,GL_READ_WRITE);
	}
	else{//Otherwise, We're accessing the index array
		if(vbo->usesIndices==0) AEError("Trying to get vbo index iterator for a vbo with no indices. O.o\n");
		va=&(vbo->indices.va);
		skip=0;
		size=sizeof(unsigned int);
		length=vbo->indices.length*size;
		data=AEVAMap(va,length,GL_READ_WRITE);
	}
	
	AEVBOIterator iterator={skip,size,0,length,data,vbo,va};
}

static inline void* AEVBOIteratorNext(AEVBOIterator* iterator){
	
}

static inline void AEVBOIteratorEnd(AEVBOIterator* iterator){
	
}
*/
#include "HeaderEnd.h"