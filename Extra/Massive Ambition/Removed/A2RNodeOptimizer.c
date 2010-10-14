#include "A2RNodeOptimizer.h"

size_t A2RNodeVertexCount(A2RNode* node){
	if(not node) return 0;
	
	switch (node->type) {
		case A2RNodeTypeVFC:
			return A2RNodeVertexCount(node->data.vfc.child);
		case A2RNodeTypeObject:
			return A2RNodeVertexCount(node->data.object.mesh);
		case A2RNodeTypeGroup:{
			size_t count=0;
			for(size_t i=0;i<AEListLength(node->data.group.children);i++) 
				count+=A2RNodeVertexCount(AEListAsArray(node->data.group.children,A2RNode*)[i]);
			return count;
			}
		case A2RNodeTypeMesh:
			return AEListLength(node->data.mesh.positions);
		default:
			break;
	}
	
	return 0;
}

size_t A2RNodeTexelCount(A2RNode* node){
	if(not node) return 0;
	
	switch (node->type) {
		case A2RNodeTypeVFC:
			return A2RNodeTexelCount(node->data.vfc.child);
		case A2RNodeTypeObject:
			return A2RNodeTexelCount(node->data.object.material);
		case A2RNodeTypeGroup:{
			size_t count=0;
			for(size_t i=0;i<AEListLength(node->data.group.children);i++) 
				count+=A2RNodeTexelCount(AEListAsArray(node->data.group.children,A2RNode*)[i]);
			return count;
			}
		case A2RNodeTypeMaterial:{
			size_t count=0;
			for(size_t i=0;i<AEListLength(node->data.material.images);i++){
				AEImage* image=AEListAsArray(node->data.material.images,AEImage*)[i];
				count+=image->w*image->h;
			}
			return count;
			}
		default:
			break;
	}
	
	return 0;
}

void A2RNodeToRVMOplist(A2RNode* node, A2RVMOplist* ops){
	if(not node) return;
	
	switch(node->type){
		case A2RNodeTypeGroup:
			for(size_t i=0; i<AEListLength(node->data.group.children); i++){
				A2RNode* child=AEListAsArray(node->data.group.children,A2RNode*)[i];
				A2RNodeToRVMOplist(child,ops);
			}
			break;
		case A2RNodeTypeMesh:
			
			break;
		case A2RNodeTypeMaterial:
			
			break;
		case A2RNodeTypeObject:
			//Transform
			A2RNodeToRVMOplist(node->data.object.material,ops);
			A2RNodeToRVMOplist(node->data.object.mesh,ops);
			break;
	}
}