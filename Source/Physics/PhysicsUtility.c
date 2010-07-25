#include "../ode/ode.h"
#include "../FastMath3D.h"

void dAEBodyGiveMass(void* body,float x,float y,float z,AEVec3 size,float amount){
	dMass mass;
	dMassSetZero(&mass);
	dMassSetBoxTotal(&mass,amount,size.x,size.y,size.z);
	dMassTranslate(&mass,x,y,z);
	dBodySetMass(body,&mass);
}

//void* dAE