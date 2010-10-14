#pragma once
#include "../Core.h"


//Don't depend on the definition of these structures

typedef struct{unsigned int v[3],n[3],t[3],t2[3];}AERawMeshFace;
typedef struct{
	struct{unsigned int v,f,t,n;}count;
	AEVec3* v;
	AEVec2* t;
	AEVec2* t2;
	AEVec3* n;
	AERawMeshFace* f;
}AERawMesh;

AERawMesh* AERawMeshLoad(const char* filename);
void AERawMeshDelete(AERawMesh* m);