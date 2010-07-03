#include "../Physics.h"

AEPContact* AEPContacts=NULL;
unsigned int AEPContactCountMax=0,AEPContactCount=0;

void AEPContactsAdd(AEPGeom* g,AEPGeom* g2,AEVec3 correction){
	AEPContact c={g,g2,correction};
	if(AEPContactCountMax<=AEPContactCount) return;
	AEPContacts[AEPContactCount++]=c;
}

void AEPContactsAct(void){
	for(unsigned int i=0;i<AEContactCount;i++){
		AEPContact* c=AEContacts+i;
		(*c->g->collisionCallback)(c);
	}
	AEContactCount=0;
}

#define AEUCharTo0_1(c) ((c)*(1.0f/255.0f))

inline AEPGeomApplyCollisionForces(AEPGeom* g,AEPGeom* g2){
			if(g->movementProperties==NULL) return;//Check if this is static, can't handle forces if so
			
			//Heavier objects displace lighter ones
			if(g2->movementProperties)
				if(g->movementProperties->mass < g2->movementProperties->mass)
					g=g2;
			/*
				Does the formula:
				axis=smallestAxis((o-o2)-(o size+o2 size)
				o[axis]=o2[axis]+o2 size[axis]+o size[axis]
			*/
			
			int axis=0;
			AEVec3 penetration;
			penetration.x=(o->x - g2->x)-(o->size.x+g2->aabb.x);
			penetration.y=(o->y - g2->y)-(o->size.y+g2->aabb.y);
			penetration.z=(o->z - g2->z)-(o->size.z+g2->aabb.z);
			if(penetration.y>penetration.x && penetration.y>penetration.z) axis=1;
			else if(penetration.z>penetration.x && penetration.z>penetration.x) axis=2;
			//Warning, ugly code, does o[axis]=o2[axis]+o2 size[axis]+o size[axis]
			((float*)o)[axis]=((float*)o2)[axis]+((float*)&(o2->size))[axis]+((float*)&(o->size))[axis];
			
			((float*)&(o->velocity))[axis]*= -AEUCharTo0_1(g->movementProperties->bounciness);//Invert movement direction and scale by its bounciness
}

int AEPGeomCollide(AEPGeom* g,AEPGeom* g2){
	
	if(g->type==AEGeomTypeRay){
		return 0;
	}

	switch(g->type){
		case AEGeomTypeAABB:return 1;
		/*case AEGeomTypeMesh:
			return 0;
		case AEGeomTypeEnv:
			for(unsigned int i=0;i < g->env->geomCount;i++){
				AEPGeom* subg=g->env->geoms+i;
				if(AEPGeomCollide(subg,g2)) return 1;
			}
			return 0;*/
	}
	
	return 0;
}

void AEPEnvStep(AEPEnv* env,float step){
	// O(n^2) slowness is clean
	
	for (int i = 0;i<env->geomCount;i++) {
			AEPGeom* g=env->geoms+i;
			
			if(g->movementProperties==NULL) continue;
			
			//Add global force to the velocity, so that the acceleration builds in the direction of the global force
			g->velocity.x+=globalForce.x*o->mass*step;
			g->velocity.y+=globalForce.y*o->mass*step;
			g->velocity.z+=globalForce.z*o->mass*step;
			
			//Velocity is useless if it's not applied
			g->x+=o->velocity.x*step;
			g->y+=o->velocity.y*step;
			g->z+=o->velocity.z*step;
			
			for(unsigned int j=0;j<env->geomCount;j++){
				AEPGeom* g2=env->geoms+j;
				if(AEPGeomCollide(g,g2)) exit(0);
			}
	}
	
	AEContactsAct();
}