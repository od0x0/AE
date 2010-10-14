#pragma once
#include "Core.h"
#include "VA.h"
#include "Material.h"
#include "Exec.h"

struct A2TerrainCell;
typedef struct A2TerrainCell A2TerrainCell;

struct A2TerrainCell{
	struct{
		AEVec3 min,max;
		//A2TerrainCell* nextCell;
		A2TerrainCell** cells;
		uint64_t cellCount;
		unsigned char hasNormals;
		unsigned char refcount;
		unsigned char unused3;
		unsigned char unused4;
	}info;
	AETexture texture;
	AETexture normalmap;
	//alpha is either depth or gloss
	AEImage* textureImage;
	AEImage* normalmapImage;
	AEVA va;
	AEVA ia;
};

A2TerrainCell* A2TerrainCellNew(void);
void A2TerrainCellDelete(A2TerrainCell* self);
A2TerrainCell* A2TerrainCellRetain(A2TerrainCell* self);
A2TerrainCell* A2TerrainCellChildGet(A2TerrainCell* self,uint64_t i);

void A2TerrainCellLoadVAFromMemory(A2TerrainCell* self, void* _memory, size_t length);
void A2TerrainCellLoadIAFromMemory(A2TerrainCell* self, void* _memory, size_t length);
void A2TerrainCellLoadTextureFromMemory(A2TerrainCell* self, void* _memory, size_t length);
void A2TerrainCellLoadInfoFromMemory(A2TerrainCell* self, void* _memory, size_t length);
void A2TerrainCellRender(A2TerrainCell* self);
