#include "../Node.h"

void AENodeEntDelete(AENode* node){
	AENodeEnt* ent=(AENodeEnt*)node;
	if(ent->body){
		switch(AEPTypeGet((void*)ent->body)){
			case AEPTypeEnv:
				AEPEnvDelete((AEPEnv*)ent->body);
				break;
			case AEPTypeBody:
				AEPBodyDelete(ent->body);
				break;
		}
	}
	AENodeDelete(ent->material);
	AENodeDelete(ent->mesh);
}

void AENodeEntRender(AENode* node){
	AENodeEnt* ent=(AENodeEnt*)node;
	glPushMatrix();
		AEVec3* position=&(ent->transform.position);
		glTranslatef(position->x,position->y,position->z);
		float m[16];
		AEQuatToMatrix4x4(ent->transform.rotation,m);
		glMultMatrixf(m);
		
		AENodeRender(ent->material);
		AENodeRender(ent->mesh);
		if(ent->children) 
			for(size_t i=0;i<AEListLength(ent->children);i++)
				AENodeRender(AEListAsArray(ent->children,AENode*)[i]);
	glPopMatrix();
}

void AENodeEntStep(AENode* node,float stepInSeconds){
	AENodeEnt* ent=(AENodeEnt*)node;
	if(ent->body){
		switch(AEPTypeGet((void*)ent->body)){
			case AEPTypeEnv:{
				AEPEnv* env=(AEPEnv*)ent->body;
				AEPEnvCheck(env);
				if(AEPEnvParentGet(env)==NULL) AEPStep(stepInSeconds);
				}break;
			case AEPTypeBody:
				AEPBodyPositionGet(ent->body,&(ent->transform.position));
				break;
		}
	}
	
	if(ent->children) 
		for(size_t i=0;i<AEListLength(ent->children);i++)
			AENodeEntStep(AEListAsArray(ent->children,AENode*)[i],stepInSeconds);
}

void AENodeEntAddChild(AENode* node,AENode* node2){
	AENodeEnt* ent=(AENodeEnt*)node;
	AENodeEnt* ent2=(AENodeEnt*)node2;
	switch(AENodeTypeGet(node2)){
		case AENodeTypeEnt:{
			if(ent->children==NULL) ent->children=AEListNew(AENode*);
			node2=AENodeRetain(node2);
			//Find first open slot, create if no preexisting
			AENode* blank=NULL;
			size_t index=AEListFindIndexOfBytes(ent->children,&blank);
			if(index==0){
				AEListAdd(ent->children,AENode*,AENodeRetain(node2));
				if(AEPTypeGet(ent->body)==AEPTypeEnv || AEPTypeGet(ent2->body)==AEPTypeBody){
					AEPEnvAddBody((AEPEnv*)ent->body,ent2->body);
					puts("Adding it");
				}
			}
			else{
				AEListAsArray(ent->children,AENode*)[index-1]=node2;
				if(AEPTypeGet(ent->body)==AEPTypeEnv || AEPTypeGet(ent2->body)==AEPTypeBody){
					AEPEnvAddBody((AEPEnv*)ent->body,ent2->body);
					puts("Adding it");
				}
			}
			}break;
		case AENodeTypeMesh:
			AENodeDelete(ent->mesh);
			ent->mesh=AENodeRetain(node2);
			break;
		case AENodeTypeMaterial:
			AENodeDelete(ent->material);
			ent->material=AENodeRetain(node2);
			break;
	}
}

void AENodeEntPositionGet(AENode* node,AEVec3* v3){
	AENodeEnt* ent=(AENodeEnt*)node;
	*v3=ent->transform.position;
}

void AENodeEntPositionSet(AENode* node,float x,float y,float z){
	AENodeEnt* ent=(AENodeEnt*)node;
	if(ent->body) AEPBodyPositionSet(ent->body,x,y,z);
	AEVec3 v3={x,y,z};
	ent->transform.position=v3;
}

void AENodeEntVelocityGet(AENode* node,AEVec3* v3){
	AENodeEnt* ent=(AENodeEnt*)node;
	if(ent->body) AEPBodyVelocityGet(ent->body,v3);
}

void AENodeEntVelocitySet(AENode* node,float x,float y,float z){
	AENodeEnt* ent=(AENodeEnt*)node;
	if(ent->body) AEPBodyVelocitySet(ent->body,x,y,z);
}

	//Radial (Rotational) versions, obviously these have more correct names, just that this is more consistent

void AENodeEntRotationGet(AENode* node,AEQuat* q4){
	AENodeEnt* ent=(AENodeEnt*)node;
	*q4=ent->transform.rotation;
}

void AENodeEntRotationSet(AENode* node,float x,float y,float z,float w){
	AENodeEnt* ent=(AENodeEnt*)node;
	AEQuat q4={x,y,z,w};
	ent->transform.rotation=q4;
}

void AENodeEntSpinGet(AENode* node,AEVec3* v3){
	AENodeEnt* ent=(AENodeEnt*)node;
	if(ent->body) AEPBodySpinGet(ent->body,v3);
}

void AENodeEntSpinSet(AENode* node,float x,float y,float z){
	AENodeEnt* ent=(AENodeEnt*)node;
	if(ent->body) AEPBodySpinSet(ent->body,x,y,z);
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

void AENodeEntBodySet(AENode* node,AEPBody* body){	
	AENodeEnt* ent=(AENodeEnt*)node;
	ent->body=body;
}

AEPBody* AENodeEntBodyGet(AENode* node){
	AENodeEnt* ent=(AENodeEnt*)node;
	return ent->body;
}