#include "A2Terrain.h"

A2Terrain* A2TerrainNew(void){
	A2Terrain* self=calloc(1,sizeof(A2Terrain));
	self->execQueue=AEExecQueueNew();
	self->cellList=AEListNew(A2TerrainCell);
	return self;
}

void A2TerrainDelete(A2Terrain* self){
	if(not self) return;
	AEExecQueueDelete(self->execQueue);
	//A2TerrainCell* cells=AEListAsArray(self->cellList,A2TerrainCell);
	//size_t cellCount=AEListLength(self->cellList);
	//for (size_t i=0; i<cellCount; i++) A2TerrainCellDeinit(cells+i);
	AEListDelete(self->cellList);
	free(self);
}
/*
void A2TerrainRender(A2Terrain* self){
	A2TerrainCell* cells=AEListAsArray(self->cellList,A2TerrainCell);
	A2TerrainCell* maincell=cells + self->activeCell;
	for (size_t i=0; i<maincell->info.visibleCount; i++)
		A2TerrainCellRender(cells+i);
	A2TerrainCellRender(maincell);
}

void A2TerrainStep(A2Terrain* self, float stepInSeconds){
	A2TerrainCell* cells=AEListAsArray(self->cellList,A2TerrainCell);
	A2TerrainCell* maincell=cells + self->activeCell;
	
	if(self->cameraHasMoved){
		self->cameraHasMoved=false;
		
		//First check the original, if we are still in it, then we can skip everything
		if (AEVec3IsBetween(self->lastCameraPosition, maincell->info.min, maincell->info.max)) return;
		//Now check the original's children
		for (size_t i=0; i<maincell->info.visibleCount; i++) {
			A2TerrainCell* cell=cells + maincell->info.visibles[i];
			if (not AEVec3IsBetween(self->lastCameraPosition, cell->info.min, cell->info.max)) continue;
			self->readyToLoad=true;
			self->activeCell=i;
			maincell=cells+i;
		}
		//This next part should be unnecessary (full scan), but I could easily implement it if I want
	}
	
	if(self->readyToLoad){
		self->readyToLoad=false;
		
		//Delete the old cells
		{
			size_t length=AEListLength(self->cellList);
			for (size_t i=0; i<length; i++) {
				
			}
		}
		//First load the main
		A2TerrainStepLoadCell(self,maincell);
	
		//Now load the main's children
		for (size_t i=0; i<maincell->info.visibleCount; i++) {
			A2TerrainCell* cell=cells + maincell->info.visibles[i];
			A2TerrainStepLoadCell(self,cell);
		}
	}
}
*/
/*void A2TerrainStepLoadCell(A2Terrain* self,A2TerrainCell* cell){
	FILE* file=NULL;
	char filename[16];
	void* memory=NULL;
	size_t memoryLength=0;
	snprintf(filename, 16-1,"  %8i",(int)self->activeCell);
			
	filename[0]='v';
	filename[1]='a';
	file=fopen(filename, "rb");
	fseek (file,0,SEEK_END);
	memoryLength=ftell(file);
	rewind(file);
	memory=calloc(memoryLength+1,1);
	fread(memory,1,memoryLength,file);
			
	A2TerrainCellLoadVAFromMemory(cell, memory, memoryLength);
			
	free(memory);
	fclose(file);
			
	filename[0]='i';
	filename[1]='a';
	file=fopen(filename, "rb");
	fseek (file,0,SEEK_END);
	memoryLength=ftell(file);
	rewind(file);
	memory=calloc(memoryLength+1,1);
	fread(memory,1,memoryLength,file);
			
	A2TerrainCellLoadIAFromMemory(cell, memory, memoryLength);
			
	free(memory);
	fclose(file);
			
	filename[0]='i';
	filename[1]='n';
	file=fopen(filename, "rb");
	fseek (file,0,SEEK_END);
	memoryLength=ftell(file);
	rewind(file);
	memory=calloc(memoryLength+1,1);
	fread(memory,1,memoryLength,file);
			
	A2TerrainCellLoadInfoFromMemory(cell, memory, memoryLength);
			
	free(memory);
	fclose(file);
	
	for (size_t i=0; i<self->info.visibleCount; i++) {
		A2TerrainStepLoadCell(self,AEListAsArray(self->cellList,A2TerrainCell)+i);
	}
}*/