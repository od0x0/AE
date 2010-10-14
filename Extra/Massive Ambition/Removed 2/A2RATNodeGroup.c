#include "A2RATNodeGroup.h"


static void A2RATNodeTypeGroupDeinit(A2RATNode* self,...){
	AEListDelete(A2RATNodeSlotGet(self, "children"));
}

A2RATNodeType A2RATNodeTypeGroup_internal={
	.name="Group",
	//.init=A2RATNodeTypeGroupInit,
	.deinit=A2RATNodeTypeGroupDeinit,
	.last=NULL
};

A2RATNodeType* A2RATNodeTypeGroup(void){
	static bool initialized=false;
	A2RATNodeType* self=&A2RATNodeTypeGroup_internal;
	if(not initialized){
		initialized=true;
		A2RATNodeTypesAdd(self);
	}
	return self;
}

void A2RATNodeChildrenAdd(A2RATNode* self, A2RATNode* child){
	AEList* children=A2RATNodeSlotGet(self, "children");
	if(not children){
		children=AEListNew(A2RATNode*);
		A2RATNodeSlotSet(self, "children", children);
	}
	child=A2RATNodeRetain(child);
	AEListAddBytes(children, &child);
}

void A2RATNodeChildrenRemove(A2RATNode* self, A2RATNode* child){
	AEList* children=A2RATNodeSlotGet(self, "children");
	if(not children) return;
	A2RATNodeDelete(child);
	AEListRemoveBytes(children, &child);
}

AEList* A2RATNodeChildrenGet(A2RATNode* self){
	return A2RATNodeSlotGet(self, "children");
}

static void A2RATNodeGroupOptimize(A2RATNode* self){
	if(not self) return;
	AEList* children=A2RATNodeChildrenGet(self);
	if(not children) return;
	
	//Split Ents that intersect the areas
	
	//Run through the children, request that they optimize.
	for (size_t i=0; i<AEListLength(children); i++) {
		A2RATNode* ent=AEListAsArray(children,A2RATNode*)[i];
		if(not ent) continue;
		A2RATNodeOptimize(ent);
	}
	
	//Sort child order, grouping similar materials, meshes, and positions.
	{
		//Group by materials
		AEList* newchildren=AEListNew(A2RATNode*);
		AEListAddBytes(newchildren, AEListAsArray(children,A2RATNode*)+0);
	
		for (size_t i=1; i<AEListLength(children); i++) {
			A2RATNode* ent=AEListAsArray(children,A2RATNode*)[i];
		
			if(not ent) continue;
			
			//Get last child of the the new list
			A2RATNode* lastent=AEListAsArray(newchildren,A2RATNode*)[i];
			
			//Check the property we care about
			if(A2RATNodeEntMeshGet(ent)==A2RATNodeEntMeshGet(lastent)) AEListAddBytes(newchildren, &ent);
		}
	
		AEListDelete(children);
		children=newchildren;
	}
	
	//Merge meshes that are too small
	
	
}