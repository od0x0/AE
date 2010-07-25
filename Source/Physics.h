#pragma once
#include "Core.h"
#include "HeaderBegin.h"
#include "VBO.h"

#define int32 odeint32
#define uint32 odeuint32
#define int16 odeint16
#define uint16 odeuint16
#define int8 odeint8
#define uint8 odeuint8
#include "ode/ode.h"
#undef int32
#undef uint32
#undef int16
#undef uint16
#undef int8
#undef uint8

struct AEPBody;
typedef struct AEPBody AEPBody;

struct AEPEnv;
typedef struct AEPEnv AEPEnv;

#define AEPGeomTypeSphere 1
#define AEPGeomTypeBox 2
#define AEPGeomTypeMesh 3

#define AEPEnvTypeSimple 1
#define AEPEnvTypeHash 2
#define AEPEnvTypeLarge 3

#define AEPBodyFlagBounciness 1
#define AEPBodyFlagBounceVelocity 2
#define AEPBodyFlagFriction 3
#define AEPBodyFlagMovable 4

typedef struct{
	int icount,vcount;
	int* indices;
	dReal* vertices;
}AEPMesh;

////////////////////////////

AEPBody* AEPBodyNew(int (*collideCallback)(AEPBody*,AEPBody*),int type,void* geomdata);

AEPEnv* AEPBodyEnvGet(AEPBody* body);
void AEPBodyEnvSet(AEPBody* body,AEPEnv* env);

void AEPBodyAddChild(AEPBody* body,AEPBody* body2);
void AEPBodyRemoveChild(AEPBody* body,AEPBody* body2);

AEPBody* AEPBodyParentGet(AEPBody* body);

void AEPBodyMassSet(AEPBody* body,AEVec3 size,float mass);

void AEPBodyPositionGet(AEPBody* body,AEVec3* v3);
void AEPBodyPositionSet(AEPBody* body,float x,float y,float z);
void AEPBodyVelocityGet(AEPBody* body,AEVec3* v3);
void AEPBodyVelocitySet(AEPBody* body,float x,float y,float z);
void AEPBodyRotationGet(AEPBody* body,AEQuat* q4);
void AEPBodyRotationSet(AEPBody* body,float x,float y,float z,float w);
void AEPBodySpinGet(AEPBody* body,AEVec3* v3);
void AEPBodySpinSet(AEPBody* body,float x,float y,float z);

void AEPBodyFlagSet(AEPBody* body,int flag,float value);
float AEPBodyFlagGet(AEPBody* body,int flag);
void AEPBodyCollisionCallbackSet(AEPBody* body,int (*collideCallback)(AEPBody*,AEPBody*));
void AEPBodyCenterOfGravitySet(AEPBody* body,float x,float y,float z);
void AEPBodyDelete(AEPBody* body);

#define AEPTypeEnv 1
#define AEPTypeBody 2
int AEPTypeGet(void* object);
void AEPTypeDataSet(void* object,void* data);
void* AEPTypeDataGet(void* object);

////////////////////////////

void AEPInit(void);

void AEPStep(float step);

void AEPGravitySet(float x,float y,float z);
AEVec3 AEPGravityGet(void);
void AEPQuit(void);

/// Deprecated

AEPEnv* AEPEnvNewSimple(AEPEnv* parent);
AEPEnv* AEPEnvNewHash(AEPEnv* parent, int minlevels, int maxlevels);
AEPEnv* AEPEnvNewLarge(AEPEnv* parent,AEVec3* center, AEVec3* size, int depth);

AEPEnv* AEPEnvParentGet(AEPEnv* env);
void AEPEnvAddBody(AEPEnv* env,AEPBody* body);
void AEPEnvRemoveBody(AEPEnv* env,AEPBody* body);
void AEPEnvCheck(AEPEnv* env);
void AEPEnvDelete(AEPEnv* env);

#include "HeaderEnd.h"