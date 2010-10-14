#include "A2RVMEnt.h"


struct A2RVMEnt{
	size_t refcount;

	A2RVMEnt* parent;
	char* name;

	struct{
		AEVec3 position;
		AEVec3 rotation;
	}transform;
	
	struct{
		struct{
			AEVec3 center;
			float radius;
		}vfc;
	
		struct{
			AEImage* textures[2];
			AEImage* lightmap;
		}material;
		
		AEVA va;
		AEVA ia;
	}render;
	
	struct{
		char isMovable;
		char isCullable;
	}flags;
};

A2RVMEnt* A2RVMEntNew(void){
	A2RVMEnt* self=calloc(1, sizeof(A2RVMEnt));
	return A2RVMEntRetain(self);
}

A2RVMEnt* A2RVMEntRetain(A2RVMEnt* self){
	if(self) self->refcount++;
	return self;
}

void A2RVMEntDelete(A2RVMEnt* self){
	if(not self) return;
	self->refcount--;
	if(self->refcount) return;
	
	A2RVMEntDelete(self->parent);
	free(self->name);
	AEImageDelete(self->render.material.textures[0]);
	AEImageDelete(self->render.material.textures[1]);
	AEImageDelete(self->render.material.lightmap);
	AEVADeinit(&(self->render.va));
	AEVADeinit(&(self->render.ia));
}

void A2RVMEntParentSet(A2RVMEnt* self, A2RVMEnt* parent){
	A2RVMEntDelete(parent);
	self->parent=A2RVMEntRetain(parent);
}

A2RVMEnt* A2RVMEntParentGet(A2RVMEnt* self){
	return self->parent;
}

void A2RVMEntNameSet(A2RVMEnt* self, char* name){
	free(self->name);
	if(name==NULL) self->name=NULL;
	else self->name=strdup(name);
}

char* A2RVMEntNameGet(A2RVMEnt* self){
	return self->name;
}

void A2RVMEntPositionSet(A2RVMEnt* self, float x, float y, float z){
	AEVec3 v3 = {x,y,z};
	self->transform.position = v3;
}

void A2RVMEntPositionGet(A2RVMEnt* self, AEVec3* v3){
	*v3 = self->transform.position;
}

void A2RVMEntRotationSet(A2RVMEnt* self, float x, float y, float z){
	AEVec3 v3 = {x,y,z};
	self->transform.rotation = v3;
}

void A2RVMEntRotationGet(A2RVMEnt* self, AEVec3* v3){
	*v3 = self->transform.rotation;
}

void A2RVMEntCullableSet(A2RVMEnt* self, bool value){
	self->flags.isCullable=value;
}

bool A2RVMEntCullableGet(A2RVMEnt* self){
	return self->flags.isCullable;
}

void A2RVMEntStaticSet(A2RVMEnt* self, bool value){
	self->flags.isMovable=not value;
}

bool A2RVMEntStaticGet(A2RVMEnt* self){
	return not self->flags.isMovable;
}

AEVA* A2RVMEntVAGet(A2RVMEnt* self){
	return &(self->render.va);
}

AEVA* A2RVMEntIAGet(A2RVMEnt* self){
	return &(self->render.ia);
}

void A2RVMEntVFCGen(A2RVMEnt* self){
	//Calculate the vfc stuff from the va
	AEVA* va=A2RVMEntVAGet(self);
	
}

void A2RVMEntTextureSet(A2RVMEnt* self, int slot, AEImage* image){

}