#include "Physics.h"

static dWorldID AEODEWorld;
static dJointGroupID AEODEContacts;

void AEPNearCallback (void *data, dGeomID o1, dGeomID o2);

typedef struct{
	void* data;
	char type;
}AEObjectHeader;

struct AEPEnv{
	AEObjectHeader header;
	dSpaceID space;
	AEPEnv* parent;
};

struct AEPBody{
	AEObjectHeader header;
	AEPEnv* env;
	AEPBody* parent;
	dMass mass;
	dBodyID body;
	dGeomID geom;
	dTriMeshDataID trimeshdata;
	int (*collideCallback)(AEPBody*,AEPBody*);
	float bounciness,bounceVelocity,friction;
	char immovable;
};

static AEPEnv* AEPEnvNew(AEPEnv* parent,int type,void* arg){
	AEPEnv* env=calloc(1,sizeof(AEPEnv));
	env->header.type=AEPTypeEnv;
	dSpaceID space=0;
	if(parent) space=parent->space;
	env->parent=parent;
	switch(type){
		case AEPEnvTypeSimple:
			env->space=dSimpleSpaceCreate(space);
			break;
		case AEPEnvTypeHash:
			env->space=dHashSpaceCreate(space);
			dHashSpaceSetLevels(env->space,((int*)arg)[0],((int*)arg)[1]);
			break;
		case AEPEnvTypeLarge:{
			float* array=arg;
			dVector3 center={array[0],array[1],array[2]};
			dVector3 extents={array[3],array[4],array[5]};
			int depth=array[7];
			env->space=dQuadTreeSpaceCreate(space,center,extents,depth);
			}break;
	}
	dSpaceSetCleanup(env->space,0);
	return env;
}

AEPEnv* AEPEnvNewSimple(AEPEnv* parent){
	return AEPEnvNew(parent,AEPEnvTypeSimple,NULL);
}

AEPEnv* AEPEnvNewHash(AEPEnv* parent, int minlevels, int maxlevels){
	int array[2]={minlevels,maxlevels};
	return AEPEnvNew(parent,AEPEnvTypeHash,array);
}

AEPEnv* AEPEnvNewLarge(AEPEnv* parent,AEVec3* center, AEVec3* size, int depth){
	float array[7]={center->x, center->y, center->z,  size->x, size->y, size->z,  depth};
	return AEPEnvNew(parent,AEPEnvTypeLarge,array);
}

AEPEnv* AEPEnvParentGet(AEPEnv* env){
	if(AEPTypeGet(env)!=AEPTypeEnv) return NULL;
	return env->parent;
}

void AEPEnvAddBody(AEPEnv* env,AEPBody* body){
	if(AEPTypeGet(env)!=AEPTypeEnv) return;
	if(body->env) AEPEnvRemoveBody(body->env,body);
	body->env=env;
	dSpaceAdd(env->space,body->geom);
}

void AEPEnvRemoveBody(AEPEnv* env,AEPBody* body){
	if(AEPTypeGet(body)!=AEPTypeEnv) return;
	body->env=NULL;
	dSpaceRemove(env->space,body->geom);
}

void AEPEnvCheck(AEPEnv* env){
	if(AEPTypeGet(env)!=AEPTypeEnv) return;
	dSpaceCollide(env->space,NULL,AEPNearCallback);
}

void AEPEnvCheckWithOther(AEPEnv* env,AEPEnv* env2){
	if(AEPTypeGet(env)!=AEPTypeEnv) return;
	if(AEPTypeGet(env2)!=AEPTypeEnv) return;
	dSpaceCollide2((dGeomID)env->space,(dGeomID)env2->space,NULL,AEPNearCallback);
}

void AEPEnvDelete(AEPEnv* env){
	if(AEPTypeGet(env)!=AEPTypeEnv) return;
	int length=dSpaceGetNumGeoms(env->space);
	for(int i=0;i<length;i++){
		AEPBodyDelete(dGeomGetData(dSpaceGetGeom(env->space,i)));
	}
	dSpaceDestroy(env->space);
	free(env);
}

