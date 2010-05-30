#include "../Core.h"
#include "../World.h"
#include "../ODE/ode/ode.h"

//Some code based off of http://artis.imag.fr/Membres/Xavier.Decoret/resources/ode/tutorial1.html

struct{
	dWorldID odeWorld;
	dSpaceID odeSpace;
	dJointGroupID odeContacts;
}AEWorld;

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
			contacts[i].surface.mode = dContactBounce | dContactSoftCFM;
			contacts[i].surface.mu = dInfinity;
			contacts[i].surface.mu2 = 0;
			contacts[i].surface.bounce = 0.8;
			contacts[i].surface.bounce_vel = 0.1;
			contacts[i].surface.soft_cfm = 0.01;
		}
		
		int numc=dCollide(o1, o2, MAX_CONTACTS, &contacts[0].geom, sizeof(dContact));
		
		if (numc){
			// Get the dynamics body for each geom
			dBodyID b1 = dGeomGetBody(o1);
			dBodyID b2 = dGeomGetBody(o2);
			// To add each contact point found to our joint group we call dJointCreateContact which is just one of the many
			// different joint types available.  
			for (int i = 0; i < numc; i++){
				// dJointCreateContact needs to know which world and joint group to work with as well as the dContact
				// object itself. It returns a new dJointID which we then use with dJointAttach to finally create the
				// temporary contact joint between the two geom bodies.
				dJointID c = dJointCreateContact(AEWorld.odeWorld,AEWorld.odeContacts, contacts + i);
				dJointAttach(c, b1, b2);
			}
		}
	}
}

static void* AEWorldDefaultEvent(int event,int value,void* data){
	switch(event){
		case AEWorldEventInit:
			dInitODE();
			AEWorld.odeWorld=dWorldCreate();
			AEWorld.odeSpace=dHashSpaceCreate(AEWorld.odeSpace);
			AEWorld.odeContacts=dJointGroupCreate(0);
			break;
		case AEWorldEventLoad:
			
			break;
		case AEWorldEventRelease:
			dSpaceDestroy(AEWorld.odeSpace);
			dWorldDestroy(AEWorld.odeWorld);
			dCloseODE();
			break;
		case AEWorldEventRender:
			break;
		case AEWorldEventPhysics:
			dSpaceCollide(AEWorld.odeSpace,&AEWorld,NearCallback);
			dWorldQuickStep(AEWorld.odeWorld,(*((float*)data)));
			dJointGroupEmpty(AEWorld.odeContacts);
			break;
	}
}

void* (*AEWorldEvent)(int event,int value,void* data)=NULL;//AEWorldDefaultEvent;
