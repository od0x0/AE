#pragma once
#include "HeaderBegin.h"
#include "Core.h"
#include "FastMath3D.h"

//Unstable/Private implementation
typedef struct{
	//float x,y,z;
	AEVec3 position;
	AEVec3 rotation;///Private, don't use this
	AEQuat rotationq;//Kept in sync with above for speedups
	float near,far,fov;//0 fov==Ortho
	int w,h;
	float vfPlanes[6][4];
}AECamera;

static inline AEVec3 AECameraPositionGet(AECamera* cam){return cam->position;}
static inline void AECameraPositionSet(AECamera* cam,float x,float y,float z){cam->position.x=x;cam->position.y=y;cam->position.z=z;}
static inline AEVec3 AECameraRotationGet(AECamera* cam){
	return cam->rotation;
}

static inline void AECameraRotationSet(AECamera* cam,AEVec3 rotation){
	cam->rotation=rotation;
	///*
	//Invert it here, because I can't do it on the matrix end when we bind it
	rotation.x=-rotation.x;
	rotation.y=-rotation.y;
	rotation.z=-rotation.z;
	cam->rotationq=AEQuatFromEuler(rotation);
	//*/
}


static inline void AECameraBoundsSet(AECamera* cam,float near,float far,float fov){cam->near=near;cam->far=far;cam->fov=fov;}
static inline void AECameraViewportSet(AECamera* cam,int w,int h){cam->w=w;cam->h=h;}

extern AECamera AECameraActive;
AECamera* AECameraActiveGet(void);
void AECameraActiveSet(AECamera* cam);

static inline void AECameraBind(AECamera* cam){
	///*
	float m[16];
	AEQuatToMatrix4x4(cam->rotationq,m);
	glLoadMatrixf(m);
	glTranslatef(-cam->position.x,-cam->position.y,-cam->position.z);
	//*/
	
	/*
	glLoadIdentity();
	glRotatef(-cam->rotation.x,	1,0,0);
	glRotatef(-cam->rotation.y,	0,1,0);
	glRotatef(-cam->rotation.z,	0,0,1);
	glTranslatef(-cam->position.x,-cam->position.y,-cam->position.z);
	*/
}

void AECameraVFCalculate(AECamera* cam);
int AECameraVFCheckSphere(AECamera* cam,float x,float y,float z,float r);

#include "HeaderEnd.h"
