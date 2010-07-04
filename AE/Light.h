#pragma once
#include "HeaderBegin.h"
#include "Core.h"
#include "Camera.h"
#include "List.h"

typedef struct{
	float x,y,z;
	float r,g,b,radius;
	char on;
}AELight;

extern AEList* AELights;

void AELightsSort(void);
void AELightsToBoundLights(char maxActive,float x,float y,float z,float radius);
void AELightsInit(void);
void AELightsQuit(void);

/*
	Handles binding lights in OpenGL.  Due to graphics card restraints, 8 is the max at one time.  However, you can bind different lights before you draw different objects.
*/

extern AELight AEBoundLights[8];
extern char AEBoundLightCount;

void AEBoundLightsBind(void);

inline void AELightingToggle(int on){
	if(on) glEnable(GL_LIGHTING);
	else glDisable(GL_LIGHTING);
}

#include "HeaderEnd.h"