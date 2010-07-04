#pragma once
#include "HeaderBegin.h"
#include "Core.h"

//Unstable implementation
typedef struct{
	float x,y,z;
	AEVec3 rotation;///Private, don't use this
	float near,far,fov;//0 fov==Ortho
	int w,h;
	float vfPlanes[6][4];
}AECamera;

inline AEVec3 AECameraRotationGet(AECamera* cam){return cam->rotation;}
inline void AECameraRotationSet(AECamera* cam,AEVec3 rotation){cam->rotation=rotation;}


extern AECamera AECameraActive;
AECamera* AECameraActiveGet(void);
void AECameraActiveSet(AECamera* cam);


void AECameraVFCalculate(AECamera* cam);
int AECameraVFCheckSphere(AECamera* cam,float x,float y,float z,float r);

#include "HeaderEnd.h"
