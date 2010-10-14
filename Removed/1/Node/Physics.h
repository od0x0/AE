//An ODE helper
#pragma once
#include "../Core.h"
#include "../VBO.h"

#define int32 odeint32
#define uint32 odeuint32
#define int16 odeint16
#define uint16 odeuint16
#define int8 odeint8
#define uint8 odeuint8
#include "../ode/ode.h"
#undef int32
#undef uint32
#undef int16
#undef uint16
#undef int8
#undef uint8

#define AEdGeomTypeSphere 1
#define AEdGeomTypeBox 2
#define AEdGeomTypeMesh 3

#define AEdSpaceTypeSimple 1
#define AEdSpaceTypeHash 2
#define AEdSpaceTypeLarge 3

typedef struct{
	int icount,vcount;
	int* indices;
	dReal* vertices;
	dTriMeshDataID trimeshdata;
}AEdMesh;

typedef struct{
	float bounciness,bounceVelocity,friction;
	dBodyID body1,body2;
}AEdSurface;

extern dWorldID AEODEWorld;
extern dJointGroupID AEODEContacts;

////////////////////////////

//These simplify the creation of geoms and spaces

dSpaceID AEdSpaceNewSimple(dSpaceID parent);

dSpaceID AEdSpaceNewHash(dSpaceID parent, int minlevels, int maxlevels);

dSpaceID AEdSpaceNewLarge(dSpaceID parent,AEVec3* center, AEVec3* size, int depth);

dGeomID AEdGeomAndBodyNew(dSpaceID parent,int type,void* arg);

void AEdGeomMassSet(dGeomID geom,AEVec3 size,float mass);

//These wrap the ode function calls and convert the layouts

void AEdGeomPositionGet(dGeomID geom,AEVec3* v3);

void AEdGeomPositionSet(dGeomID geom,float x,float y,float z);

void AEdGeomVelocityGet(dGeomID geom,AEVec3* v3);

void AEdGeomVelocitySet(dGeomID geom,float x,float y,float z);

void AEdGeomRotationGet(dGeomID geom,AEQuat* q4);

void AEdGeomRotationSet(dGeomID geom,float x,float y,float z,float w);

void AEdGeomSpinGet(dGeomID geom,AEVec3* v3);

void AEdGeomSpinSet(dGeomID geom,float x,float y,float z);

////////////////////////////

//Handles collisions between two geoms (used in dSpaceCollide and dSpaceCollide2), collisionCallbackClosure is a pointer to an void pointer array with [0] as a function pointer to a callback of int (*)(dGeomID,dGeomID,AEdSurface*,void* userdata) userdata is collisionCallbackClosure passed back.  Or, it can be null, and everything is handled as normal (no slipping, collisions are 2 way and everything is movable, no bouncing)

void AEdNearCallback (void *collisionCallbackClosure, dGeomID o1, dGeomID o2);

//These handle AEODEWorld and AEODEContacts

void AEdGravitySet(float x,float y,float z);

void AEdGravityGet(AEVec3* v3);

void AEdInit(void);

void AEdStep(float step);

void AEdQuit(void);