/*
 *  AEMesh.c
 *  Editor
 *
 *  Created by Oliver Daids on 11/17/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "AEMesh.h"

void AEMeshTriangleSubdivide4(AEMeshTriangle* self, AEMeshTriangle* t0, AEMeshTriangle* t1, AEMeshTriangle* t2, AEMeshTriangle* t3){
	for (int i=0; i<3; i++) t0->vertices[i]=AEMeshVertexLerp(self->vertices[(i+0)%3], self->vertices[(i+1)%3], 0.5);
	
	t1->vertices[2]=self->vertices[1];
	t1->vertices[1]=t0->vertices[0];
	t1->vertices[0]=t0->vertices[1];
	t2->vertices[2]=self->vertices[2];
	t2->vertices[1]=t0->vertices[1];
	t2->vertices[0]=t0->vertices[2];
	t3->vertices[2]=self->vertices[0];
	t3->vertices[1]=t0->vertices[2];
	t3->vertices[0]=t0->vertices[0];
}

int AEMeshTriangleSplit(AEMeshTriangle* self, AEVec4 plane, AEMeshTriangle* t0, AEMeshTriangle* t1, AEMeshTriangle* t2){
	//AEError("Unimplemented.");
	/*enum{
		none,
		coplanar,
		behind,
		infront
	};
	
	int hitmethod[3]={none,none,none};

	for(int i=0;i<3;i++){
		AEVec3 rayStart=self->vertices[i].position;
		AEVec3 rayEnd=self->vertices[(i+1)%3].position;//As opposite the definition of a ray this sounds.
		float lengthOfEdge=AEVec3DistanceBetween(rayEnd, rayStart);
		AEVec3 rayDirection=AEVec3Normalized(AEVec3Sub(rayEnd,rayStart));
		
		float d=AEPlanePointSignedDistance(plane, rayEnd);
		if(AEEpsilon > fabsf(d)) hitmethod[i]=coplanar;
		else if(d < 0) hitmethod[i]=behind;
		else hitmethod[i]=infront;
		
		if(hitmethod[i]==coplanar) continue;
		
		float t=AEPlaneRayIntersectionTime(plane, rayStart, rayDirection);
		if(t < AEEpsilon) continue;
		if(t+AEEpsilon > lengthOfEdge) continue;
		
		AEVec3 hit=AERayAtTime(rayStart, rayDirection, t);
		
	}*/
	return 0;
}

/*int AEMeshTriangleSplit(AEMeshTriangle* self, AEVec4 plane, AEMeshTriangle* t0, AEMeshTriangle* t1, AEMeshTriangle* t2){
	int splitcase=AEMeshTriangleSplitCaseNone;
	
	int numberOfVerticesBehind=0;
	int numberOfVerticesInFront=0;
	int numberOfVerticesCoplanar=0;
	
	float hitDistances[3];
	
	for(int i=0;i<3;i++){
		float d=AEPlanePointSignedDistance(plane, self->vertices[i].position);
		if(AEEpsilon > fabsf(d)) numberOfVerticesCoplanar++;
		else if(d < 0) numberOfVerticesBehind++;
		else numberOfVerticesInFront++;
		
	}
	
	if(numberOfVerticesCoplanar==3) splitcase=AEMeshTriangleSplitCaseCoplanar;
	else if(numberOfVerticesInFront and numberOfVerticesBehind) splitcase=numberOfVerticesCoplanar?AEMeshTriangleSplitCaseEdgeVertex:AEMeshTriangleSplitCaseEdgeEdge;
	
	switch (splitcase) {
		case AEMeshTriangleSplitCaseNone:
			break;
		case AEMeshTriangleSplitCaseCoplanar:
			break;
		case AEMeshTriangleSplitCaseEdgeEdge:
			
			break;
		case AEMeshTriangleSplitCaseEdgeVertex:
			break;
		default:
			AEError("Dude, your triangle splits weirdly, WTF?!");
			break;
	}
	return 0;
}*/