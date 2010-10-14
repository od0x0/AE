#include "NodeInternal.h"

void AENodeEntDelete(AENode* node){
	AENodeEnt* ent=(AENodeEnt*)node;
	/*if(ent->body){
		switch(AEPTypeGet((void*)ent->body)){
			case AEPTypeEnv:
				AEPEnvDelete((AEPEnv*)ent->body);
				break;
			case AEPTypeBody:
				AEPBodyDelete(ent->body);
				break;
		}
	}*/
	//dBodyDestroy(ent->body);
	if(ent->children) 
		for(size_t i=0;i<AEListLength(ent->children);i++)
			AENodeDelete(AEListAsArray(ent->children,AENode*)[i]);
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
	/*if(ent->body){
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
	}*/
	if(ent->body){
		AEVec3 position={0,0,0};
		AENodeEntPositionGet((AENode*)ent,&position);
		AEQuat rotation={0,0,0,1};
		AENodeEntRotationGet((AENode*)ent,&rotation);
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
			ent2->world=ent->world;
			//Find first open slot, create if no preexisting
			AENode* blank=NULL;
			size_t index=AEListFindIndexOfBytes(ent->children,&blank);
			if(index==0){
				AEListAdd(ent->children,AENode*,AENodeRetain(node2));
				/*if(AEPTypeGet(ent->body)==AEPTypeEnv || AEPTypeGet(ent2->body)==AEPTypeBody){
					AEPEnvAddBody((AEPEnv*)ent->body,ent2->body);
					puts("Adding it");
				}*/
			}
			else{
				AEListAsArray(ent->children,AENode*)[index-1]=node2;
				/*if(AEPTypeGet(ent->body)==AEPTypeEnv || AEPTypeGet(ent2->body)==AEPTypeBody){
					AEPEnvAddBody((AEPEnv*)ent->body,ent2->body);
					puts("Adding it");
				}*/
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
		case AENodeTypeWorld:
			AEError("AENodeWorld cannot be a non-root node, and thus may not be a child.");
			break;
	}
}

void AENodeEntPositionGet(AENode* node,AEVec3* v3){
	AENodeEnt* ent=(AENodeEnt*)node;
	if(ent->body){
		const dReal* odev3=dBodyGetPosition(ent->body);
		AEVec3 v={odev3[0],odev3[1],odev3[2]};
		ent->transform.position=v;
	}
	*v3=ent->transform.position;
}

void AENodeEntPositionSet(AENode* node,float x,float y,float z){
	AENodeEnt* ent=(AENodeEnt*)node;
	if(ent->body) dBodySetPosition(ent->body,x,y,z);
	//AEPBodyPositionSet(ent->body,x,y,z);
	AEVec3 v3={x,y,z};
	ent->transform.position=v3;
}

void AENodeEntVelocityGet(AENode* node,AEVec3* v3){
	AENodeEnt* ent=(AENodeEnt*)node;
	if(ent->body){
		const dReal* odev3=dBodyGetLinearVel(ent->body);
		v3->x=odev3[0];
		v3->y=odev3[1];
		v3->z=odev3[2];
	}
	//AEPBodyVelocityGet(ent->body,v3);
}

void AENodeEntVelocitySet(AENode* node,float x,float y,float z){
	AENodeEnt* ent=(AENodeEnt*)node;
	if(ent->body) dBodySetLinearVel(ent->body,x,y,z);
}

	//Radial (Rotational) versions, obviously these have more correct names, just that this is more consistent

void AENodeEntRotationGet(AENode* node,AEQuat* q4){
	AENodeEnt* ent=(AENodeEnt*)node;
	if(ent->body){
		const dReal* odeq4=dBodyGetQuaternion(ent->body);
		ent->transform.rotation.x=odeq4[1];
		ent->transform.rotation.y=odeq4[2];
		ent->transform.rotation.z=odeq4[3];
		ent->transform.rotation.w=odeq4[0];
	}
	*q4=ent->transform.rotation;
}

void AENodeEntRotationSet(AENode* node,float x,float y,float z,float w){
	AENodeEnt* ent=(AENodeEnt*)node;
	AEQuat q4={x,y,z,w};
	if(ent->body){
		dQuaternion odeq4={w,x,y,z};
		dBodySetQuaternion(ent->body,odeq4);
		q4.x=odeq4[1];
		q4.y=odeq4[2];
		q4.z=odeq4[3];
		q4.w=odeq4[0];
	}
	ent->transform.rotation=q4;
}

void AENodeEntSpinGet(AENode* node,AEVec3* v3){
	AENodeEnt* ent=(AENodeEnt*)node;
	if(ent->body){
		const dReal* odev3=dBodyGetAngularVel(ent->body);
		v3->x=odev3[0];
		v3->y=odev3[1];
		v3->z=odev3[2];
	}
	//if(ent->body) AEPBodySpinGet(ent->body,v3);
}

void AENodeEntSpinSet(AENode* node,float x,float y,float z){
	AENodeEnt* ent=(AENodeEnt*)node;
	if(ent->body) dBodySetAngularVel(ent->body,x,y,z);
	//if(ent->body) AEPBodySpinSet(ent->body,x,y,z);
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

/*void AENodeEntBodySet(AENode* node,AEPBody* body){	
	AENodeEnt* ent=(AENodeEnt*)node;
	ent->body=body;
}

AEPBody* AENodeEntBodyGet(AENode* node){
	AENodeEnt* ent=(AENodeEnt*)node;
	return ent->body;
}*/

void AENodeEntBodySet(AENode* node,dBodyID body){
	AENodeEnt* ent=(AENodeEnt*)node;
	ent->body=body;
	if(ent->body) dBodySetData(ent->body,ent);
}

dBodyID AENodeEntBodyGet(AENode* node){
	AENodeEnt* ent=(AENodeEnt*)node;
	return ent->body;
}

#define AENodeEntBodyPropertyBounciness 1
#define AENodeEntBodyPropertyBounceVelocity 2
#define AENodeEntBodyPropertyFriction 3
#define AENodeEntBodyPropertyMovable 4

void AENodeEntBodyPropertySet(AENode* node,int flag,float value){
	AENodeEnt* ent=(AENodeEnt*)node;
	if(ent->body==0) ent->body=dBodyCreate(ent->world->odeworld);
	dBodySetData(ent->body,ent);
	switch (flag) {
		case AENodeEntBodyPropertyBounciness:
			ent->bodyProperties.bounciness=value;
			break;
		case AENodeEntBodyPropertyBounceVelocity:
			ent->bodyProperties.bounceVelocity=value;
			break;
		case AENodeEntBodyPropertyFriction:
			ent->bodyProperties.friction=value;
			break;
		case AENodeEntBodyPropertyMovable:
			ent->bodyProperties.immovable=!value;
			break;
		default:
			AEError("Unknown flag set.\n");
			break;
	}
}

float AENodeEntBodyPropertyGet(AENode* node,int flag){
	AENodeEnt* ent=(AENodeEnt*)node;
	switch (flag) {
		case AENodeEntBodyPropertyBounciness:
			return ent->bodyProperties.bounciness;
		case AENodeEntBodyPropertyBounceVelocity:
			return ent->bodyProperties.bounceVelocity;
		case AENodeEntBodyPropertyFriction:
			return ent->bodyProperties.friction;
		case AENodeEntBodyPropertyMovable:
			return !ent->bodyProperties.immovable;
		default:
			AEError("Unknown flag accessed.\n");
			break;
	}
	return (1.0/0.0)/(1.0/0.0);//NaN
}

/*case AENodeEntBodyPropertyMass:{
			AEVec3 size={values[0],values[1],values[2]};
			float massamount=values[3];
			dMass mass;
			dBodyGetMass(ent->body,&mass);
			if(massamount) dBodySetGravityMode(ent->body,1);
			else{
				dMassSetZero(&mass);
				dBodySetGravityMode(ent->body,0);
				return;
			}
			dMassSetBoxTotal(&mass,size.x,size.y,size.z,massamount);
			dBodySetMass(ent->body,&mass);
			}break;*/