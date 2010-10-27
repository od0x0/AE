#include "AEUI.h"

struct AEUIView{
	size_t refcount;
	char* name;
	AEUI* ui;
	AEUIView* parent;
	AEArray(AEUIView*) children;
	AEUIRect bounds;
	AEUIViewDrawFunc drawFunc;
	AEUIViewEventFunc eventFunc;
	AEUIViewDeinitFunc deinitFunc;
	void* userdata;
};

AEUIView* AEUIViewNew(AEUI* ui){
	AEUIView* self=calloc(1,sizeof(AEUIView));
	self->ui=ui;
	return AEUIViewRetain(self);
}

AEUIView* AEUIViewRetain(AEUIView* self){
	if(self) self->refcount++;
	return self;
}

void AEUIViewDelete(AEUIView* self){
	if(not self) return;
	self->refcount--;
	if(self->refcount) return;
	self->deinitFunc(self);
	size_t childCount=AEArrayLength(& self->children);
	for (size_t i=0; i<childCount; i++) {
		AEUIViewDelete(AEArrayAsCArray(& self->children)[i]);
	}
	AEArrayDeinit(& self->children);
	free(self->name);
	free(self);
}

void AEUIViewRender(AEUIView* self){
	self->drawFunc(self);
}
void AEUIViewEvent(AEUIView* self, const AEUIEvent* event){
	self->eventFunc(self, event);
}

void AEUIViewNameSet(AEUIView* self, char* name){
	free(self->name);
	self->name=strdup(name);
}

char* AEUIViewNameGet(AEUIView* self){
	return self->name;
}

AEUI* AEUIViewUIGet(AEUIView* self){
	return self->ui;
}

void AEUIViewParentSet(AEUIView* self, AEUIView* parent){
	self->parent=parent;
}

AEUIView* AEUIViewParentGet(AEUIView* self){
	return self->parent;
}

void AEUIViewChildrenAdd(AEUIView* self, AEUIView* child){
	child=AEUIViewRetain(child);
	if(child) AEUIViewParentSet(child, self);
	AEArrayAdd(& self->children, child);
}

void AEUIViewChildrenRemove(AEUIView* self, AEUIView* child){
	AEUIViewDelete(child);
	AEArrayRemoveBytes(& self->children, &child);
}

AEUIView* AEUIViewChildrenGet(AEUIView* self, size_t i){
	return AEArrayAsCArray(& self->children)[i];
}

void AEUIViewChildrenSet(AEUIView* self, size_t i, AEUIView* child){
	child=AEUIViewRetain(child);
	AEUIViewDelete(AEArrayAsCArray(& self->children)[i]);
	AEArrayAsCArray(& self->children)[i]=child;
}

size_t AEUIViewChildrenCount(AEUIView* self, size_t i){
	return AEArrayLength(& self->children);
}

void AEUIViewBoundsSet(AEUIView* self, AEUIRect* bounds){
	self->bounds=*bounds;
}

void AEUIViewBoundsGet(AEUIView* self, AEUIRect* bounds){
	*bounds=self->bounds;
}

void AEUIViewDrawFuncSet(AEUIView* self, AEUIViewDrawFunc func){
	self->drawFunc=func;
}

AEUIViewDrawFunc AEUIViewDrawFuncGet(AEUIView* self){
	return self->drawFunc;
}

void AEUIViewEventFuncSet(AEUIView* self, AEUIViewEventFunc func){
	self->eventFunc=func;
}

AEUIViewEventFunc AEUIViewEventFuncGet(AEUIView* self){
	return self->eventFunc;
}

void AEUIViewDeinitFuncSet(AEUIView* self, AEUIViewDeinitFunc func){
	self->deinitFunc=func;
}

AEUIViewDeinitFunc AEUIViewDeinitFuncGet(AEUIView* self){
	return self->deinitFunc;
}

void AEUIViewUserdataSet(AEUIView* self, void* userdata){
	self->userdata=userdata;
}

void* AEUIViewUserdataGet(AEUIView* self){
	return self->userdata;
}