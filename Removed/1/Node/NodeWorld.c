#include "NodeInternal.h"

static size_t WorldCount=0;
static dJointGroupID ContactJointGroup=0;
static AENodeWorld* AENodeWorldActive=NULL;

static void AENodeWorldsInitialize(void){
	WorldCount++;
	if(WorldCount > 1) return;
	
	dInitODE();
	ContactJointGroup=dJointGroupCreate(0);
}

static void AENodeWorldsDeinitialize(void){
	//First, check this
	if(WorldCount==0) AEError("Mismatched calls to allocation/deallocation of AENodeWorlds.");
	//Ok, now it is safe to continue
	WorldCount--;
	if(WorldCount > 0) return;
	
	dJointGroupDestroy(ContactJointGroup);
	dCloseODE();
}

void AENodeWorldInit(AENode* node){
	AENodeWorld* world=(AENodeWorld*)node;
	
	AENodeWorldsInitialize();
	
	world->odeworld=dWorldCreate();
	
	AENodeWorldActive=world;
}

void AENodeWorldDelete(AENode* node){
	AENodeWorld* world=(AENodeWorld*)node;
	
	if(world->children) 
		for(size_t i=0;i<AEListLength(world->children);i++)
			AENodeDelete(AEListAsArray(world->children,AENode*)[i]);
	
	AENodeWorldsDeinitialize();
}

void AENodeWorldRender(AENode* node){
	AENodeWorld* world=(AENodeWorld*)node;
	
	if(world->children) 
		for(size_t i=0;i<AEListLength(world->children);i++)
			AENodeRender(AEListAsArray(world->children,AENode*)[i]);
}

static void AENodeNearCallback (void *data, dGeomID o1, dGeomID o2){
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
			AENodeEnt* ents[2]={dBodyGetData(surface.body2),dBodyGetData(surface.body2)};
			surface.bounciness=(ents[0]->bodyProperties.bounciness+ents[1]->bodyProperties.bounciness)*0.5;
			surface.bounceVelocity=(ents[0]->bodyProperties.bounceVelocity+ents[1]->bodyProperties.bounceVelocity)*0.5;
			surface.friction=(ents[0]->bodyProperties.friction+ents[1]->bodyProperties.friction)*0.5;
			if(ents[0]->bodyProperties.immovable) surface.body1=0;
			if(ents[1]->bodyProperties.immovable) surface.body2=0;
			
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
				dJointID c = dJointCreateContact(((AENodeWorld*)data)->odeworld,ContactJointGroup, contacts + i);
				dJointAttach(c, surface.body1, surface.body2);
			}
		}
	}
}

/*
static int CollideCallback(dGeomID geom1,dGeomID geom2,AEdSurface* surface,void* userdata){
	AENodeEnt* ents[2]={dBodyGetData(surface->body2),dBodyGetData(surface->body2)};
	surface->bounciness=(ents[0]->bodyProperties.bounciness+ents[1]->bodyProperties.bounciness)*0.5;
	surface->bounceVelocity=(ents[0]->bodyProperties.bounceVelocity+ents[1]->bodyProperties.bounceVelocity)*0.5;
	surface->friction=(ents[0]->bodyProperties.friction+ents[1]->bodyProperties.friction)*0.5;
	if(ents[0]->bodyProperties.immovable) surface->body1=0;
	if(ents[1]->bodyProperties.immovable) surface->body2=0;
	return 1;
}
*/
void AENodeWorldStep(AENode* node,float stepInSeconds){
	AENodeWorld* world=(AENodeWorld*)node;
	
	if(world->children) 
		for(size_t i=0;i<AEListLength(world->children);i++)
			AENodeStep(AEListAsArray(world->children,AENode*)[i],stepInSeconds);

	dSpaceCollide2((dGeomID)world->staticSpace,(dGeomID)world->dynamicSpace,AENodeNearCallback,node);
	dSpaceCollide(world->dynamicSpace,AENodeNearCallback,node);
	
	dWorldQuickStep(world->odeworld,stepInSeconds);
	dJointGroupEmpty(ContactJointGroup);
}

void AENodeWorldAddChild(AENode* node,AENode* node2){
	AENodeWorld* world=(AENodeWorld*)node;
	if(world->children==NULL) world->children=AEListNew(AENode*);
	node2=AENodeRetain(node2);
	//Find first open slot, create if no preexisting
	if(AENodeTypeGet(node2)!=AENodeTypeEnt) AEError("Invalid node type being added to a world.  Only Ents are allowed to be children of a World.");
	AENodeEnt* ent2=(AENodeEnt*)node2;
	ent2->world=(AENodeWorld*)node;
	AENode* blank=NULL;
	size_t index=AEListFindIndexOfBytes(world->children,&blank);
	if(index==0) AEListAdd(world->children,AENode*,AENodeRetain(node2));
	else AEListAsArray(world->children,AENode*)[index-1]=node2;
}

dWorldID AENodeWorldODEWorldGet(AENode* node){
	AENodeWorld* world=(AENodeWorld*)node;
	return world->odeworld;
}

void AENodeWorldODEWorldSet(AENode* node,dWorldID odeworld){
	AENodeWorld* world=(AENodeWorld*)node;
	world->odeworld=odeworld;
}

dSpaceID AENodeWorldStaticSpaceGet(AENode* node){
	AENodeWorld* world=(AENodeWorld*)node;
	return world->staticSpace;
}

void AENodeWorldStaticSpaceSet(AENode* node,dSpaceID space){
	AENodeWorld* world=(AENodeWorld*)node;
	world->staticSpace=space;
}

dSpaceID AENodeWorldDynamicSpaceGet(AENode* node){
	AENodeWorld* world=(AENodeWorld*)node;
	return world->dynamicSpace;
}

void AENodeWorldDynamicSpaceSet(AENode* node,dSpaceID space){
	AENodeWorld* world=(AENodeWorld*)node;
	world->dynamicSpace=space;
}