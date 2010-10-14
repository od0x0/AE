#include "A2RATNodeEnt.h"
static void A2RATNodeTypeEntDeinit(A2RATNode* self,...){
	free(A2RATNodeSlotGet(self, "position"));
	free(A2RATNodeSlotGet(self, "rotation"));
	A2RATNodeEntMeshSet(self, NULL);
	A2RATNodeEntTrackingNameSet(self, NULL);
}

A2RATNodeType A2RATNodeTypeEnt_internal={
	.name="Ent",
	//.init=A2RATNodeTypeEntInit,
	.deinit=A2RATNodeTypeEntDeinit,
	.last=NULL
};

A2RATNodeType* A2RATNodeTypeEnt(void){
	static bool initialized=false;
	A2RATNodeType* self=&A2RATNodeTypeEnt_internal;
	if(not initialized){
		initialized=true;
		A2RATNodeTypesAdd(self);
	}
	return self;
}

void A2RATNodeEntPositionSet(A2RATNode* self, float x, float y, float z){
	AEVec3* pointer=A2RATNodeSlotGet(self, "position");
	if(not pointer) pointer=malloc(sizeof(AEVec3));
	pointer->x=x;
	pointer->y=y;
	pointer->z=z;
	A2RATNodeSlotSet(self, "position", pointer);
}

void A2RATNodeEntPositionGet(A2RATNode* self, AEVec3* v3){
	AEVec3* pointer=A2RATNodeSlotGet(self, "position");
	if(pointer) memcpy(v3, pointer, sizeof(AEVec3));
	else memset(v3, 0, sizeof(AEVec3));
}

void A2RATNodeEntRotationSet(A2RATNode* self, float x, float y, float z){
	AEVec3* pointer=A2RATNodeSlotGet(self, "rotation");
	if(not pointer) pointer=malloc(sizeof(AEVec3));
	pointer->x=x;
	pointer->y=y;
	pointer->z=z;
	A2RATNodeSlotSet(self, "rotation", pointer);
}

void A2RATNodeEntRotationGet(A2RATNode* self, AEVec3* v3){
	AEVec3* pointer=A2RATNodeSlotGet(self, "rotation");
	if(pointer) memcpy(v3, pointer, sizeof(AEVec3));
	else memset(v3, 0, sizeof(AEVec3));
}

void A2RATNodeEntMeshSet(A2RATNode* self, A2RATNode* mesh){
	A2RATNodeDelete(A2RATNodeEntMeshGet(self));
	A2RATNodeSlotSet(self, "mesh", A2RATNodeRetain(mesh));
}

A2RATNode* A2RATNodeEntMeshGet(A2RATNode* self){
	return A2RATNodeSlotGet(self, "mesh");
}

void A2RATNodeEntTrackingNameSet(A2RATNode* self, char* name){
	free(A2RATNodeSlotGet(self, "trackingname"));
	if (name) name=strdup(name);
	A2RATNodeSlotSet(self, "trackingname", name);
}

char* A2RATNodeEntTrackingNameGet(A2RATNode* self){
	return A2RATNodeSlotGet(self, "trackingname");
}

static void A2RATNodeEntOptimize(A2RATNode* self){
	//If the mesh of the ent is too large, split it.
	
	
	//Optimize children
	
	
}