#pragma once
#include "AE.h"

typedef struct{
	AEVec2 texcoord;
	AEVec3 normal;
	AEVec3 position;
}AEMeshUtilityVertex;

static inline AEMeshUtilityVertex AEMeshUtilityVertexLerp(AEMeshUtilityVertex a, AEMeshUtilityVertex b, float t){
	AEMeshUtilityVertex r;
	r.position=AEVec3Lerp(a.position, b.position, t);
	r.normal=AEVec3Lerp(a.normal, b.normal, t);
	r.texcoord=AEVec3AsVec2(AEVec3Lerp(AEVec2AsVec3(a.texcoord), AEVec2AsVec3(b.texcoord), t));
	return r;
}

typedef struct{
	AEMeshUtilityVertex* vertices;
	size_t vertexCount;
	size_t vertexCountAllocated;
}AEMeshUtilityPolygonInfo;

static inline void AEMeshUtilityPolygonInfoAddVertex(AEMeshUtilityPolygonInfo* self, const AEMeshUtilityVertex* vertex){
	if(self->vertexCount+1 > self->vertexCountAllocated) return;
	self->vertices[self->vertexCount++]=*vertex;
}

static void AEMeshUtilitySplitPolygon(AEMeshUtilityPolygonInfo *self, AEPlane plane, AEMeshUtilityPolygonInfo* front, AEMeshUtilityPolygonInfo* back){
	AEMeshUtilityVertex vertexA = self->vertices[self->vertexCount-1];
	float sideA = AEPlanePointSignedDistance(plane, vertexA.position);
	for (size_t i=0; i<self->vertexCount; i++){
		AEMeshUtilityVertex vertexB = self->vertices[i];
		float sideB = AEPlanePointSignedDistance(plane, vertexB.position);
		if(sideB-AEEpsilon > 0){
			if(sideA+AEEpsilon < 0){
				const float t = AEPlaneLineSegmentIntersectionPercent(plane, vertexB.position, vertexA.position);
				const AEMeshUtilityVertex intersection = AEMeshUtilityVertexLerp(vertexB, vertexA, t);
				AEMeshUtilityPolygonInfoAddVertex(front, &intersection);
				AEMeshUtilityPolygonInfoAddVertex(back, &intersection);
			}
		}
		else if (sideB+AEEpsilon < 0){
			if (sideA-AEEpsilon > 0){
				const float t = AEPlaneLineSegmentIntersectionPercent(plane, vertexB.position, vertexA.position);
				const AEMeshUtilityVertex intersection = AEMeshUtilityVertexLerp(vertexB, vertexA, t);
				AEMeshUtilityPolygonInfoAddVertex(front, &intersection);
				AEMeshUtilityPolygonInfoAddVertex(back, &intersection);
			}
		}
		AEMeshUtilityPolygonInfoAddVertex(back, &vertexB);
		vertexA = vertexB;
		sideA = sideB;
	}
}
