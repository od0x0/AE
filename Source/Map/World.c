#include "../Core.h"
#include "../World.h"
/*
struct{
	dWorldID odeWorld;
	dSpaceID odeSpace;
}AEWorld;

static void NearCallback (void *data, dGeomID o1, dGeomID o2){
	if (dGeomIsSpace (o1) || dGeomIsSpace (o2)) {
		// colliding a space with something
		dSpaceCollide2 (o1,o2,data,&nearCallback);
		// collide all geoms internal to the space(s)
		if (dGeomIsSpace (o1)) dSpaceCollide (o1,data,&nearCallback);
		if (dGeomIsSpace (o2)) dSpaceCollide (o2,data,&nearCallback);
	}
	else {
		// colliding two non-space geoms, so generate contact
		// points between o1 and o2
		int num_contact = dCollide (o1,o2,max_contacts,contact_array,skip);
		// add these contact points to the simulation
		//...
	}
}

void* AEWorldDefaultEvent(int event,int value,void* data){
	switch(event){
		case AEWorldEventInit:
			AEWorld.odeWorld=dWorldCreate();
			AEWorld.odeSpace=dHashSpaceCreate(AEWorld.odeSpace);
			break;
		case AEWorldEventLoad:
			
			break;
		case AEWorldEventRelease:
			dSpaceDestroy(AEWorld.odeSpace);
			dWorldDestroy(AEWorld.odeWorld);
			break;
		case AEWorldEventRender:
			break;
		case AEWorldEventPhysics:
			dSpaceCollide(AEWorld.odeSpace,&AEWorld,NearCallback);
			dWorldQuickStep(AEWorld.odeWorld,(*((float*)data)));
			break;
	}
}
*/
void* (*AEWorldEvent)(int event,int value,void* data)=NULL;//AEWorldDefaultEvent;
