#pragma once
#include "AECore.h"
#include "AEMath.h"

typedef struct AECamera AECamera;

AECamera* AECameraNew(void);
AECamera* AECameraRetain(AECamera* cam);
void AECameraDelete(AECamera* cam);

void AECameraPositionGet(AECamera* cam,AEVec3* v3);
void AECameraPositionSet(AECamera* cam,float x,float y,float z);
void AECameraRotationGet(AECamera* cam, AEVec3* v3);
void AECameraRotationSet(AECamera* cam,float x,float y,float z);

void AECameraBoundsSet(AECamera* cam,float near,float far,float fov);
void AECameraBoundsGet(AECamera* cam,float* near,float* far,float* fov);
void AECameraViewportSet(AECamera* cam,int w,int h);
void AECameraViewportGet(AECamera* cam,int* w,int* h);

AECamera* AECameraActiveGet(void);
void AECameraActiveSet(AECamera* cam);

void AECameraBind(AECamera* cam);

void AECameraVFCSet(AECamera* cam, bool enabled);
bool AECameraVFCGet(AECamera* cam);

void AECameraVFCalculate(AECamera* cam);
int AECameraVFCheckSphere(AECamera* cam,float x,float y,float z,float r);