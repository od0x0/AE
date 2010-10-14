#include "A2RMap.h"

struct A2RMapCell{
	uint64_t ID;
	AEVec3 center;
	float radius;
	AEVA va; 
	AEVA ia;
	AETexture texture;
};

struct A2RMapTrigger{
	uint64_t ID;
	AEVec3 min;
	AEVec3 max;
};

struct A2RMap{
	A2RMapCell* cells;
	uint64_t cellCount;
	A2RMapTrigger* triggers;
	uint64_t triggerCount;
	
	AEVec3 cameraPosition;
	
	void (*triggerFunc)(A2RMap* self, A2RMapTrigger* trigger);
	
	void* userdata;
};

uint64_t A2RMapTriggerIDGet(A2RMapTrigger* self){
	return self->ID;
}

void A2RMapUserdataSet(A2RMap* self, void* userdata){
	self->userdata=userdata;
}

void* A2RMapUserdataGet(A2RMap* self){
	return self->userdata;
}

A2RMap* A2RMapNew(void (*triggerFunc)(A2RMap* self, A2RMapTrigger* trigger)){
	A2RMap* self=calloc(1, sizeof(A2RMap));
	self->triggerFunc=triggerFunc;
	return self;
}

void A2RMapRender(A2RMap* self){
	AECamera* camera=AECameraActiveGet();
	for (uint64_t i=0; i<self->cellCount; i++) {
		A2RMapCell* cell=self->cells+i;
		AEVec3 p=cell->center;
		const float r=cell->radius;
		if(r and not AECameraVFCheckSphere(camera, p.x, p.y, p.z, r)) continue;
		glTranslatef(p.x, p.y, p.z);
		AETextureBind(cell->texture);
		AEVABind(& cell->va);
		AEVABind(& cell->ia);
		AEVADraw(0, cell->ia.length);
		p=cell->center;
		glTranslatef(-p.x, -p.y, -p.z);
	}
}

void A2RMapLoadFromFILE(A2RMap* self, FILE* file){
	//Reload the new set of cells and triggers, deleting the old
	for (uint64_t i=0; i<self->cellCount; i++) {
		A2RMapCell* cell=self->cells+i;
		AETextureDelete(cell->texture);
		AEVADeinit(& cell->va);
		AEVADeinit(& cell->ia);
	}
	free(self->cells);
	free(self->triggers);
	
	self->cellCount=0;
	self->triggerCount=0;
	
	/*

struct A2RMapTrigger{
	uint64_t ID;
	AEVec3 min;
	AEVec3 max;
};
	*/
	
	uint64_t triggerCount=0;
	fread(&triggerCount, 1, sizeof(uint64_t), file);
	triggerCount=AEUInt64From8Bytes((uint8_t*)&triggerCount, true);
	self->triggerCount=triggerCount;
	
	for (uint64_t i=0; i<self->triggerCount; i++) {
		A2RMapTrigger* trigger=self->triggers+i;
		
		uint64_t ID=0;
		fread(&ID, 1, sizeof(uint64_t), file);
		ID=AEUInt64From8Bytes((uint8_t*)&ID, true);
		
		trigger->ID=ID;
		
		fread(& trigger->min, 1, sizeof(AEVec3), file);
		fread(& trigger->max, 1, sizeof(AEVec3), file);
	}
	
	//Load the cells
	uint64_t cellCount=0;
	fread(&cellCount, 1, sizeof(uint64_t), file);
	cellCount=AEUInt64From8Bytes((uint8_t*)&cellCount, true);
	self->cellCount=cellCount;
	
	for (uint64_t i=0; i<self->cellCount; i++) {
		A2RMapCell* cell=self->cells+i;
		
		uint64_t ID=0;
		fread(&ID, 1, sizeof(uint64_t), file);
		ID=AEUInt64From8Bytes((uint8_t*)&ID, true);
		
		cell->ID=ID;
		
		fread(& cell->center, 1, sizeof(AEVec3), file);
		fread(& cell->radius, 1, sizeof(float), file);
		
		AEVAUnserializeFromFILE(& cell->va, file);
		AEVAUnserializeFromFILE(& cell->ia, file);
		
		uint64_t textureMemorySize=0;
		fread(&textureMemorySize, 1, sizeof(uint64_t), file);
		textureMemorySize=AEUInt64From8Bytes((uint8_t*)&textureMemorySize, true);
		void* textureMemory=malloc(textureMemorySize);
		fread(textureMemory, 1, textureMemorySize, file);
		cell->texture=AETextureLoadFromMemoryWithFlags(textureMemory, textureMemorySize, AETextureLoadFlagDXTCompression | AETextureLoadFlagMipmaps);
		free(textureMemory);
	}
}

void A2RMapCameraPositionSet(A2RMap* self, float x, float y, float z){
		
	{
		const AEVec3 then=self->cameraPosition;
		const AEVec3 now={x,y,z};

		//If it is the same place as the last call, just return
		if((now.x==then.x) and (now.y==then.y) and (now.z==then.z)) return;
		//Else, change the position
		self->cameraPosition=now;
	}
	
	for (uint64_t i=0; i<self->triggerCount; i++) {
		A2RMapTrigger* trigger=self->triggers+i;
		const AEVec3 p=self->cameraPosition;
		const AEVec3 min=trigger->min;
		const AEVec3 max=trigger->max;
		if (AEVec3IsBetween(p, min, max)) {
			if(i==0) return;//0 is the current area index
			self->triggerFunc(self, trigger);
		}
	}
}

void A2RMapDelete(A2RMap* self){
	if(not self) return;
	
	for (uint64_t i=0; i<self->cellCount; i++) {
		A2RMapCell* cell=self->cells+i;
		AETextureDelete(cell->texture);
		AEVADeinit(& cell->va);
		AEVADeinit(& cell->ia);
	}
	free(self->cells);
	
	free(self->triggers);
	
	free(self);
}