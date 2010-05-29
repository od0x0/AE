#pragma once
#include "../../Core.h"
#include "../../VFChecking.h"

typedef struct{
	AEVec3 v[4];
	AEVec3 n[4];
	AEVec2 t[4];
	unsigned int texture;
}OctaTriangle;

typedef struct Octa{
	float x,y,z,size;
	OctaFace* faces;//null if has no faces
	struct Octa* children;//null if has no children
}Octa;

Octa* OctaChildren(Octa* parent);
void OctaRemoveChildren(Octa* octa);
void OctaDeform(Octa* octa,AEVec3 center,float radius,float strength,AEVec3i lock);
Octa* OctaGetAt(Octa* octa,float x,float y,float z);
void OctaRender(Octa* octa);//Brute force and slow