AEPBody* AEPBodyNew(int (*collideCallback)(AEPBody*,AEPBody*),int type,void* arg){
	AEPBody* body=calloc(1,sizeof(AEPBody));
	body->header.type=AEPTypeBody;
	body->collideCallback=collideCallback;
	
	dMassSetZero(&body->mass);
	body->body=dBodyCreate(AEODEWorld);
	//dBodySetMass(body->body,&body->mass);
	
	switch(type){
		case AEPGeomTypeSphere:
			body->geom=dCreateSphere(0,((float*)arg)[0]);
			break;
		case AEPGeomTypeBox:
			body->geom=dCreateBox(0,((float*)arg)[0],((float*)arg)[1],((float*)arg)[2]);
			break;
		case AEPGeomTypeMesh:{
			body->trimeshdata=dGeomTriMeshDataCreate();
			AEPMesh* mesh=arg;
			
			/*dReal* verts=malloc(sizeof(dReal)*3*mesh->vcount);
			for(unsigned int i=0;i < mesh->vcount*3;i++){
				verts[i+0]=mesh->vertices[i].x;
				verts[i+1]=mesh->vertices[i].y;
				verts[i+2]=mesh->vertices[i].z;
			}*/
			dGeomTriMeshDataBuildSimple(body->trimeshdata,mesh->vertices,mesh->vcount,mesh->indices,mesh->icount);
			body->geom=dCreateTriMesh(0,body->trimeshdata,NULL,NULL,NULL);
			dGeomSetData(body->geom,mesh);
			}break;
	}
	
	body->friction=dInfinity;
	
	//dBodySetGravityMode(body->body,0);
	
	dGeomSetBody(body->geom,body->body);
	dBodySetData(body->body,body);
	
	return body;
}

void AEPBodyAddChild(AEPBody* body,AEPBody* body2){
	if(AEPTypeGet(body)!=AEPTypeBody) return;
	AEPEnv* env=body->env;
	if(env==NULL) env=AEPEnvNew(AEPBodyEnvGet(body->parent),AEPEnvTypeSimple,NULL);
	AEPEnvAddBody(env,body2);
	body->env=env;
}

void AEPBodyRemoveChild(AEPBody* body,AEPBody* body2){
	if(AEPTypeGet(body)!=AEPTypeBody) return;
	if(body->env==NULL) return;
	AEPEnvRemoveBody(body->env,body2);
}

AEPBody* AEPBodyParentGet(AEPBody* body){
	if(AEPTypeGet(body)!=AEPTypeBody) return NULL;
	return body->parent;
}

void AEPBodyMassSet(AEPBody* body,AEVec3 size,float mass){
	if(AEPTypeGet(body)!=AEPTypeBody) return;
	if(mass) dBodySetGravityMode(body->body,1);
	else{
		dMassSetZero(&body->mass);
		dBodySetGravityMode(body->body,0);
		return;
	}
	dMassSetBoxTotal(&body->mass,size.x,size.y,size.z,mass);
	dMassAdjust(&body->mass,mass);
	body->mass.mass=1;
	dBodySetMass(body->body,&body->mass);
}

void AEPBodyPositionGet(AEPBody* body,AEVec3* v3){
	if(AEPTypeGet(body)!=AEPTypeBody) return;
	const dReal* v4=dBodyGetPosition(body->body);
	//printf("%lf %lf %lf \n",v4[0],v4[1],v4[2]);
	//exit(0);
	v3->x=v4[0];
	v3->y=v4[1];
	v3->z=v4[2];
}

void AEPBodyPositionSet(AEPBody* body,float x,float y,float z){
	if(AEPTypeGet(body)!=AEPTypeBody) return;
	dBodySetPosition(body->body,x,y,z);
}

