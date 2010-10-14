#include "A2RMap.h"

/////////////////////////////////////
////////////Map Cell/////////////////
/////////////////////////////////////

//AXA = AX-is A-ngle
//How do we switch between imposter/foi?

struct A2RMapCell {
	size_t refcount;
	
	uint64_t ID;
	
	AEVec3 position;
	
	AEVec3 size;

	struct{
		AEVA va;//tv formatted
		AEVA ia;
		AETexture texture;
	}main;
	
	uint32_t* children;
	uint64_t childCount;
};

A2RMapCell* A2RMapCellRetain(A2RMapCell* self){
	if(self) self->refcount++;
	return self;
}

A2RMapCell* A2RMapCellNew(void){
	A2RMapCell* self = calloc(1, sizeof(A2RMapCell));
	self = A2RMapCellRetain(self);
	return self;
}

void A2RMapCellDelete(A2RMapCell* self){
	if(not self) return;
	self->refcount--;
	if(self->refcount) return;
	
	AEVADeinit(& self->main.va);
	AEVADeinit(& self->main.ia);
	AETextureDelete(self->main.texture);
	free(self->children);
	
	free(self);
}

void A2RMapCellRender(A2RMapCell* self){
	const AEVec3 p=self->position;
	glTranslatef(p.x, p.y, p.z);
	AEVABind(& self->main.va);
	AEVABind(& self->main.ia);
	AETextureBind(self->main.texture);
	glTranslatef(-p.x, -p.y, -p.z);
}

uint32_t* A2RMapCellChildrenGet(A2RMapCell* self){
	return self->children;
}

uint64_t A2RMapCellChildCountGet(A2RMapCell* self){
	return self->childCount;
}

uint64_t A2RMapCellIDGet(A2RMapCell* self){
	return self->ID;
}

AEVec3* A2RMapCellPositionGet(A2RMapCell* self){
	return & self->position;
}

bool A2RMapCellContains(A2RMapCell* self, AEVec3* point){
	const AEVec3 min=AEVec3Sub(self->position, self->size);
	const AEVec3 max=AEVec3Add(self->position, self->size);
	return AEVec3IsBetween(*point, min, max);
}

AEVec3* A2RMapCellSizeGet(A2RMapCell* self){
	return & self->size;
}

void A2RMapCellSerializeToFILE(A2RMapCell* self,FILE* file){
	
}

void A2RMapCellUnserializeFromFILE(A2RMapCell* self,FILE* file, void(*childcellloader)(uint32_t cellID, void* arg), void* arg){
	
	//Clear the old data
	AEVADeinit(& self->main.va);
	
	AEVADeinit(& self->main.ia);
	
	AETextureDelete(self->main.texture);
	self->main.texture=0;
	
	free(self->children);
	self->children=NULL;
	self->childCount=0;
	
	//Load from file
	fread(& self->ID, 1, sizeof(uint64_t), file);
	self->ID=AEUInt64From8Bytes((uint8_t*)& self->ID, true);
	
	fread(& self->position, 1, sizeof(AEVec3), file);
	fread(& self->size, 1, sizeof(AEVec3), file);
	
	AEVAUnserializeFromFILE(& self->main.va, file);
	
	AEVAUnserializeFromFILE(& self->main.ia, file);
	
	uint64_t dataSize=0;
	fread(&dataSize, 1, sizeof(uint64_t), file);
	dataSize=AEUInt64From8Bytes((uint8_t*)&dataSize, true);
	void* data=malloc(dataSize);
	fread(data, 1, dataSize, file);
	self->main.texture=AETextureLoadFromMemoryWithFlags(data, dataSize, SOIL_FLAG_COMPRESS_TO_DXT|SOIL_FLAG_MIPMAPS);
	free(data);
	
	//Load the children if main, skip otherwise
	
	if (childcellloader) {
		uint64_t childCount=0;
		fread(&childCount, 1, sizeof(uint64_t), file);
		self->childCount=AEUInt64From8Bytes((uint8_t*)&childCount, true);
		
		self->children=calloc(self->childCount, sizeof(uint32_t));
		fread(self->children, self->childCount, sizeof(uint32_t), file);
		for (uint64_t i=0; i<self->childCount; i++) {
			self->children[i]=AEUInt32From4Bytes((uint8_t*)self->children+i, true);
			childcellloader(self->children[i], arg);
		}
	}
}

/////////////////////////////////////
////////////Map//////////////////////
/////////////////////////////////////
struct A2RMap {
	size_t refcount;
	uint64_t loadedCellCount, loadedCellAllocated;
	A2RMapCell* mainCell;
	A2RMapCell* *loadedCells;
};

A2RMap* A2RMapNew(void){
	A2RMap* self=calloc(1, sizeof(A2RMap));
	return A2RMapRetain(self);
}

A2RMap* A2RMapRetain(A2RMap* self){
	if(self) self->refcount++;
	return self;
}

void A2RMapDelete(A2RMap* self){
	if(not self) return;
	self->refcount--;
	if(self->refcount) return;
	
	free(self->loadedCells);
	free(self);
}

void A2RMapRender(A2RMap* self){
	for (uint64_t i=0; i<self->loadedCellCount; i++)
		A2RMapCellRender(self->loadedCells[i]);
}

void A2RMapStep(A2RMap* self, float stepInSeconds, void(*loaderfunc)(uint64_t ID, void* arg), void* arg){
	
	AEVec3 cameraPosition;
	AECameraPositionGet(AECameraActiveGet(), &cameraPosition);
	
	if(A2RMapCellContains(self->mainCell, &cameraPosition)) return;//Nothing needs to happen
	
	for (uint64_t i=0; i<self->loadedCellCount; i++) {
		A2RMapCell* cell=self->loadedCells[i];
		if(A2RMapCellContains(cell, &cameraPosition)){
			loaderfunc(cell->ID, arg);
			for (uint64_t i=0; i<self->loadedCellCount; i++)
				A2RMapCellDelete(self->loadedCells+i);
			break;
		}
	}
}