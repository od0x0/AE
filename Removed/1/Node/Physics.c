#include "Physics.h"

dWorldID AEODEWorld;
dJointGroupID AEODEContacts;

static dSpaceID AEdSpaceNew(dSpaceID parent,int type,void* arg){
	dSpaceID space=0;
	switch(type){
		case AEdSpaceTypeSimple:
			space=dSimpleSpaceCreate(parent);
			break;
		case AEdSpaceTypeHash:
			space=dHashSpaceCreate(parent);
			dHashSpaceSetLevels(space,((int*)arg)[0],((int*)arg)[1]);
			break;
		case AEdSpaceTypeLarge:{
			float* array=arg;
			dVector3 center={array[0],array[1],array[2]};
			dVector3 extents={array[3],array[4],array[5]};
			int depth=array[7];
			space=dQuadTreeSpaceCreate(parent,center,extents,depth);
			}break;
	}
	dSpaceSetCleanup(space,1);
	return space;
}

dSpaceID AEdSpaceNewSimple(dSpaceID parent){
	return AEdSpaceNew(parent,AEdSpaceTypeSimple,NULL);
}

dSpaceID AEdSpaceNewHash(dSpaceID parent, int minlevels, int maxlevels){
	int array[2]={minlevels,maxlevels};
	return AEdSpaceNew(parent,AEdSpaceTypeHash,array);
}

dSpaceID AEdSpaceNewLarge(dSpaceID parent,AEVec3* center, AEVec3* size, int depth){
	float array[7]={center->x, center->y, center->z,  size->x, size->y, size->z,  depth};
	return AEdSpaceNew(parent,AEdSpaceTypeLarge,array);
}

/*void AEdMeshClear(AEdMesh* mesh){
	if(mesh==NULL) return;
	
	dGeomTriMeshDataDestroy(mesh->trimeshdata);
	AEdMesh* mesh=dGeomGetData(body->geom);
	free(mesh->vertices);
	free(mesh->indices);
}*/

static dGeomID AEdMeshGeomNew(dSpaceID parent,AEdMesh* mesh){
	if(mesh==NULL) AEError("Given a NULL mesh.");
	//mesh->trimeshdata=dGeomTriMeshDataCreate();
	dGeomTriMeshDataBuildSimple(mesh->trimeshdata,mesh->vertices,mesh->vcount,mesh->indices,mesh->icount);
	dGeomID geom=dCreateTriMesh(parent,mesh->trimeshdata,NULL,NULL,NULL);
	dGeomSetData(geom,mesh);
	return geom;
}

dGeomID AEdGeomAndBodyNew(dSpaceID parent,int type,void* arg){
	
	dBodyID body=dBodyCreate(AEODEWorld);
	
	dMass mass;
	dBodyGetMass(body,&mass);
	dMassSetZero(&mass);
	dBodySetMass(body,&mass);
	
	dGeomID geom=0;
	
	switch(type){
		case AEdGeomTypeSphere:
			geom=dCreateSphere(parent,((float*)arg)[0]);
			break;
		case AEdGeomTypeBox:
			geom=dCreateBox(parent,((float*)arg)[0],((float*)arg)[1],((float*)arg)[2]);
			break;
		case AEdGeomTypeMesh:
			geom=AEdMeshGeomNew(parent,arg);
			break;
	}
	
	dGeomSetBody(geom,body);
	
	return geom;
}

void AEdBodyMassSet(dBodyID body,AEVec3 size,float massamount){
	dMass mass;
	dBodyGetMass(body,&mass);
	if(massamount) dBodySetGravityMode(body,1);
	else{
		dMassSetZero(&mass);
		dBodySetGravityMode(body,0);
		return;
	}
	dMassSetBoxTotal(&mass,size.x,size.y,size.z,massamount);
	//dMassAdjust(dBodyGetMass(dGeomGetBody(geom)),mass);
	dBodySetMass(body,&mass);
}

void AEdGeomPositionGet(dGeomID geom,AEVec3* v3){
	const dReal* v4=dGeomGetPosition(geom);
	v3->x=v4[0];
	v3->y=v4[1];
	v3->z=v4[2];
}

void AEdGeomPositionSet(dGeomID geom,float x,float y,float z){
	dGeomSetPosition(geom,x,y,z);
}

void AEdGeomVelocityGet(dGeomID geom,AEVec3* v3){
	const dReal* v4=dBodyGetLinearVel(dGeomGetBody(geom));
	v3->x=v4[0];
	v3->y=v4[1];
	v3->z=v4[2];
}

