#include "Node Internal.h"

AEList* AENodes=NULL;

unsigned int AENodeRetain(unsigned int nodeID){
	if(nodeID){
		AENode* node=AENodeGetForID(nodeID);
		node->refcount++;
	}
	return nodeID;
}

unsigned int AENodeNew(unsigned char type,...){
	if(AENodes==NULL) AENodes=AEListNew(AENode*);
	unsigned int index=0;
	if(AEListLength(AENodes)!=0){
		AENode* pointer=NULL;
		index=AEListFindIndexOfBytes(AENodes,&pointer);
		//Look for the first empty slot
		
		if(index==0) index=0;
		//It doesn't have an empty slot, leave it at zero so it creates a new one
	}
	
	//Initialize node
	AENode* node=calloc(1,sizeof(AENode));
	node->type=type;
	switch(node->type){
	////////////  <><><><><><><><><><><><><><><><>	To do
		default:
			break;
	}
	
	//Now add the node to the node list.  If we have a presupplied-non-zero index, then we use that, else add to end
	if(index){
		//Since arrays start from 0, subtract 1
		AEListAsArray(AENodes,AENode*)[index-1]=node;
	}
	else{
		AEListAdd(AENodes,node);
		index=AEListLength(AENodes);//Length is always +1 from the actual offset of the last element
	}
	
	return AENodeRetain(index);//Index is the nodeID
}

void AENodeDelete(unsigned int nodeID){
	if(nodeID==0) return;
	//Can't do anything about a null id
	
	AENode* node=AENodeGetForID(nodeID);
	
	//First check to see if the reference count is positive, return if so
	if((node->refcount--) > 0) return;
	
	
	//Delete children as well
	
	//Delete the node, and clear the location
	free(node);
	AEListAsArray(AENodes,AENode*)[nodeID]=NULL;
	
	////////////  <><><><><><><><><><><><><><><><>	To do
}

void AENodeConnect(unsigned int nodeID,unsigned int node2ID){
	if(nodeID==0) return 0;
	if(node2ID==0) return 0;
	AENode* node=AENodeGetForID(nodeID);
	switch(AENodeTypeGet(node)){
		case AENodeTypeNone: break;
		case AENodeTypeEnt: AENodeEntConnect(nodeID,node2ID); break;
		//case AENodeTypeBody: AENodeBodyConnect(nodeID,node2ID); break;
		case AENodeTypeMesh: AENodeMeshConnect(nodeID,node2ID); break;
		case AENodeTypeMaterial: AENodeMaterialConnect(nodeID,node2ID); break;
		//case AENodeTypeCluster: AENodeClusterConnect(nodeID,node2ID); break;
		case AENodeTypeCamera: AENodeCameraConnect(nodeID,node2ID); break;
		case AENodeTypeLight: AENodeLightConnect(nodeID,node2ID); break;
	}
}

void AENodeDisconnect(unsigned int nodeID,unsigned int node2ID){

}

unsigned char AENodeTypeGet(unsigned int nodeID){
	if(nodeID==0) return 0;
	return AENodeGetForID(nodeID)->type;
}

float AENodeTimeLast=0;
float AENodeTimeCurrent=0;
float AENodeTimeStep=0;

void AENodeTimeUpdate(float stepInSeconds){
	AENodeTimeLast=AENodeTimeCurrent;
	AENodeTimeStep=stepInSeconds;
	AENodeTimeCurrent+=AENodeTimeStep;
}