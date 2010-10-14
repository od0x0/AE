#include "NodeInternal.h"

//Internal utility, a memory/speed optimization
typedef struct{
	unsigned int refcount;
	char name[32];
	struct AENodeType* parent;
	size_t instanceSize;
	void (*init)(AENode* node);
	void (*step)(AENode* node,float stepInSeconds);
	void (*render)(AENode* node);
	void (*deinit)(AENode* node);
	AETable* messages;//of AENodeMessageFunc, Inherited by all of type
}AENodeType;

AENodeType* AENodeTypeNew(AENodeType* parent);
AENodeType* AENodeTypeRetain(AENodeType* type);
void AENodeTypeDelete(AENodeType* type);

unsigned int AENodesTime=0;

AEList* AENodes=NULL;

AENode* AENodeRoot=NULL;

//The code here is pretty straightforward, it abstracts the lower level stuff

void AENodesInit(void){
	AENodeTypes=AEListNew(AENodeType);
	AENodeType type={"",AENodeTypeUnknown,0,NULL,NULL,NULL,NULL};
	
	strcpy(type.name,"Entity");
	type.typeThatThisQualifiesAs=AENodeTypeEnt;
	type.instanceSize=sizeof(AENodeEnt);
	type.init=NULL;
	type.step=NULL;
	type.render=AENodeEntRender;
	type.deinit=AENodeEntDelete;
	AENodeTypesAdd(&type);
	
	strcpy(type.name,"Mesh");
	type.typeThatThisQualifiesAs=AENodeTypeMesh;
	type.instanceSize=sizeof(AENodeMesh);
	type.init=NULL;
	type.step=NULL;
	type.render=AENodeMeshRender;
	type.deinit=AENodeMeshDelete;
	AENodeTypesAdd(&type);
	
	strcpy(type.name,"Material");
	type.typeThatThisQualifiesAs=AENodeTypeMaterial;
	type.instanceSize=sizeof(AENodeMaterial);
	type.init=NULL;
	type.step=NULL;
	type.render=AENodeMaterialRender;
	type.deinit=AENodeMaterialDelete;
	AENodeTypesAdd(&type);
}

void AENodesQuit(void){
	AEListDelete(AENodeTypes);
	AENodeDelete(AENodeRoot);
}

//Ideally, xxxStep should be called with constant stepsize
//We could perform some crazy stuff here, like threading
void AENodesStep(float stepInSeconds){
	AENodesTime+=stepInSeconds*1000.0f;
	if(AENodes)for(size_t i=0;i<AEListLength(AENodes);i++) {
		AENode* node=AEListAsArray(AENodes,AENode*)[i];
		if(node) AENodeStep(node,stepInSeconds);
	}
}

void AENodesRender(void){
	AENodeRender(AENodeRoot);
}

void AENodeRootSet(AENode* node){
	AENodeRoot=node;
}

AENode* AENodeRootGet(void){
	return AENodeRoot;
}

AENode* AENodesFind(char* name){
	if(AENodes) for(size_t i=0;i<AEListLength(AENodes);i++){
		AENode* node=AEListAsArray(AENodes,AENode*)[i];
		if(node && node->name && strcmp(name, node->name)==0) return node;
	}
	return NULL;
}

int AENodeTypeGet(AENode* node){
	if(node==NULL) AEError("Passed a null.");
	return node->type;
}

char* AENodeNameGet(AENode* node){return node->name;}

void AENodeNameSet(AENode* node,char* name){
	free(node->name);
	node->name=name;
}

AENode* AENodeRetain(AENode* node){node->refcount++;return node;}

