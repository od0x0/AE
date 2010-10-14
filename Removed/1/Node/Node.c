#include "NodeInternal.h"

AENode* AENodeRoot;

//The code here is pretty straightforward, it abstracts the lower level stuff

void AENodesInit(char* serverAddress,char* username,char* password,unsigned char* hostID){
	
}

void AENodesQuit(void){
	
}

//Ideally, xxxStep should be called with constant stepsize
void AENodesStep(float stepInSeconds){
	
}

void AENodesRender(void){
	AENodeRender(AENodeRoot);
}

void AENodeActiveWorldSet(AENode* node){
	AENodeRoot=node;
}

AENode* AENodeActiveWorldGet(void){
	return AENodeRoot;
}

unsigned char AENodeTypeGet(AENode* node){return node->type;}

char* AENodeNameGet(AENode* node){return node->name;}

void AENodeNameSet(AENode* node,char* name){
	free(node->name);
	node->name=name;
}

AENode* AENodeRetain(AENode* node){node->refcount++;return node;}

AENode* AENodeNew(unsigned char type,char* name){
	size_t size=0;
	//Start off witjh getting size first, to cut off redundant code
	switch(type) {
		case AENodeTypeEnt: size=sizeof(AENodeEnt); break;
		case AENodeTypeMesh: size=sizeof(AENodeMesh); break;
		case AENodeTypeMaterial: size=sizeof(AENodeMaterial); break;
		case AENodeTypeWorld: size=sizeof(AENodeWorld); break;
	}
	AENode* node=calloc(1,size);
	node->name=strdup(name);
	node->type=type;
	//init the memory if it must
	switch(type) {
		case AENodeTypeEnt: break;
		case AENodeTypeMesh: break;
		case AENodeTypeMaterial: break;
		case AENodeTypeWorld: AENodeWorldInit(node); break;
	}
	return AENodeRetain(node);
};

void AENodeDelete(AENode* node){
	if(node==NULL) return;
	node->refcount--;
	if(node->refcount) return;
	switch(node->type) {
		case AENodeTypeEnt: AENodeEntDelete(node); break;
		case AENodeTypeMesh: AENodeMeshDelete(node); break;
		case AENodeTypeMaterial: AENodeMaterialDelete(node); break;
		case AENodeTypeWorld: AENodeWorldDelete(node); break;
	}
	free(node->name);
	free(node);
}

void AENodeRender(AENode* node){
	if(node==NULL) return;
	switch(node->type) {
		case AENodeTypeEnt: AENodeEntRender(node); break;
		case AENodeTypeMesh: AENodeMeshRender(node); break;
		case AENodeTypeMaterial: AENodeMaterialRender(node); break;
		case AENodeTypeWorld: AENodeWorldRender(node); break;
	}
}

void AENodeStep(AENode* node,float stepInSeconds){
	if(node==NULL) return;
	switch(node->type) {
		case AENodeTypeEnt: AENodeEntStep(node,stepInSeconds); break;
		case AENodeTypeMesh: AENodeMeshStep(node,stepInSeconds); break;
		case AENodeTypeMaterial: AENodeMaterialStep(node,stepInSeconds); break;
		case AENodeTypeWorld: AENodeWorldStep(node,stepInSeconds); break;
	}
}

void* AENodeAuxGet(AENode* node){
	return node->aux;
}

void AENodeAuxSet(AENode* node,void* data){
	node->aux=data;
}