void AEdGeomVelocitySet(dGeomID geom,float x,float y,float z){
	dBodySetLinearVel(dGeomGetBody(geom),x,y,z);
}

#warning You may need to take into account ODE's w,x,y,z order for quaternions


// This code swizzles from ODE's w,x,y,z to our x,y,z,w

void AEdGeomRotationGet(dGeomID geom,AEQuat* q4){
	const dReal* odeq4=dBodyGetQuaternion(dGeomGetBody(geom));
	q4->x=odeq4[1];
	q4->y=odeq4[2];
	q4->z=odeq4[3];
	q4->w=odeq4[0];
}

void AEdGeomRotationSet(dGeomID geom,float x,float y,float z,float w){
	dQuaternion odeq4={w,x,y,z};
	dGeomSetQuaternion(geom,odeq4);
}

void AEdGeomSpinGet(dGeomID geom,AEVec3* v3){
	const dReal* v4=dBodyGetAngularVel(dGeomGetBody(geom));
	v3->x=v4[0];
	v3->y=v4[1];
	v3->z=v4[2];
}

void AEdGeomSpinSet(dGeomID geom,float x,float y,float z){
	dBodySetAngularVel(dGeomGetBody(geom),x,y,z);
}

////////////////////////////

void AEdNearCallback (void *data, dGeomID o1, dGeomID o2){
	void* *callbackpointers=data;
	int (*collideCallback)(dGeomID,dGeomID,AEdSurface*,void*)=NULL;
	void* collideCallbackData=NULL;
	
	if(callbackpointers){
		collideCallback=callbackpointers[0];
		collideCallbackData=callbackpointers[1];
	}
	
	if (dGeomIsSpace (o1) || dGeomIsSpace (o2)){
		// colliding a space with something
		dSpaceCollide2 (o1,o2,data,&AEdNearCallback);
		// collide all geoms internal to the space(s)
		if (dGeomIsSpace (o1)) dSpaceCollide ((dSpaceID)o1,data,&AEdNearCallback);
		if (dGeomIsSpace (o2)) dSpaceCollide ((dSpaceID)o2,data,&AEdNearCallback);
	}
	else{
		
		dBodyID b1 = dGeomGetBody(o1);
		dBodyID b2 = dGeomGetBody(o2);
		
		static const int MAX_CONTACTS = 3;
		dContact contacts[MAX_CONTACTS];
		
		int numc=dCollide(o1, o2, MAX_CONTACTS, &contacts[0].geom, sizeof(dContact));
		
		if (numc){
			AEdSurface surface={.bounciness=0, .bounceVelocity=0, .friction=dInfinity, .body1=b1, .body2=b2};
			if(collideCallback && (*collideCallback)(o1,o2,&surface,collideCallbackData)==0)return;
			
			for (int i = 0; i < MAX_CONTACTS; i++){
				//Set to the average of the bodies' surfaces
				
				contacts[i].surface.mode = (surface.bounciness ? dContactBounce : 0) | dContactSoftCFM;
				contacts[i].surface.mu = surface.friction;
				contacts[i].surface.mu2 = 0;
				contacts[i].surface.bounce = surface.bounciness;
				contacts[i].surface.bounce_vel = surface.bounceVelocity;
				contacts[i].surface.soft_cfm = 0.01;
			}
			
			for (int i = 0; i < numc; i++){
				dJointID c = dJointCreateContact(AEODEWorld,AEODEContacts, contacts + i);
				dJointAttach(c, surface.body1, surface.body2);
			}
		}
	}
}

void AEdInit(void){
	dInitODE();
	AEODEWorld=dWorldCreate();
	AEODEContacts=dJointGroupCreate(0);
}

void AEdStep(float step){
	dWorldQuickStep(AEODEWorld,step);
	dJointGroupEmpty(AEODEContacts);
}

void AEdGravitySet(float x,float y,float z){
	dWorldSetGravity(AEODEWorld,x,y,z);
}

void AEdGravityGet(AEVec3* v3){
	dVector3 v4;
	dWorldGetGravity(AEODEWorld,v4);
	AEVec3 temp={v4[0],v4[1],v4[2]};
	*v3=temp;
}

void AEdQuit(void){
	dJointGroupEmpty(AEODEContacts);
	dWorldDestroy(AEODEWorld);
	dCloseODE();
}