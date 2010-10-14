#include "A2RATNodeTexture.h"

//static void A2RATNodeTypeTextureInit(A2RATNode* self){}

static void A2RATNodeTypeTextureDeinit(A2RATNode* self,...){
	AEImageDelete(A2RATNodeTextureImageGet(self));
}

A2RATNodeType A2RATNodeTypeTexture_internal={
	.name="Texture",
	//.init=A2RATNodeTypeTextureInit,
	.deinit=A2RATNodeTypeTextureDeinit,
	.last=NULL
};

A2RATNodeType* A2RATNodeTypeTexture(void){
	static bool initialized=false;
	A2RATNodeType* self=&A2RATNodeTypeTexture_internal;
	if(not initialized){
		initialized=true;
		A2RATNodeTypesAdd(self);
	}
	return self;
}

void A2RATNodeTextureImageSet(A2RATNode* self, AEImage* image){
	A2RATNodeSlotSet(self, "image", image);
}

AEImage* A2RATNodeTextureImageGet(A2RATNode* self){
	return A2RATNodeSlotGet(self, "image");
}

static void A2RATNodeTextureOptimize(A2RATNode* self){
	//Texture compression is really something best left up to the artist to decide, so I won't bother here.
	
}