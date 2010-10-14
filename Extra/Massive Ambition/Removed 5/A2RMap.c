#include "A2RMap.h"

/////////////////////////////////////
////////////Map Cell/////////////////
/////////////////////////////////////

//AXA = AX-is A-ngle
//How do we switch between imposter/foi?

struct A2RMapCell {
	uint64_t ID;
	AEVec3 position;
	AEVec3 size;
	AEVA va;//tv formatted
	AEVA ia;
	AETexture texture;
};

void A2RMapCellInit(A2RMapCell* self){
	memset(self, 0, sizeof(A2RMapCell));
}

void A2RMapCellDeinit(A2RMapCell* self){
	AEVADeinit(& self->va);
	AEVADeinit(& self->ia);
	AETextureDelete(self->texture);
	memset(self, 0, sizeof(A2RMapCell));
}

void A2RMapCellRender(A2RMapCell* self){
	const AEVec3 p=self->position;
	glTranslatef(p.x, p.y, p.z);
	AEVABind(& self->va);
	AEVABind(& self->ia);
	AETextureBind(self->texture);
	AEVADraw(0, self->ia.length);
	glTranslatef(-p.x, -p.y, -p.z);
}

bool A2RMapCellContains(A2RMapCell* self, AEVec3* point){
	const AEVec3 min=AEVec3Sub(self->position, self->size);
	const AEVec3 max=AEVec3Add(self->position, self->size);
	return AEVec3IsBetween(*point, min, max);
}

void A2RMapCellUnserializeFromFILE(A2RMapCell* self,FILE* file){
	
	//Clear the old data
	A2RMapCellDeinit(self);
	//A2RMapCellInit(self);
	
	//Load from file
	fread(& self->ID, 1, sizeof(uint64_t), file);
	self->ID=AEUInt64From8Bytes((uint8_t*)& self->ID, true);
	
	fread(& self->position, 1, sizeof(AEVec3), file);
	fread(& self->size, 1, sizeof(AEVec3), file);
	
	AEVAUnserializeFromFILE(& self->va, file);
	AEVAUnserializeFromFILE(& self->ia, file);
	
	uint64_t dataSize=0;
	fread(&dataSize, 1, sizeof(uint64_t), file);
	dataSize=AEUInt64From8Bytes((uint8_t*)&dataSize, true);
	void* data=malloc(dataSize);
	fread(data, 1, dataSize, file);
	self->texture=AETextureLoadFromMemoryWithFlags(data, dataSize, SOIL_FLAG_COMPRESS_TO_DXT|SOIL_FLAG_MIPMAPS);
	free(data);
}

//Not serialize, because we aren't writing from a cell
void A2RMapCellWriteToFILE(FILE* file, uint64_t ID, AEVec3* position, AEVec3* size, AEVA* va, AEVA* ia, void* memory, uint64_t memorySize){
	uint8_t buffer[16];

	AEUInt64To8Bytes(ID, buffer, true);
	fwrite(buffer, 1, sizeof(uint64_t), file);
	
	fwrite(position, 1, sizeof(AEVec3), file);
	fwrite(size, 1, sizeof(AEVec3), file);
	
	AEVASerializeToFILE(va, file);
	AEVASerializeToFILE(ia, file);
	
	AEUInt64To8Bytes(memorySize, buffer, true);
	fwrite(buffer, 1, sizeof(uint64_t), file);
	fwrite(memory, 1, memorySize, file);
}

/////////////////////////////////////
////////////Map//////////////////////
/////////////////////////////////////
struct A2RMap {
	size_t refcount;
	uint64_t loadedCellCount;
	uint64_t adjacentCellCount;
	A2RMapCell* mainCell;
	A2RMapCell* loadedCells;
	AEVec3 lastCameraPosition;
};

A2RMap* A2RMapRetain(A2RMap* self){
	if(self) self->refcount++;
	return self;
}

A2RMap* A2RMapNew(void){
	A2RMap* self=calloc(1, sizeof(A2RMap));
	return A2RMapRetain(self);
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
		A2RMapCellRender(self->loadedCells+i);
}

void A2RMapLoadCell(A2RMap* self, uint64_t cellID){
	FILE* file=NULL;
	char filename[1024];
	
	//Delete old cells
	for (uint64_t i=0; i<self->loadedCellCount; i++) {
		A2RMapCell* cell=self->loadedCells+i;
		A2RMapCellDeinit(cell);
	}
	
	self->loadedCellCount=0;
	free(self->loadedCells);
	self->loadedCells=NULL;
	
	//Load child list
	uint64_t cellCount=0;
	uint64_t adjacentCellCount=0;
	A2RMapCell* cells=NULL;
	
	sprintf(filename, "%lu-h.a2rmc", (unsigned long)cellID);
	file=fopen(filename, "rb");
	{
		//The children info is the first half of the -h
		fread(&cellCount, 1, sizeof(uint64_t), file);
		cellCount=AEUInt64From8Bytes((uint8_t*)&cellCount, true);
		fread(&adjacentCellCount, 1, sizeof(uint64_t), file);
		adjacentCellCount=AEUInt64From8Bytes((uint8_t*)&adjacentCellCount, true);
		//It's the first half because of this line's position
		cells=calloc(cellCount, sizeof(A2RMapCell));
		
		//Now load the main cell data, same layout as the -l
		A2RMapCellUnserializeFromFILE(cells+0, file);
		
		//Load the children
		for (uint64_t i=1; i<cellCount; i++) {
			//Get the ID from the file
			uint8_t buffer[16];
			fread(buffer, 1, sizeof(uint64_t), file);
			uint64_t ID=AEUInt64From8Bytes(buffer, true);
			
			//Load the child cell, the -l version
			A2RMapCell* cell=cells+i;
			sprintf(filename, "%lu-%c.a2rmc", (unsigned long)ID, (adjacentCellCount > i) ? 'm' : 'l');
			//Watch out, I'm shadowing the upperlevel "file", applies to the end of the for loop
			FILE* file=fopen(filename, "rb");
			if (not file) continue;
			
			A2RMapCellUnserializeFromFILE(cell, file);
			
			fclose(file);
			file=NULL;
		}
		
		self->mainCell=cells+0;
		self->loadedCells=cells;
		self->loadedCellCount=cellCount+1;
		self->adjacentCellCount=adjacentCellCount;
	}
	fclose(file);
	file=NULL;
}

void A2RMapCameraPositionSet(A2RMap* self, AEVec3* v3){
	if(v3->x==self->lastCameraPosition.x and v3->y==self->lastCameraPosition.y and v3->z==self->lastCameraPosition.z) return;
	
	if (A2RMapCellContains(self->mainCell, v3)) return;
	for (uint64_t i=0; i<self->adjacentCellCount; i++) {
		A2RMapCell* cell=self->loadedCells+i;
		if(not cell) continue;
		if (A2RMapCellContains(cell, v3))
			A2RMapLoadCell(self, cell->ID);
	}
	
	self->lastCameraPosition=*v3;
}
