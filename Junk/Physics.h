#pragma once
#include "Core.h"
#include "HeaderBegin.h"
#include "VBO.h"

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

typedef struct{
	int icount,vcount;
	int* indices;
	AEVec4* vertices;
}AEPMesh;

////////////////////////////
AEPEnv* AEPEnvNew(AEPEnv* parent,int type,void* arg);
void AEPEnvAddBody(AEPEnv* env,AEPBody* body);
void AEPEnvRemoveBody(AEPEnv* env,AEPBody* body);
void AEPEnvCheck(AEPEnv* env);
void AEPEnvDelete(AEPEnv* env);
AEPBody* AEPBodyNew(int (*collideCallback)(AEPBody*,AEPBody*),void* data,int type,void* arg);
void AEPBodySetMass(AEPBody* body,AEVec3 size,float mass);
void AEPBodySetVelocity(AEPBody* body,float x,float y,float z);
void AEPBodySetPos(AEPBody* body,float x,float y,float z);
AEVec3 AEPBodyGetPos(AEPBody* body);
void AEPBodySetCenterOfGravity(AEPBody* body,float x,float y,float z);
void AEPBodyDelete(AEPBody* body);

////////////////////////////

void AEPInit(void);

void AEPStep(float step);

void AEPSetGravity(float x,float y,float z);
AEVec3 AEPGetGravity(void);
void AEPQuit(void);

#include "HeaderEnd.h"