#pragma once
#include "Core.h"
void AECrosshairDraw(float size);
void AEFPSCameraMouseMoveSetup(float speed,float upmax,float downmax);

AEVec3 AEVBOCenter(AEVBO* vbo);
AEVec3 AEVBOAABB(AEVBO* vbo);

void AEVBOTranslate(AEVBO* vbo,AEVec3 move);

int AETriBoxOverlap(float boxcenter[3],float boxhalfsize[3],float triverts[3][3]);
int AEObjectCollidesWithVBO(AEObject* o,AEVBO* vbo,AEVec3 vboPos);

void AEObjectApplyFriction(AEObject* o,float amount);
void AEObjectApply3DFriction(AEObject* o,AEVec3 amount);

void AEObjectAimVelocityAt(AEObject* o,AEVec3* target,AEVec3 speed);