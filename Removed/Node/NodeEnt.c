#include "NodeInternal.h"

void AENodeEntDelete(AENode* node){
	AENodeEnt* ent=(AENodeEnt*)node;
	if(ent->renderchildren){
		for(size_t i=0;i<AEListLength(ent->renderchildren);i++)
			AENodeDelete(AEListAsArray(ent->renderchildren,AENode*)[i]);
		AEListDelete(ent->renderchildren);
	}
	if(ent->children){
		for(size_t i=0;i<AEListLength(ent->children);i++)
			AENodeDelete(AEListAsArray(ent->children,AENode*)[i]);
		AEListDelete(ent->children);
	}
}

void AENodeEntRender(AENode* node){
	AENodeEnt* ent=(AENodeEnt*)node;
	
	glPushMatrix();
		AEVec3* position=&(ent->transform.position);
		glTranslatef(position->x,position->y,position->z);
		glRotatef(ent->transform.rotation.x,1,0,0);
		glRotatef(ent->transform.rotation.y,0,1,0);
		glRotatef(ent->transform.rotation.z,0,0,1);
		//float m[16];
		//AEQuatToMatrix4x4(ent->transform.rotationq,m);
		//glMultMatrixf(m);
		
		if(ent->renderchildren) 
			for(size_t i=0;i<AEListLength(ent->renderchildren);i++)
				AENodeRender(AEListAsArray(ent->renderchildren,AENode*)[i]);
	glPopMatrix();
	
	if(ent->children) 
		for(size_t i=0;i<AEListLength(ent->children);i++)
			AENodeRender(AEListAsArray(ent->children,AENode*)[i]);
	
}

void AENodeEntStep(AENode* node,float stepInSeconds){
	AENodeEnt* ent=(AENodeEnt*)node;
	//We don't need this anymore
	/*if(ent->renderchildren)
		for(size_t i=0;i<AEListLength(ent->renderchildren);i++){
			//printf("Stepping renderchild: %s\n",AEListAsArray(ent->renderchildren,AENode*)[i]->name);
			AENodeStep(AEListAsArray(ent->renderchildren,AENode*)[i],stepInSeconds);
		}
	if(ent->children) 
		for(size_t i=0;i<AEListLength(ent->children);i++){
			//printf("Stepping child: %s\n",AEListAsArray(ent->children,AENode*)[i]->name);
			AENodeStep(AEListAsArray(ent->children,AENode*)[i],stepInSeconds);
		}*/
}

void AENodeEntAddChild(AENode* node,AENode* node2){
	AENodeEnt* ent=(AENodeEnt*)node;
	if(node==node2) return;
	
	if(ent->children==NULL) ent->children=AEListNew(AENode*);
	node2=AENodeRetain(node2);
	//Find first open slot, create if no preexisting
	AENode* blank=NULL;
	size_t index=AEListFindIndexOfBytes(ent->children,&blank);
	if(index==0){
		AEListAdd(ent->children,AENode*,node2);
	}
	else{
		AEListAsArray(ent->children,AENode*)[index-1]=node2;
	}
}

size_t AENodeEntChildCount(AENode* node){
	AENodeEnt* ent=(AENodeEnt*)node;
	size_t count=0;
	if(ent->children) count+=AEListLength(ent->children);
	return count;
}

AENode* AENodeEntGetChild(AENode* node,size_t i){
	AENodeEnt* ent=(AENodeEnt*)node;
	int offset=0;
	if(ent->children){
		if(i < (AEListLength(ent->children)+offset)) return AEListAsArray(ent->children,AENode*)[i-offset];
	}
	return NULL;
}

AENode* AENodeEntFindChild(AENode* node,char* name){
	AENodeEnt* ent=(AENodeEnt*)node;
	if(node->name && strcmp(node->name,name)==0) return node;
	if(ent->children) for(size_t i=0;i<AEListLength(ent->children);i++){
		AENode* node2=AEListAsArray(ent->children,AENode*)[i];
		if(AENodeTypesGet(node2->type)->typeThatThisQualifiesAs==AENodeTypeEnt){
			AENode* node3=AENodeEntFindChild(node2,name);
			if(node3) return node3;
		}
	}
	return NULL;
}

void AENodeEntRemoveChild(AENode* node,AENode* node2){
	AENodeEnt* ent=(AENodeEnt*)node;
	if(ent->children) AEListRemove(ent->children,AENode*,node2);
	AENodeDelete(node2);
}

