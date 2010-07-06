#pragma once
#include "HeaderBegin.h"
#include "Core.h"

//Unstable/Private implementation
typedef struct{
	//float x,y,z;
	AEVec3 position;
	AEVec3 rotation;///Private, don't use this
	float near,far,fov;//0 fov==Ortho
	int w,h;
	float vfPlanes[6][4];
}AECamera;

static inline AEVec3 AECameraPositionGet(AECamera* cam){return cam->position;}
inline void AECameraPositionSet(AECamera* cam,AEVec3 position){cam->position=position;}
inline AEVec3 AECameraRotationGet(AECamera* cam){return cam->rotation;}
inline void AECameraRotationSet(AECamera* cam,AEVec3 rotation){cam->rotation=rotation;}
inline void AECameraBoundsSet(AECamera* cam,float near,float far,float fov){cam->near=near;cam->far=far;cam->fov=fov;}
inline void AECameraViewportSet(AECamera* cam,int w,int h){cam->w=w;cam->h=h;}

extern AECamera AECameraActive;
AECamera* AECameraActiveGet(void);
void AECameraActiveSet(AECamera* cam);


void AECameraVFCalculate(AECamera* cam);
int AECameraVFCheckSphere(AECamera* cam,float x,float y,float z,float r);

#include "HeaderEnd.h"
