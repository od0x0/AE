#pragma once
#include "HeaderBegin.h"
typedef struct{
	float x,y,z;
	AEVec3 rotation;
	float near,far,fov;//0 fov==Ortho
	int w,h;
	float vfPlanes[6][4];
}AECamera;

void AECameraSetActive(AECamera* cam);
void AECameraVFCalculate(AECamera* cam);
int AECameraVFCheckSphere(AECamera* cam,float x,float y,float z,float r);
#include "HeaderEnd.h"
