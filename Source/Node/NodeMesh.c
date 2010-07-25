#include "../Node.h"

void AENodeMeshDelete(AENode* node){
	AENodeMesh* mesh=(AENodeMesh*)node;
	AEVBODelete(mesh->data.vbo);
}

void AENodeMeshRender(AENode* node){
	AENodeMesh* mesh=(AENodeMesh*)node;
	AEVBOBind(mesh->data.vbo);
	AEVBODraw(mesh->data.vbo);
}

void AENodeMeshStep(AENode* node,float stepInSeconds){
	AENodeMesh* mesh=(AENodeMesh*)node;
}

void AENodeMeshVBOSet(AENode* node,AEVBO* vbo){
	AENodeMesh* mesh=(AENodeMesh*)node;
	mesh->data.vbo=vbo;
}

AEVBO* AENodeMeshVBOGet(AENode* node){
	AENodeMesh* mesh=(AENodeMesh*)node;
	return mesh->data.vbo;
}