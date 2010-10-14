#include "../Node.h"

unsigned char AENodeTypeGet(AENode* node){return node->type;}
AENode* AENodeRetain(AENode* node){node->refcount++;return node;}

AENode* AENodeNew(unsigned char type,char* name){
	size_t size=0;
	switch(type){
		case AENodeTypeEnt: size=sizeof(AENodeEnt); break;
		case AENodeTypeMesh: size=sizeof(AENodeMesh); break;
		case AENodeTypeMaterial: size=sizeof(AENodeMaterial); break;
	}
	AENode* node=calloc(1,size);
	node->name=strdup(name);
	node->type=type;
	return AENodeRetain(node);
};

void AENodeDelete(AENode* node){
	if(node==NULL) return;
	node->refcount--;
	if(node->refcount) return;
	switch(node->type){
		case AENodeTypeEnt: AENodeEntDelete(node); break;
		case AENodeTypeMesh: AENodeMeshDelete(node); break;
		case AENodeTypeMaterial: AENodeMaterialDelete(node); break;
	}
	free(node->name);
	free(node);
}

void AENodeRender(AENode* node){
	if(node==NULL) return;
	switch(node->type){
		case AENodeTypeEnt: AENodeEntRender(node); break;
		case AENodeTypeMesh: AENodeMeshRender(node); break;
		case AENodeTypeMaterial: AENodeMaterialRender(node); break;
	}
}

void AENodeStep(AENode* node,float stepInSeconds){
	if(node==NULL) return;
	switch(node->type){
		case AENodeTypeEnt: AENodeEntStep(node,stepInSeconds); break;
		case AENodeTypeMesh: AENodeMeshStep(node,stepInSeconds); break;
		case AENodeTypeMaterial: AENodeMaterialStep(node,stepInSeconds); break;
	}
}