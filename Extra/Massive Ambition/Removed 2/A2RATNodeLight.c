#include "A2RATNodeLight.h"

static void A2RATNodeTypeLightDeinit(A2RATNode* self,...){
	free(A2RATNodeSlotGet(self, "position"));
	free(A2RATNodeSlotGet(self, "rotation"));
	free(A2RATNodeSlotGet(self, "radius"));
	A2RATNodeLightTrackingNameSet(self, NULL);
}

A2RATNodeType A2RATNodeTypeLight_internal={
	.name="Light",
	//.init=A2RATNodeTypeLightInit,
	.deinit=A2RATNodeTypeLightDeinit,
	.last=NULL
};

A2RATNodeType* A2RATNodeTypeLight(void){
	static bool initialized=false;
	A2RATNodeType* self=&A2RATNodeTypeLight_internal;
	if(not initialized){
		initialized=true;
		A2RATNodeTypesAdd(self);
	}
	return self;
}

void A2RATNodeLightPositionSet(A2RATNode* self, float x, float y, float z){
	AEVec3* pointer=A2RATNodeSlotGet(self, "position");
	if(not pointer) pointer=malloc(sizeof(AEVec3));
	pointer->x=x;
	pointer->y=y;
	pointer->z=z;
	A2RATNodeSlotSet(self, "position", pointer);
}

void A2RATNodeLightPositionGet(A2RATNode* self, AEVec3* v3){
	AEVec3* pointer=A2RATNodeSlotGet(self, "position");
	if(pointer) memcpy(v3, pointer, sizeof(AEVec3));
	else memset(v3, 0, sizeof(AEVec3));
}

void A2RATNodeLightRotationSet(A2RATNode* self, float x, float y, float z){
	AEVec3* pointer=A2RATNodeSlotGet(self, "rotation");
	if(not pointer) pointer=malloc(sizeof(AEVec3));
	pointer->x=x;
	pointer->y=y;
	pointer->z=z;
	A2RATNodeSlotSet(self, "rotation", pointer);
}

void A2RATNodeLightRadiusSet(A2RATNode* self, float radius){
	float* pointer=A2RATNodeSlotGet(self, "radius");
	if(not pointer) pointer=malloc(sizeof(float));
	*pointer=radius;
	A2RATNodeSlotSet(self, "radius", pointer);
}

float A2RATNodeLightRadiusGet(A2RATNode* self){
	float* pointer=A2RATNodeSlotGet(self, "radius");
	if(pointer) return *pointer;
	return 0;
}

void A2RATNodeLightTrackingNameSet(A2RATNode* self, char* name){
	free(A2RATNodeSlotGet(self, "trackingname"));
	if (name) name=strdup(name);
	A2RATNodeSlotSet(self, "trackingname", name);
}

char* A2RATNodeLightTrackingNameGet(A2RATNode* self){
	return A2RATNodeSlotGet(self, "trackingname");
}