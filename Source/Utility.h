#pragma once
#include "Core.h"
void AECrosshairDraw(float size);
void AEFPSCameraMouseMoveSetup(float speed,float upmax,float downmax);

AEVec3 AEVBOCalculateOffset(AEVBO* vbo);
AEVec3 AEVBOCalculateSize(AEVBO* vbo);
void AEVBOTranslate(AEVBO* vbo,AEVec3 move);

int AETriBoxOverlap(float boxcenter[3],float boxhalfsize[3],float triverts[3][3]);
int AEObjectCollidesWithVBO(AEObject* o,AEVBO* vbo,AEVec3 vboPos);
void AEObjectApplyFriction(AEObject* o,float amount);