void AEPBodyVelocityGet(AEPBody* body,AEVec3* v3){
	if(AEPTypeGet(body)!=AEPTypeBody) return;
	const dReal* v4=dBodyGetLinearVel(body->body);
	v3->x=v4[0];
	v3->y=v4[1];
	v3->z=v4[2];
}

void AEPBodyVelocitySet(AEPBody* body,float x,float y,float z){
	if(AEPTypeGet(body)!=AEPTypeBody) return;
	dBodySetLinearVel(body->body,x,y,z);
}

//#warning You may need to take into account ODE's w,x,y,z order for quaternions


// This code swizzles from ODE's w,x,y,z to our x,y,z,w

void AEPBodyRotationGet(AEPBody* body,AEQuat* q4){
	if(AEPTypeGet(body)!=AEPTypeBody) return;
	const dReal* odeq4=dBodyGetQuaternion(body->body);
	q4->x=odeq4[1];
	q4->y=odeq4[2];
	q4->z=odeq4[3];
	q4->w=odeq4[0];
}

void AEPBodyRotationSet(AEPBody* body,float x,float y,float z,float w){
	if(AEPTypeGet(body)!=AEPTypeBody) return;
	dQuaternion odeq4={w,x,y,z};
	dBodySetQuaternion(body->body,odeq4);
}

void AEPBodySpinGet(AEPBody* body,AEVec3* v3){
	if(AEPTypeGet(body)!=AEPTypeBody) return;
	const dReal* v4=dBodyGetAngularVel(body->body);
	v3->x=v4[0];
	v3->y=v4[1];
	v3->z=v4[2];
}

void AEPBodySpinSet(AEPBody* body,float x,float y,float z){
	if(AEPTypeGet(body)!=AEPTypeBody) return;
	dBodySetAngularVel(body->body,x,y,z);
}

AEPEnv* AEPBodyEnvGet(AEPBody* body){
	if(AEPTypeGet(body)!=AEPTypeBody) return NULL;
	return body->env;
}


int AEPBodyTestRay(AEPBody* body,AEVec3* start,AEVec3* location){
	return 0;
}

void AEPBodyFlagSet(AEPBody* body,int flag,float value){
	if(AEPTypeGet(body)!=AEPTypeBody) return;
	switch (flag) {
		case AEPBodyFlagFriction:
			body->friction=value;
			break;
		case AEPBodyFlagBounciness:
			body->bounciness=value;
			break;
		case AEPBodyFlagBounceVelocity:
			body->bounceVelocity=value;
			break;
		case AEPBodyFlagMovable:
			body->immovable=!value;
			dBodySetGravityMode(body->body,value);
			break;
	}
}

float AEPBodyFlagGet(AEPBody* body,int flag){
	if(AEPTypeGet(body)!=AEPTypeBody) return (1.0/0.0)/(1.0/0.0);
	switch (flag) {
		case AEPBodyFlagFriction:
			return body->friction;
			break;
		case AEPBodyFlagBounciness:
			return body->bounciness;
			break;
		case AEPBodyFlagBounceVelocity:
			return body->bounceVelocity;
			break;
		case AEPBodyFlagMovable:
			return !body->immovable;
			break;
	}
	return (1.0/0.0)/(1.0/0.0);
}

void AEPBodyCenterOfGravitySet(AEPBody* body,float x,float y,float z){
	if(AEPTypeGet(body)!=AEPTypeBody) return;
	dMassTranslate(&body->mass,x,y,z);
}

void AEPBodyDelete(AEPBody* body){
	if(AEPTypeGet(body)!=AEPTypeBody) return;
	if(body->trimeshdata){
		dGeomTriMeshDataDestroy(body->trimeshdata);
		AEPMesh* mesh=dGeomGetData(body->geom);
		free(mesh->vertices);
		free(mesh->indices);
		free(mesh);
	}
	
	dGeomDestroy(body->geom);
	dBodyDestroy(body->body);
	free(body);
}

void AEPBodyCollisionCallbackSet(AEPBody* body,int (*collideCallback)(AEPBody*,AEPBody*)){
	if(AEPTypeGet(body)!=AEPTypeBody) return;
	body->collideCallback=collideCallback;
}

