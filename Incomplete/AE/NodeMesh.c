#include "Node Internal.h"

void AENodeMeshFormatSet(unsigned int nodeID,char* format,char* type){
	if(nodeID==0) return;
	AENode* node=AENodeGetForID(nodeID);
	
	node->type=AENodeTypeMesh;
	node->vbo=AEVBONew(format,type);
}
	//VBO format type "v" is plain mesh data, useful for collision
	
void AENodeMeshVertSet(unsigned int nodeID,unsigned int vindex,float* data);
void AENodeMeshVertGet(unsigned int nodeID,unsigned int vindex,float* data);
size_t AENodeMeshVertSizeInNumberOfFloatsGet(unsigned int nodeID,char type);


void AENodeMeshClear(unsigned int nodeID){
	if(nodeID==0) return;
	if(node2ID==0) return;
	AENode* node=AENodeGetForID(nodeID);
	if(nodeID==node2ID){
		//If you're trying to disconnect yourself from yourself, then that's called beheading, so delete this node.  Note that this should never be called by anything other than AENodeDelete, which will also free() it for you
		for(unsigned int i=0;i<AEListLength(node->nodeIDList);i++)
			AENodeEntDisconnect(nodeID,AEListAsArray(node->nodeIDList)[i]);
		if(node->body) dBodyDestroy(node->body);
		AEVBODelete(node->vbo);
		return;
	}
}

//Render action is bind/render whatever, depends
void AENodeMeshRenderAction(unsigned int nodeID){
	if(nodeID==0) return;
	AENode* node=AENodeGetForID(nodeID);
	AEVBOBind(node->data.mesh.vbo);
	AEVADraw(node->data.mesh.vbo);
}