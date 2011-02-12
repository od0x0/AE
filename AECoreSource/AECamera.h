#pragma once
#include "AECore.h"
#include "AEMath.h"

typedef struct AECamera AECamera;

AECamera* AECameraNew(void);
//AECamera* AECameraRetain(AECamera* cam); (deprecated)
void AECameraDelete(AECamera* cam);

void AECameraGetPosition(AECamera* cam, AEVec3* v3);
void AECameraSetPosition(AECamera* cam, float x, float y, float z);
void AECameraGetRotation(AECamera* cam, AEVec3* v3);
void AECameraSetRotation(AECamera* cam, float x, float y, float z);

void AECameraSetBounds(AECamera* cam, float near, float far, float fov);
void AECameraGetBounds(AECamera* cam, float* near, float* far, float* fov);
void AECameraSetViewport(AECamera* cam, int w, int h);
void AECameraGetViewport(AECamera* cam, int* w, int* h);

AECamera* AECamerasGetActive(void);
void AECamerasSetActive(AECamera* cam);//Does a copy of it's value!

void AECameraBind(AECamera* cam);

void AECameraSetVFCEnabled(AECamera* cam, bool enabled);
bool AECameraGetVFCEnabled(AECamera* cam);

void AECameraVFCalculate(AECamera* cam);
int AECameraVFCheckSphere(AECamera* cam, float x, float y, float z, float r);