void AENodeEntAddRenderChild(AENode* node,AENode* node2){
	AENodeEnt* ent=(AENodeEnt*)node;
	if(node==node2) return;
	
	if(ent->renderchildren==NULL) ent->renderchildren=AEListNew(AENode*);
	node2=AENodeRetain(node2);
	//Find first open slot, create if no preexisting
	AENode* blank=NULL;
	size_t index=AEListFindIndexOfBytes(ent->renderchildren,&blank);
	if(index==0){
		AEListAdd(ent->renderchildren,AENode*,node2);
	}
	else{
		AEListAsArray(ent->renderchildren,AENode*)[index-1]=node2;
	}
}

size_t AENodeEntRenderChildCount(AENode* node){
	AENodeEnt* ent=(AENodeEnt*)node;
	size_t count=0;
	if(ent->renderchildren) count+=AEListLength(ent->renderchildren);
	return count;
}

AENode* AENodeEntGetRenderChild(AENode* node,size_t i){
	AENodeEnt* ent=(AENodeEnt*)node;
	int offset=0;
	if(ent->renderchildren){
		if(i < (AEListLength(ent->renderchildren)+offset)) return AEListAsArray(ent->renderchildren,AENode*)[i-offset];
	}
	return NULL;
}

AENode* AENodeEntRenderChildFind(AENode* node,char* name){
	AENodeEnt* ent=(AENodeEnt*)node;
	if(node->name && strcmp(node->name,name)==0) return node;
	if(ent->renderchildren) for(size_t i=0;i<AEListLength(ent->renderchildren);i++){
		AENode* node2=AEListAsArray(ent->renderchildren,AENode*)[i];
		if(AENodeTypesGet(node2->type)->typeThatThisQualifiesAs==AENodeTypeEnt){
			AENode* node3=AENodeEntFindChild(node2,name);
			if(node3) return node3;
		}
	}
	return NULL;
}

void AENodeEntRenderChildRemove(AENode* node,AENode* node2){
	AENodeEnt* ent=(AENodeEnt*)node;
	if(ent->renderchildren) AEListRemove(ent->renderchildren,AENode*,node2);
	AENodeDelete(node2);
}

void AENodeEntPositionGet(AENode* node,AEVec3* v3){
	AENodeEnt* ent=(AENodeEnt*)node;
	*v3=ent->transform.position;
}

void AENodeEntPositionSet(AENode* node,float x,float y,float z){
	AENodeEnt* ent=(AENodeEnt*)node;
	AEVec3 v3={x,y,z};
	ent->transform.position=v3;
}

void AENodeEntVelocityGet(AENode* node,AEVec3* v3){
	AENodeEnt* ent=(AENodeEnt*)node;
}

void AENodeEntVelocitySet(AENode* node,float x,float y,float z){
	AENodeEnt* ent=(AENodeEnt*)node;
}

	//Radial (Rotational) versions, obviously these have more correct names, just that this is more consistent

void AENodeEntRotationGetQuaternion(AENode* node,AEQuat* q4){
	AENodeEnt* ent=(AENodeEnt*)node;
	*q4=ent->transform.rotationq;
}

void AENodeEntRotationSetQuaternion(AENode* node,float x,float y,float z,float w){
	AENodeEnt* ent=(AENodeEnt*)node;
	AEQuat q4={x,y,z,w};
	ent->transform.rotationq=q4;
}

/*
*/

void AENodeEntRotationGet(AENode* node,AEVec3* v3){
	AENodeEnt* ent=(AENodeEnt*)node;
	*v3=ent->transform.rotation;//AEQuatToEuler(ent->transform.rotationq);
}

void AENodeEntRotationSet(AENode* node,float x,float y,float z){
	AENodeEnt* ent=(AENodeEnt*)node;
	ent->transform.rotation=AEVec3FromCoords(x,y,z);
	AEQuat rotation=AEQuatFromEuler(AEVec3FromCoords(x,y,z));
	AENodeEntRotationSetQuaternion((AENode*)ent,rotation.x,rotation.y,rotation.z,rotation.w);
}

void AENodeEntSpinGet(AENode* node,AEVec3* v3){
	AENodeEnt* ent=(AENodeEnt*)node;
}

void AENodeEntSpinSet(AENode* node,float x,float y,float z){
	AENodeEnt* ent=(AENodeEnt*)node;
}

//Bounding sphere for occlusion culling
float AENodeEntRadiusGet(AENode* node){
	AENodeEnt* ent=(AENodeEnt*)node;
	return ent->radius;
}

void AENodeEntRadiusSet(AENode* node,float radius){
	AENodeEnt* ent=(AENodeEnt*)node;
	ent->radius=radius;
}