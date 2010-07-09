#include "Node Internal.h"

void AENodeEntConnect(unsigned int nodeID,unsigned int node2ID){
	if(nodeID==0) return;
	if(node2ID==0) return;
	if(nodeID==node2ID) return;
	AENode* node=AENodeGetForID(nodeID);
	AENode* node2=AENodeGetForID(node2ID);
	if(node->type!=AENodeTypeEnt) return;
	if(node->nodeIDList==NULL) node->nodeIDList=AEListNew(unsigned int);
	unsigned int blank=0;
	size_t index=AEListFindIndexOfBytes(node->nodeIDList,&blank);
	if(index==0) AEListAdd(node->nodeIDList,node2ID);
	else AEListAsArray(node->nodeIDList,unsigned int)[index-1]=node2ID;
	AENodeRetain(node2ID);
}

void AENodeEntDisconnect(unsigned int nodeID,unsigned int node2ID){
	if(nodeID==0) return;
	if(node2ID==0) return;
	AENode* node=AENodeGetForID(nodeID);
	if(nodeID==node2ID){
		//If you're trying to disconnect yourself from yourself, then that's called beheading, so delete this node, note that this should never be called by anything other than AENodeDelete, which will also free() it for you
		for(unsigned int i=0;i<AEListLength(node->nodeIDList);i++)
			AENodeEntDisconnect(nodeID,AEListAsArray(node->nodeIDList)[i]);
		if(node->body) dBodyDestroy(node->body);
		return;
	}
	AENodeDelete(node2ID);
}

//Render action is bind/render whatever, depends
void AENodeEntRenderAction(unsigned int nodeID){
	if(nodeID==0) return;
	AENode* node=AENodeGetForID(nodeID);
	glPushMatrix();
		glTranslatef(node->data.ent.position.x,node->data.ent.position.y,node->data.ent.position.z);
		/*glRotatef(node.data.ent->rotation.x,	1,0,0);
		glRotatef(node.data.ent->rotation.y,	0,1,0);
		glRotatef(node.data.ent->rotation.z,	0,0,1);*/
		float m4x4[16];
		AEQuatToMatrix4x4(node->data.ent.rotation,m4x4);
		glMultMatrix(m4x4);
		
		for(unsigned int i=0;i<AEListLength(node->nodeIDList);i++)
			AENodeRenderAction(AEListAsArray(node->nodeIDList)[i]);
		
	glPopMatrix();
}