AENode* AENodeNew(unsigned int type,char* name){
	if(type==0) return NULL;
	size_t size=AENodeTypesGet(type)->instanceSize;
	//Start off with getting size first, to cut off redundant code
	/*switch(type) {
		case AENodeTypeEnt: size=sizeof(AENodeEnt); break;
		case AENodeTypeMesh: size=sizeof(AENodeMesh); break;
		case AENodeTypeMaterial: size=sizeof(AENodeMaterial); break;
	}*/
	//printf("%s(): Made it to line #%i\n",__func__,__LINE__);
	AENode* node=calloc(1,size);
	node->name=strdup(name);
	node->type=type;
	//init the memory if it must, or handle post-allocation things
	switch(type) {
		case AENodeTypeEnt:{
			if(AENodeRootGet()==NULL) AENodeRootSet(node);
			}break;
		case AENodeTypeMesh: break;
		case AENodeTypeMaterial: break;
		default:
			if(AENodeTypesGet(type)->init) (*(AENodeTypesGet(type)->init))(node);
			break;
	}
	
	if(AENodes==NULL) AENodes=AEListNew(AENode*);
	//Find first open slot, create if no preexisting
	AENode* blank=NULL;
	size_t index=AEListFindIndexOfBytes(AENodes,&blank);
	if(index==0){
		AEListAdd(AENodes,AENode*,node);
	}
	else{
		AEListAsArray(AENodes,AENode*)[index-1]=node;
	}

	/*
	if(AENodes==NULL) AENodes=AEListNew(AENode*);
	if(AENodesFind(node)) AEListAdd(AENodes,AENode*,node);*/
	return node;//AENodeRetain(node);
};

void AENodeDelete(AENode* node){
	if(node==NULL) return;
	//Interesting logic so that 0 and 1 are equal
	if(node->refcount){
		node->refcount--;
		if(node->refcount) return;
	}
	switch(node->type) {
		case AENodeTypeEnt: AENodeEntDelete(node); break;
		case AENodeTypeMesh: AENodeMeshDelete(node); break;
		case AENodeTypeMaterial: AENodeMaterialDelete(node); break;
		default:
			if(AENodeTypesGet(node->type)->deinit) (*(AENodeTypesGet(node->type)->deinit))(node);
			break;
	}
	AEListRemove(AENodes,AENode*,node);
	
	free(node->name);
	free(node);
}

void AENodeRender(AENode* node){
	if(node==NULL) return;
	//AENodeType* type=AENodeTypesGet(node->type);
	//printf("Rendering %s of type %s which qualifies as a %s.\n",node->name,type->name,AENodeTypesGet(type->typeThatThisQualifiesAs)->name);
	switch(node->type) {
		case AENodeTypeEnt: AENodeEntRender(node); break;
		case AENodeTypeMesh: AENodeMeshRender(node); break;
		case AENodeTypeMaterial: AENodeMaterialRender(node); break;
		default:
			if(AENodeTypesGet(node->type)->render) (*(AENodeTypesGet(node->type)->render))(node);
			break;
	}
}

void AENodeStep(AENode* node,float stepInSeconds){
	if(node==NULL) return;
	//size_t timestamp=AENodesTime;
	if(node->timestamp==AENodesTime) return;
	node->timestamp=AENodesTime;
	switch(node->type) {
		/*case AENodeTypeEnt: AENodeEntStep(node,stepInSeconds); break;
		case AENodeTypeMesh: AENodeMeshStep(node,stepInSeconds); break;
		case AENodeTypeMaterial: AENodeMaterialStep(node,stepInSeconds); break;*/
		default:
			if(AENodeTypesGet(node->type)->step) (*(AENodeTypesGet(node->type)->step))(node,stepInSeconds);
			break;
	}
}

void AENodeMessageSet(AENode* node,char* name,AENodeMessageFunc fn){
	AENodeType* type=node->type;
	if(type->messages) type->messages=AETableNew();
	AETableSet(node->messages, name, fn);
}

AENodeMessageFunc AENodeMessageGet(AENode* node,char* name){
	AENodeMessageFunc fn=NULL;
	if(type && type->messages) fn=AETableGet(type->messages, name);
	if(fn==NULL){
		printf("AENodeTypeMessagesGet(): Node Type \"%s\" does not have a message called \"%s\".\n",type->name,messageName);
	}
	return fn;
}