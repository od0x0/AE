#pragma once
#include "AE.h"

//Voxel Grid Octree

typedef struct AEVO AEVO;

struct AEVO{
	//The octree bounds
	AEVec3 min, max;
	//The voxel grid resolution, notice the limit of 255 in each direction.
	uint8_t w, h, d;
	//The voxel density grid, can be null when w, h, or d = 0
	uint8_t* voxels;
	//The children, always an array of 8, or null
	AEVO* children;
	//Userdata pointer
	//void* userdata;
};

void AEVOInit(AEVO* self);
void AEVODeinit(AEVO* self);

typedef float (*AEVOApplyFunc)(float density, float x, float y, float z, void* userdata);
void AEVOApply(AEVO* self, AEVOApplyFunc func, void* userdata);

/*
Does various things
1) If the resolution is too big, we subdivide
2) If the resolution is 0, then we clear it.
3) If the resolution is valid, we scale/resample.
*/
void AEVOResample(AEVO* self, uint64_t w, uint64_t h, uint64_t d, uint8_t splitThreshold);
//Gets the average density within the bounds
//float AEVOAverageDensityWithin(AEVO* self, AEVec3 min, AEVec3 max);

#ifdef AEVODebugDrawEnabled
static void AEVODebugDraw(AEVO* self){
	glTranslatef(self->min.x, self->min.y, self->min.z);
	const uint8_t w = self->w;
	const uint8_t h = self->h;
	const uint8_t d = self->d;
	
	glColor3f(1, 0, 0);
	const AEVec3 size = AEVec3Sub(self->max, self->min);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(size.x, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, size.y, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, size.z);
	glEnd();
	
	glColor3f(0, 0, 0);
	glBegin(GL_POINTS);
	for (int x=0; x<w; x++) 
	for (int y=0; y<h; y++) 
	for (int z=0; z<d; z++) 
	{
		const float alpha = self->voxels[x+(y*w)+(z*w*h)]/255.0f;
		glColor4f(0, 0, 0, alpha);
		glVertex3f(x, y, z);
	}
	glEnd();
	glColor3f(1, 1, 1);
	
	if(self->children) for (int i=0; i<8; i++) AEVODebugDraw(self->children + i);
	glTranslatef(-self->min.x, -self->min.y, -self->min.z);
}
#endif