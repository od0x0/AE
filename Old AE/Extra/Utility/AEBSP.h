#pragma once
#include "AE.h"

//General Utility Routines

static inline void AEBSPLerpVertex(float* r, float* a, float* b, float t, size_t floatsPerVertex){
	for (size_t i=0; i<floatsPerVertex; i++) r[i]=AELerp(a[i], b[i], t);
}

typedef AEArray(float) AEBSPPolygon;

static inline void AEBSPPolygonSplit(AEBSPPolygon* polygon, AEBSPPolygon* inside, AEBSPPolygon* outside, AEPlane plane, size_t floatsPerVertex, size_t positionVec3Index){
	const size_t vertexCount=AEArrayLength(polygon)/floatsPerVertex;
	float v[floatsPerVertex];
	memset(v, 0, sizeof(float)*floatsPerVertex);
	float* vertices=AEArrayAsCArray(polygon);
	
	AEVec3 pointA={vertices[floatsPerVertex*(vertexCount-1)+positionVec3Index], vertices[floatsPerVertex*(vertexCount-1)+positionVec3Index+1], vertices[floatsPerVertex*(vertexCount-1)+positionVec3Index+2]};
	float sideA=AEPlanePointSignedDistance(plane, pointA);
	
	for (size_t i=0; i<vertexCount; i++) {
		size_t index=floatsPerVertex*i;
		AEVec3 p={vertices[index+positionVec3Index], vertices[index+positionVec3Index+1], vertices[index+positionVec3Index+2]};
		//AEPlane
		
	}
}