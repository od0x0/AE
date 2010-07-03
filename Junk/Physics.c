#include "../Physics.h"
#include "ODE/ode/ode.h"

static dWorldID AEODEWorld;
static dJointGroupID AEODEContacts;

static void NearCallback (void *data, dGeomID o1, dGeomID o2);

struct AEPEnv{
	dSpaceID space;
};

struct AEPBody{
	dMass mass;
	dBodyID body;
	dGeomID geom;
	dTriMeshDataID trimeshdata;
	int (*collideCallback)(AEPBody*,AEPBody*);
	void* data;
};


AEPEnv* AEPEnvNew(AEPEnv* parent,int type,void* arg){
	AEPEnv* env=calloc(1,sizeof(AEPEnv));
	dSpaceID space=0;
	if(parent) space=parent->space;
	switch(type){
		case AEPEnvTypeSimple:
			env->space=dSimpleSpaceCreate(space);
			break;
		case AEPEnvTypeHash:
			env->space=dHashSpaceCreate(space);
			dHashSpaceSetLevels(env->space,((int*)arg)[0],((int*)arg)[1]);
			break;
		case AEPEnvTypeLarge:
			//env->space=dQuadTreeSpaceCreate(space,arg,arg+sizeof(AEVec4),*((float*)arg+sizeof(AEVec4)*2));
			break;
	}
	dSpaceSetCleanup(env->space,0);
	return env;
}

void AEPEnvAddBody(AEPEnv* env,AEPBody* body){
	dSpaceAdd(env->space,body->geom);
}

void AEPEnvRemoveBody(AEPEnv* env,AEPBody* body){
	dSpaceRemove(env->space,body->geom);
}

void AEPEnvCheck(AEPEnv* env){
	dSpaceCollide(env->space,env,NearCallback);
}

void AEPEnvDelete(AEPEnv* env){
	int length=dSpaceGetNumGeoms(env->space);
	for(int i=0;i<length;i++){
		AEPBodyDelete(dGeomGetData(dSpaceGetGeom(env->space,i)));
	}
	dSpaceDestroy(env->space);
	free(env);
}

AEPBody* AEPBodyNew(int (*collideCallback)(AEPBody*,AEPBody*),void* data,int type,void* arg){
	AEPBody* body=calloc(1,sizeof(AEPBody));
	
	body->collideCallback=collideCallback;
	
	dMassSetZero(&body->mass);
	body->body=dBodyCreate(AEODEWorld);
	dBodySetMass(body->body,&body->mass);
	
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
			dGeomTriMeshDataBuildSimple(body->trimeshdata,mesh->vertices,mesh->vcount,mesh->indices,mesh->icount);
			body->geom=dCreateTriMesh(0,body->trimeshdata,NULL,NULL,NULL);
			}break;
	}
	
	body->data=data;
	dGeomSetBody(body->geom,body->body);
	dGeomSetData(body->geom,body);
	
	return body;
}

void AEPBodySetMass(AEPBody* body,AEVec3 size,float mass){
	dMassSetBoxTotal(&body->mass,size.x,size.y,size.z,mass);
	dBodySetMass(body->body,&body->mass);
}

void AEPBodySetVelocity(AEPBody* body,float x,float y,float z){
	dBodySetLinearVel(body->body,x,y,z);
}

void AEPBodySetPos(AEPBody* body,float x,float y,float z){
	dBodySetPosition(body->body,x,y,z);
}

AEVec3 AEPBodyGetPos(AEPBody* body){
	dReal* v4=dBodyGetPosition(body->body);
	AEVec3 v3={v4[0],v[1],v[2]};
	/*v3.x=v4[0];
	v3.y=v4[1];
	v3.z=v4[2];*/
	return v3;
}

void AEPBodySetCenterOfGravity(AEPBody* body,float x,float y,float z){
	dMassTranslate(&body->mass,x,y,z);
}

void AEPBodyDelete(AEPBody* body){
	if(body->trimeshdata) dGeomTriMeshDataDestroy(body->trimeshdata);
	dGeomDestroy(body->geom);
	dBodyDestroy(body->body);
	free(body);
}

////////////////////////////

static void NearCallback (void *data, dGeomID o1, dGeomID o2){
	if (dGeomIsSpace (o1) || dGeomIsSpace (o2)){
		// colliding a space with something
		dSpaceCollide2 (o1,o2,data,&NearCallback);
		// collide all geoms internal to the space(s)
		if (dGeomIsSpace (o1)) dSpaceCollide ((dSpaceID)o1,data,&NearCallback);
		if (dGeomIsSpace (o2)) dSpaceCollide ((dSpaceID)o2,data,&NearCallback);
	}
	else{
		static const int MAX_CONTACTS = 3;
		dContact contacts[MAX_CONTACTS];

		for (int i = 0; i < MAX_CONTACTS; i++){
			//Should really set to the average of the bodies' surfaces
			contacts[i].surface.mode = dContactBounce | dContactSoftCFM;
			contacts[i].surface.mu = dInfinity;
			contacts[i].surface.mu2 = 0;
			contacts[i].surface.bounce = 0.8;
			contacts[i].surface.bounce_vel = 0.1;
			contacts[i].surface.soft_cfm = 0.01;
		}
		
		int numc=dCollide(o1, o2, MAX_CONTACTS, &contacts[0].geom, sizeof(dContact));
		
		if (numc){
			AEPBody* pb1=dGeomGetData(o1);
			AEPBody* pb2=dGeomGetData(o2);
			if(pb1->collideCallback && (*pb1->collideCallback)(pb1,pb2)==0) return;
			// Get the dynamics body for each geom
			dBodyID b1 = dGeomGetBody(o1);
			dBodyID b2 = dGeomGetBody(o2);
			// To add each contact point found to our joint group we call dJointCreateContact which is just one of the many
			// different joint types available.  
			for (int i = 0; i < numc; i++){
				// dJointCreateContact needs to know which world and joint group to work with as well as the dContact
				// object itself. It returns a new dJointID which we then use with dJointAttach to finally create the
				// temporary contact joint between the two geom bodies.
				dJointID c = dJointCreateContact(AEODEWorld,AEODEContacts, contacts + i);
				dJointAttach(c, b1, b2);
			}
		}
	}
}

void AEPInit(void){
	dInitODE();
	AEODEWorld=dWorldCreate();
	AEODEContacts=dJointGroupCreate(AEODEWorld);
}

void AEPStep(float step){
	dWorldQuickStep(AEODEWorld,(*((float*)data)));
	dJointGroupEmpty(AEODEContacts);
}

void AEPSetGravity(float x,float y,float z){
	dWorldSetGravity(AEODEWorld,x,y,z);
}

AEVec3 AEPGetGravity(void){
	dVector3 v4;
	dWorldGetGravity(AEODEWorld,v4);
	AEVec3 v3={v[0].v[1],v[2]};
	return v3;
}

void AEPQuit(void){
	dJointGroupEmpty(AEODEContacts);
	dWorldDestroy(AEODEWorld);
	dCloseODE();
}