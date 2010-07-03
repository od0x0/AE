#include "../Core.h"
#include "../SOIL/SOIL.h"
#include <math.h>
#include <string.h>

AEObject** AEObjects=NULL;
unsigned int AEObjectCount=0;

AEObject* AEObjectNew(void (*event)(AEObject* o,int event,void* data),void* data){
	//Allocate new object
	AEObject* o=calloc(1,sizeof(AEObject));
	
	//Bind to a slot in the object array
	AEObjectCount++;
	AEObjects=(AEObject**)realloc(AEObjects,sizeof(AEObject*)*AEObjectCount);
	AEObjects[AEObjectCount-1]=o;
	
	//Initialize object
	//NULL callback == default callback
	if(event==NULL) event=AEObjectDefaultEvent;
	o->event=event;
	(*o->event)(o,AEObjectEventInit,data);
	return o;
}

int AEObjectsCollide(AEObject* o,AEObject* o2){
	//Really fast AABB collision detection (as compared to the system I used in Toxic, which used a rather quickly coded physics engine)
	
	if((o->size.x+o->size.y+o->size.z)==0) return 0;
	else if((o2->size.x+o2->size.y+o2->size.z)==0) return 0;
	//If the size of either object is 0, assume that they are non-collidable, and return no collision
	
	AEVec3 distance={o->x-o2->x,o->y-o2->y,o->z-o2->z};
	if(distance.x<0) distance.x=-distance.x;
	if(distance.y<0) distance.y=-distance.y;
	if(distance.z<0) distance.z=-distance.z;
	
	AEVec3 combinedSize={o->size.x+o2->size.x,o->size.y+o2->size.y,o->size.z+o2->size.z};
	
	if(distance.x>combinedSize.x) return 0;
	if(distance.y>combinedSize.y) return 0;
	if(distance.z>combinedSize.z) return 0;
	
	return 1;
}

void AEObjectsSignal(int event,void* data){
	//Loops over all objects, sending signal (event)
	for (int i = 0;i<AEObjectCount;i++) (*(AEObjects[i]->event))(AEObjects[i],event,data);
}

//Whaduya think?
//Implements a sort of dynamic message lookup, I didn't use dedicated callbacks for this reason.
//Same concept as messaging in ObjC
void AEObjectDefaultEvent(AEObject* o,int event,void* data){
	switch(event){
		case AEObjectEventInit:
			break;
			
		case AEObjectEventRender:
			if(o->isStatic==AEObjectCompiled) break;
			//For the case that some Octant has an object compiled into it
		case AEObjectEventRenderFromCompile:
			glPushMatrix();
				glTranslatef(o->x,o->y,o->z);
				glRotatef(o->rotation.x,	1,0,0);
				glRotatef(o->rotation.y,	0,1,0);
				glRotatef(o->rotation.z,	0,0,1);
				
				AETextureBind(o->texture);
				AEVBODraw(o->model);
			glPopMatrix();			
			break;
			
		case AEObjectEventCollide:{
			if(o->isStatic) return;
			//Redundant check, but oh well.  (It's redundant because it gets checked in AEObjectsPhysics)
			//However, this might not be called from only that function
			/*o->x=o->last.x;
			o->y=o->last.y;
			o->z=o->last.z;*/
			/*
				Does the formula:
				axis=smallestAxis((o-o2)-(o size+o2 size)
				o[axis]=o2[axis]+o2 size[axis]+o size[axis]
			*/
			
			AEObject* o2=data;
			int axis=0;
			AEVec3 penetration;
			penetration.x=(o->x-o2->x)-(o->size.x+o2->size.x);
			penetration.y=(o->y-o2->y)-(o->size.y+o2->size.y);
			penetration.z=(o->z-o2->z)-(o->size.z+o2->size.z);
			if(penetration.y>penetration.x && penetration.y>penetration.z) axis=1;
			else if(penetration.z>penetration.x && penetration.z>penetration.x) axis=2;
			//Warning, ugly code, does o[axis]=o2[axis]+o2 size[axis]+o size[axis]
			((float*)o)[axis]=((float*)o2)[axis]+((float*)&(o2->size))[axis]+((float*)&(o->size))[axis];
			
			((float*)&(o->velocity))[axis]*=-1;
			}break;
			
		case AEObjectEventRelease:
			AETextureDelete(o->texture);
			o->texture=0;
			AEVBODelete(o->model);
			o->model=NULL;
			break;
		
		//For debugging purposes, although may get annoying in certain places
		/*default:
			printf("AEObjectDefaultEvent(%p,%i,%p):Unknown event sent to Object %i\n",o,event,data,AELinearSearch(o,AEObjects,AEObjectCount)-1);
			break;*/
	}
}

void AEObjectDelete(AEObject* o){	
	//Slow, but I really don't care about that.  You shouldn't be deleting objects every frame anyway
	if(o){
	//If this is a valid object, delete it
		if(o->event) (*o->event)(o,AEObjectEventRelease,NULL);
		free(o);
		unsigned int current=0;
		for(unsigned int i=0;i<AEObjectCount;i++){
			if(AEObjects[i]!=o)
				AEObjects[current++]=AEObjects[i];
		}
		AEObjectCount--;
	}else{
	//If o is null, delete all objects
		for(unsigned int i=0;i<AEObjectCount;i++) AEObjectDelete(AEObjects[i]);
		AEObjectCount=0;
		//Can't hurt to be sure that the final object count is 0
	}
}

void AEObjectsPhysics(float step,AEVec3 globalForce){
	// O(n^2) slowness is clean
	AEObject* o=NULL;
	for (int i = 0;i<AEObjectCount;i++) {
		o=AEObjects[i];
		
		if(o->isStatic) continue;
		
		//Add global force to the velocity, so that the acceleration builds in the direction of the global force
		o->velocity.x+=globalForce.x*o->mass*step;
		o->velocity.y+=globalForce.y*o->mass*step;
		o->velocity.z+=globalForce.z*o->mass*step;
	
		//Velocity is useless if it's not applied
		o->x+=o->velocity.x*step;
		o->y+=o->velocity.y*step;
		o->z+=o->velocity.z*step;
		
		//Sorry, but we have to check for all, this is only designed for a small amount of dynamic objects anyway
		for (int i = 0;i<AEObjectCount;i++) {
			AEObject* o2=AEObjects[i];
			if(o==o2) continue;
			if(AEObjectsCollide(o,o2)){
				(*o->event)(o,AEObjectEventCollide,o2);
			}
		}
	}
}