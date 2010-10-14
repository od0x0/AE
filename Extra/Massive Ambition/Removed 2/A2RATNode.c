#include "A2RATNode.h"

static A2RATNodeType A2RATNodeTypeNone_internal={
	.name="None",
	.last=NULL
};
A2RATNodeType* A2RATNodeTypeNone(void){return &A2RATNodeTypeNone_internal;}
static A2RATNodeType* A2RATNodeTypeLast=&A2RATNodeTypeNone_internal;

void A2RATNodeTypesAdd(A2RATNodeType* type){
	type->last=A2RATNodeTypeLast;
	A2RATNodeTypeLast=type;
}

static A2RATNodeType* A2RATNodeTypeLookup_internal(A2RATNodeType* self, char* name){
	if(not self) return NULL;
	if(strcmp(self->name,name)==0) return self;
	return A2RATNodeTypeLookup_internal(self->last, name);
}

A2RATNodeType* A2RATNodeTypesLookup(char* name){
	return A2RATNodeTypeLookup_internal(A2RATNodeTypeLast, name);
}

struct A2RATNode{
	size_t refcount;
	AETable* slots;
	A2RATNodeType* type;
};

A2RATNodeType* A2RATNodeTypeGet(A2RATNode* self){
	return self->type;
}

A2RATNode* A2RATNodeNew(A2RATNodeType* type){
	A2RATNode* self=calloc(1, sizeof(A2RATNode));
	self->type=type;
	if(type->init) type->init(self);
	return A2RATNodeRetain(self);
}

A2RATNode* A2RATNodeRetain(A2RATNode* self){
	if(self) self->refcount++;
	return self;
}

void A2RATNodeDelete(A2RATNode* self){
	if(not self) return;
	self->refcount--;
	if(self->refcount) return;
	
	if(self->type->deinit) self->type->deinit(self);
	AETableDelete(self->slots);
	free(self);
}

void* A2RATNodeSlotGet(A2RATNode* self, char* name){
	if(not self) return NULL;
	if(not self->slots) return NULL;
	
	return AETableGet(self->slots, name);
}

void A2RATNodeSlotSet(A2RATNode* self, char* name, void* value){
	if(not self) return;
	if(not name) return;
	
	if(not self->slots) self->slots = AETableNew();
	AETableSet(self->slots, name, value);
}

void A2RATNodeOptimize(A2RATNode* self){
	A2RATNodeMessageFunc func=A2RATNodeSlotGet(self, "Optimize");
	if(func) func(self);
}