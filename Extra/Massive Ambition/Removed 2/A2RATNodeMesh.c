#include "A2RATNodeMesh.h"

static void A2RATNodeTypeMeshDeinit(A2RATNode* self,...){
	AEListDelete(A2RATNodeMeshVerticesGet(self));
	AEListDelete(A2RATNodeMeshNormalsGet(self));
	size_t count=A2RATNodeMeshTexUnitCountGet(self);
	for(size_t i=0; i<count; i++){
		AEListDelete(A2RATNodeMeshTexCoordsGet(self,i));
	}
	
}

A2RATNodeType A2RATNodeTypeMesh_internal={
	.name="Mesh",
	.deinit=A2RATNodeTypeMeshDeinit,
	.last=NULL
};

A2RATNodeType* A2RATNodeTypeMesh(void){
	static bool initialized=false;
	A2RATNodeType* self=&A2RATNodeTypeMesh_internal;
	if(not initialized){
		initialized=true;
		A2RATNodeTypesAdd(self);
	}
	return self;
}

void A2RATNodeMeshVerticesSet(A2RATNode* self, AEList* vec3list){
	AEListDelete(A2RATNodeSlotGet(self, "v"));
	A2RATNodeSlotSet(self, "v", AEListCopy(vec3list));
}

AEList* A2RATNodeMeshVerticesGet(A2RATNode* self){
	return A2RATNodeSlotGet(self, "v");
}

void A2RATNodeMeshNormalsSet(A2RATNode* self, AEList* vec3list){
	AEListDelete(A2RATNodeSlotGet(self, "n"));
	A2RATNodeSlotSet(self, "n", AEListCopy(vec3list));
}

AEList* A2RATNodeMeshNormalsGet(A2RATNode* self){
	return A2RATNodeSlotGet(self, "n");
}

void A2RATNodeMeshTexCoordsSet(A2RATNode* self, size_t unit, AEList* vec2list){
	if(A2RATNodeMeshTexUnitCountGet(self) < unit){
		A2RATNodeSlotSet(self, "tucount", (void*)unit);
	}
	char name[]={'t','0'+unit,'\0'};
	AEListDelete(A2RATNodeSlotGet(self, name));
	A2RATNodeSlotSet(self, name, AEListCopy(vec2list));
}

AEList* A2RATNodeMeshTexCoordsGet(A2RATNode* self, int unit){
	if(A2RATNodeMeshTexUnitCountGet(self) < unit) return NULL;
	char name[]={'t','0'+unit,'\0'};
	return A2RATNodeSlotGet(self, name);
}

size_t A2RATNodeMeshTexUnitCountGet(A2RATNode* self){
	return (size_t)A2RATNodeSlotGet(self, "tucount");
}

static void A2RATNodeMeshOptimize(A2RATNode* self){
	//We could run through the mesh, removing polygons that are wasted, but it's probably not going to give much of a speed boost.
}