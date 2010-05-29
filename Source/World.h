#pragma once

extern void* (*AEWorldEvent)(int event,int value,void* data);

#define AEWorldEventInit 1
#define AEWorldEventLoad 2
#define AEWorldEventRelease 3
#define AEWorldEventRender 4
#define AEWorldEventCheckSphereCollision 5
#define AEWorldEventCheckRayCollision 6
#define AEWorldEventCheckBoxCollision 7
#define AEWorldEventAdd 8
#define AEWorldEventPhysics 9