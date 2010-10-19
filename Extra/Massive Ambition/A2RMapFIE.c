#include "A2RMapFIE.h"

void A2RMapFIEInit(A2RMapFIE* self){
	memset(self, 0, sizeof(A2RMapFIE));
}

void A2RMapFIEDeinit(A2RMapFIE* self){
	if(not self) return;
	AEVADeinit(& self->va);
	AEVADeinit(& self->ia);
	AEMaterialDelete(self->material);
	self->material=NULL;
}

AEVA* A2RMapFIEVAGetPointer(A2RMapFIE* self){
	return & self->va;
}

AEVA* A2RMapFIEIAGetPointer(A2RMapFIE* self){
	return & self->ia;
}

AEMaterial* A2RMapFIEMaterialGet(A2RMapFIE* self){
	return self->material;
}

void A2RMapFIEMaterialSet(A2RMapFIE* self, AEMaterial* material){
	AEMaterialDelete(self->material);
	self->material=AEMaterialRetain(material);
}