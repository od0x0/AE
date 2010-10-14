#include "A2RWorld.h"

struct A2RWorld{
	AEList* areas;
	struct{
		AEVec3 position;
		AEVec3 rotation;
		float near, far, fov;
	}camera;
	unsigned int activeArea;
	AEList* vbos;
	AEList* materials;
};

typedef struct{
	AEVec3 min, max;
	unsigned int* visibles;
	unsigned char visibleCount;
	unsigned char LOD;
	unsigned char unused1;
	unsigned char unused2;
	A2RVM* rvm;
}A2RArea;

enum{
	A2RAreaLODNone,
	A2RAreaLODMinimal,
	A2RAreaLODMiddle,
	A2RAreaLODMaximum
};

A2RWorld* A2RWorldNew(void){
	A2RWorld* world=calloc(1, sizeof(A2RWorld));
	return world;
}

static void A2RWorldCalculateActiveArea(A2RWorld* self){
	A2RArea** areas=AEListAsArray(self->areas,A2RArea*);
	A2RArea* activearea=areas[self->activeArea];
	//First check the active area, if it is the one we are in, then we have nothing to do, just return
	if(AEVec3IsBetween(self->camera.position, activearea->min, activearea->max)) return;
	
	//Then check the visibles of the active area
	for(unsigned int i=0; i < activearea->visibleCount; i++){
		unsigned int* visibles=activearea->visibles;
		A2RArea* area=areas[visibles[i]];
		if(AEVec3IsBetween(self->camera.position, area->min, area->max)){
			self->activeArea=visibles[i];
			return;
		}
	}
	//If we don't find anything, then we have to go through all of the areas, it shouldn't be much of a problem for most people, but in the event that a level designer doesn't realize that "visibles" are the things that may be visible in the near future, and not what is visible just now, for example, 2 rooms and a door that can be open or closed
	for(unsigned int i=0; i < AEListLength(self->areas); i++){
		A2RArea* area=areas[i];
		if(AEVec3IsBetween(self->camera.position, area->min, area->max)){
			self->activeArea=i;
			return;
		}
	}
}

void A2RWorldHandleMove(A2RWorld* self){
	const unsigned int oldactivearea=self->activeArea;
	A2RWorldCalculateActiveArea(self);
	const unsigned int newactivearea=self->activeArea;
	if (oldactivearea == newactivearea) return;
	//Reload
	//A2RWorldAreaLoad(self,self->activeArea,A2RAreaLODMaximum);
}

void A2RWorldCameraPositionSet(A2RWorld* self,float x,float y,float z){
	self->camera.position=AEVec3FromCoords(x, y, z);
}

void A2RWorldCameraRotationSet(A2RWorld* self,float x,float y,float z){
	self->camera.rotation=AEVec3FromCoords(x, y, z);
}

void A2RWorldCameraBoundsSet(A2RWorld* self,float near,float far,float fov){
	self->camera.near=near;
	self->camera.far=far;
	self->camera.fov=fov;
}

void A2RWorldRender(A2RWorld* self){
	A2RArea** areas=AEListAsArray(self->areas,A2RArea*);
	A2RArea* area=areas[self->activeArea];
	A2RVMExecute(area->rvm);
	unsigned int* visibles=area->visibles;
	for(unsigned int i=0; i < area->visibleCount; i++){
		area=areas[visibles[i]];
		A2RVMExecute(area->rvm);
	}
}

void A2RWorldDelete(A2RWorld* self){
	if(not self) return;
	for(unsigned int i=0; i < AEListLength(self->areas); i++){
		A2RArea* area=AEListAsArray(self->areas,A2RArea*)[i];
		free(area->visibles);
		A2RVMDelete(area->rvm);
	}
	AEListDelete(self->areas);
	AEListDelete(self->vbos);
	AEListDelete(self->materials);
	free(self);
}

/*
	WorldFolder
		Info
		# of area
			Info
			min
				Material
				VBO
			mid
				Material
				VBO
			max
				Material
				VBO
*/

void A2RWorldAreaLoad(A2RWorld* world, unsigned int areanumber,int lod){
	
}