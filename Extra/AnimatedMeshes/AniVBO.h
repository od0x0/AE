#pragma once
#include "Core.h"
#include "VBO.h"
#include "AniMesh.h"

//Unfinished!

typedef struct{
	int refcount;
	AEAniModel* model;
	AEVA texCoordArray, indexArray;
	AEList* animations;
}AEAniModelVBOClass;

AEAniModelVBOClass* AEAniModelVBOClassNew(char* filename);
void AEAniModelVBOClassDelete(AEAniModelVBOClass* self);

typedef struct{
	AEAniModelVBOClass* shared;
	AEVA vertexArray;
	AEAniAnimation* currentAnimation;
	AEAniAnimationInfo state;
}AEAniModelVBO;

AEAniModelVBO* AEAniModelVBOLoad(char* filename);
AEAniModelVBO* AEAniModelVBOClone(AEAniModelVBO* self);
void AEAniModelVBOStep(AEAniModelVBO* self,float step);
void AEAniModelVBODraw(AEAniModelVBO* self);
void AEAniModelVBODelete(AEAniModelVBO* self);

void AEAniModelVBOPlayAnimation(AEAniModelVBO* self,AEAniAnimation* animation);
void AEAniModelVBOPrepareVerts(AEAniModelVBO* self);