////////////////////////////

void AEPNearCallback (void *data, dGeomID o1, dGeomID o2){
	
	if (dGeomIsSpace (o1) || dGeomIsSpace (o2)){
		// colliding a space with something
		dSpaceCollide2 (o1,o2,data,&AEPNearCallback);
		// collide all geoms internal to the space(s)
		if (dGeomIsSpace (o1)) dSpaceCollide ((dSpaceID)o1,data,&AEPNearCallback);
		if (dGeomIsSpace (o2)) dSpaceCollide ((dSpaceID)o2,data,&AEPNearCallback);
	}
	else{
		static const int MAX_CONTACTS = 3;
		dContact contacts[MAX_CONTACTS];
		
		dBodyID b1 = dGeomGetBody(o1);
		dBodyID b2 = dGeomGetBody(o2);
		AEPBody* pb1=dBodyGetData(b1);
		AEPBody* pb2=dBodyGetData(b2);
		
		const float bounciness=(pb1->bounciness+pb2->bounciness)*0.5;
		const float bounceVelocity=(pb1->bounceVelocity+pb2->bounceVelocity)*0.5;
		const float friction=(pb1->friction+pb2->friction)*0.5;
		
		for (int i = 0; i < MAX_CONTACTS; i++){
			//Should really set to the average of the bodies' surfaces
			
			contacts[i].surface.mode = (bounciness ? dContactBounce : 0) | dContactSoftCFM;
			contacts[i].surface.mu = friction;
			contacts[i].surface.mu2 = 0;
			contacts[i].surface.bounce = bounciness;
			contacts[i].surface.bounce_vel = bounceVelocity;
			contacts[i].surface.soft_cfm = 0.01;
		}
		
		int numc=dCollide(o1, o2, MAX_CONTACTS, &contacts[0].geom, sizeof(dContact));
		
		
		if (numc){
			if(pb1->collideCallback && (*pb1->collideCallback)(pb1,pb2)==0)return;
			if(pb2->collideCallback && (*pb2->collideCallback)(pb2,pb1)==0)return;

			// Get the dynamics body for each geom
			// To add each contact point found to our joint group we call dJointCreateContact which is just one of the many
			// different joint types available.  
			for (int i = 0; i < numc; i++){
				// dJointCreateContact needs to know which world and joint group to work with as well as the dContact
				// object itself. It returns a new dJointID which we then use with dJointAttach to finally create the
				// temporary contact joint between the two geom bodies.
				dJointID c = dJointCreateContact(AEODEWorld,AEODEContacts, contacts + i);
				dJointAttach(c, pb1->immovable?0:b1, pb2->immovable?0:b2);
			}
		}
	}
}

int AEPTypeGet(void* object){
	if(object==NULL) return 0;
	AEObjectHeader* header=(AEObjectHeader*)object;
	return header->type;
}

void AEPTypeDataSet(void* object,void* data){
	AEObjectHeader* header=(AEObjectHeader*)object;
	header->data=data;
}

void* AEPTypeDataGet(void* object){
	AEObjectHeader* header=(AEObjectHeader*)object;
	return header->data;
}

void AEPInit(void){
	dInitODE();
	AEODEWorld=dWorldCreate();
	AEODEContacts=dJointGroupCreate(0);
}

void AEPStep(float step){
	dWorldQuickStep(AEODEWorld,step);
	dJointGroupEmpty(AEODEContacts);
}

void AEPGravitySet(float x,float y,float z){
	dWorldSetGravity(AEODEWorld,x,y,z);
}

AEVec3 AEPGravityGet(void){
	dVector3 v4;
	dWorldGetGravity(AEODEWorld,v4);
	AEVec3 v3={v4[0],v4[1],v4[2]};
	return v3;
}

void AEPQuit(void){
	dJointGroupEmpty(AEODEContacts);
	dWorldDestroy(AEODEWorld);
	dCloseODE();
}