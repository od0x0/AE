#include "AEUI.h"

struct AEUI{
	size_t refcount;
	AEUIRect windowBounds;
	AEUIView* root;
	AEUIFont* font;
};

AEUI* AEUINew(char* fontpath, int size){
	AEUI* self=calloc(1,sizeof(AEUI));
	self->font=AEUIFontLoad(fontpath, size);
	return AEUIRetain(self);
}

AEUI* AEUIRetain(AEUI* self){
	if(self) self->refcount++;
	return self;
}

void AEUIDelete(AEUI* self){
	if(not self) return;
	self->refcount--;
	if(self->refcount) return;
	AEUIViewDelete(self->root);
	AEUIFontDelete(self->font);
	free(self);
}

void AEUIWindowBoundsGet(AEUI* self, AEUIRect* bounds){
	*bounds=self->windowBounds;
}

void AEUIWindowBoundsSet(AEUI* self, AEUIRect* bounds){
	self->windowBounds=*bounds;
}

AEUIView* AEUIRootViewGet(AEUI* self){
	return self->root;
}

void AEUIRootViewSet(AEUI* self, AEUIView* root){
	AEUIViewDelete(self->root);
	self->root=AEUIViewRetain(root);
}
AEUIFont* AEUIFontGet(AEUI* self){
	return self->font;
}