#include "A2RMapEntity.h"

struct A2RMapEntity{
	size_t refcount;

	AEVec3 position;
	AEVec3 rotation;
	
	AEVA va;
	AEVA ia;
	AEMaterial* material;
	
	void* userdata;
};

A2RMapEntity* A2RMapEntityNew(void){
	A2RMapEntity* self=calloc(1, sizeof(A2RMapEntity));
	return A2RMapEntityRetain(self);
}

A2RMapEntity* A2RMapEntityRetain(A2RMapEntity* self){
	if(self) self->refcount++;
	return self;
}

void A2RMapEntityDelete(A2RMapEntity* self){
	if(not self) return;
	self->refcount--;
	if(self->refcount) return;
	
	AEVADeinit(& self->va);
	AEVADeinit(& self->ia);
	AEMaterialDelete(self->material);
	
	free(self);
}

void A2RMapEntityRender(A2RMapEntity* self){
	const AEVec3 p=self->position;
	const AEVec3 r=self->rotation;
	
	//Only rotate when we need to rotate
	glTranslatef(p.x, p.y, p.z);
	if(r.x) glRotatef(r.x,	1,0,0);
	if(r.y) glRotatef(r.y,	0,1,0);
	if(r.z) glRotatef(r.z,	0,0,1);
	
	AEMaterialBind(self->material);
	AEVABind(& self->va);
	AEVABind(& self->ia);
	
	AEVADraw(0, self->ia.length);
	
	if(r.z) glRotatef(-r.z,	0,0,1);
	if(r.y) glRotatef(-r.y,	0,1,0);
	if(r.x) glRotatef(-r.x,	1,0,0);
	glTranslatef(-p.x, -p.y, -p.z);
}

AEVA* A2RMapEntityVAGetPointer(A2RMapEntity* self){return & self->va;}
AEVA* A2RMapEntityIAGetPointer(A2RMapEntity* self){return & self->ia;}

void* A2RMapEntityUserdataGet(A2RMapEntity* self){return self->userdata;}
void A2RMapEntityUserdataSet(A2RMapEntity* self, void* data){self->userdata=data;}

AEMaterial* A2RMapEntityMaterialGet(A2RMapEntity* self){return self->material;}
void A2RMapEntityMaterialSet(A2RMapEntity* self, AEMaterial* material){
	AEMaterialDelete(self->material);
	self->material=AEMaterialRetain(material);
}

void A2RMapEntityPositionGet(A2RMapEntity* self, AEVec3* v3){*v3 = self->position;}
void A2RMapEntityRotationGet(A2RMapEntity* self, AEVec3* v3){*v3 = self->rotation;}
void A2RMapEntityPositionSet(A2RMapEntity* self, float x, float y, float z){self->position = ((AEVec3){x,y,z});}
void A2RMapEntityRotationSet(A2RMapEntity* self, float x, float y, float z){self->rotation = ((AEVec3){x,y,z});}