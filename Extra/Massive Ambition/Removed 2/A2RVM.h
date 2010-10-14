#pragma once
#include "VA.h"
#include "Material.h"
#include "Table.h"
/////////////////////////////////////////////		Actual RVM stuff
struct A2RVM;
typedef struct A2RVM A2RVM;

typedef void* A2RVMTrackedData;

typedef enum{
	A2RVMOpcodeNOP,
	
	A2RVMOpcodeBindVA, //va index
	A2RVMOpcodeBindVA_V, //va index
	A2RVMOpcodeBindVA_N, //va index
	A2RVMOpcodeBindVA_TC, //va index
	A2RVMOpcodeBindVA_TC2, //va index
	A2RVMOpcodeBindVA_TC3, //va index
	A2RVMOpcodeBindIA, //ia index
	A2RVMOpcodeBindProgram, //program index
	A2RVMOpcodeBindTexture, //texture index
	A2RVMOpcodeBindTexture2, //texture index
	A2RVMOpcodeBindTexture3, //texture index
	A2RVMOpcodeBindLight, //light index
	A2RVMOpcodeBindCamera, //camera index
	
	A2RVMOpcodeLoadIdentity, //none
	A2RVMOpcodeLoadCamera, //none
	
	A2RVMOpcodeTranslate, //vec3 index
	A2RVMOpcodeRotateAXA, //vec4 index (w=angle)
	A2RVMOpcodeRotateEUL, //vec3 index
	
	A2RVMOpcodeSkipIfVFCFail, //vfcobj index
	
	A2RVMOpcodeDraw //ia index
}A2RVMOpcode;

typedef struct{
	AEVec3 position;
	float radius;
	size_t opCountToSkipIfVFTestFails;
}A2RVMVFCObject;

typedef struct{
	AEVec3 position;
	AEVec3 rotation;
	float near,far,fov;
	char vfCullingEnabled;
}A2RVMCamera;

A2RVM* A2RVMNew(void);
A2RVM* A2RVMRetain(A2RVM* self);
void A2RVMDelete(A2RVM* self);

void A2RVMSerializeToFILE(A2RVM* self, FILE* file);
void A2RVMUnserializeFromFILE(A2RVM* self, FILE* file);

void A2RVMRender(A2RVM* self);

A2RVMTrackedData* A2RVMTrackedDataLookup(A2RVM* self, char* name);
AEVec3* A2RVMTrackedDataAsVec3(A2RVMTrackedData* self);
AEVec4* A2RVMTrackedDataAsVec4(A2RVMTrackedData* self);
float* A2RVMTrackedDataAs4x4(A2RVMTrackedData* self);
AEVA* A2RVMTrackedDataAsVA(A2RVMTrackedData* self);
AEVA* A2RVMTrackedDataAsIA(A2RVMTrackedData* self);
AETexture* A2RVMTrackedDataAsTexture(A2RVMTrackedData* self);
A2RVMVFCObject* A2RVMTrackedDataAsVFCObject(A2RVMTrackedData* self);
A2RVMCamera* A2RVMTrackedDataAsCamera(A2RVMTrackedData* self);
void A2RVMTrackedDataShouldUpdate(A2RVMTrackedData* self);

size_t A2RVMVAAdd(A2RVM* self, AEVA* va);
size_t A2RVMIAAdd(A2RVM* self, AEVA* ia);
size_t A2RVMTextureAdd(A2RVM* self, AETexture texture);
size_t A2RVMCameraAdd(A2RVM* self, A2RVMCamera* camera);
size_t A2RVMVec3Add(A2RVM* self, float x, float y, float z);
size_t A2RVMVec4Add(A2RVM* self, float x, float y, float z, float w);
size_t A2RVMVFCObjectAdd(A2RVM* self, A2RVMVFCObject* object);

void A2RVMTrackIndex(A2RVM* self, char* name, size_t index);

void A2RVMOpAdd(A2RVM* self, A2RVMOpcode opcode, size_t argOffset);

/*
void A2RVMBindVAAdd(A2RVM* self, size_t vaindex);
void A2RVMBindIAAdd(A2RVM* self, size_t iaindex);
void A2RVMBindProgramAdd(A2RVM* self, size_t textureindex);
void A2RVMBindTextureAdd(A2RVM* self, size_t textureindex);
void A2RVMBindTexture2Add(A2RVM* self, size_t textureindex);
void A2RVMBindLightmapAdd(A2RVM* self, size_t textureindex);
void A2RVMBindLightAdd(A2RVM* self, size_t lightindex);

void A2RVMLoadIdentityAdd(A2RVM* self, size_t unused);
void A2RVMLoadCameraAdd(A2RVM* self, size_t cameraindex);

void A2RVMTranslateAdd(A2RVM* self, size_t vec3index);
void A2RVMRotateAXAAdd(A2RVM* self, size_t vec4index);
void A2RVMRotateEULAdd(A2RVM* self, size_t vec3index);

void A2RVMSkipIfVFCFailAdd(A2RVM* self, size_t vfcobjindex);
void A2RVMDrawAdd(A2RVM* self, size_t unused);
*/