#include "AEVO.h"

//////////////////////////////

static AEVO* Subdivide(AEVO* self){
	if(not self) return NULL;
	if(self->children) return NULL;
	
	self->children = calloc(8, sizeof(AEVO));
	
	const AEVec3 childSize = AEVec3Mul(AEVec3Sub(self->max, self->min), AEVec3From1(0.5));
	
	for (int x=0; x<2; x++) 
	for (int y=0; y<2; y++) 
	for (int z=0; z<2; z++) 
	{
		size_t i = x+(y*2)+(z*2*2);
		//AEVec3From3(x/2.0, y/2.0, z/2.0)
		const AEVec3 offset = AEVec3Mul(childSize, AEVec3From3(x, y, z));
		self->children[i].min = AEVec3Add(self->min, offset);
		self->children[i].max = AEVec3Add(self->children[i].min, childSize);
	}
	return self->children;
}

//////////////////////////////

void AEVOInit(AEVO* self){
	memset(self, 0, sizeof(AEVO));
}

void AEVODeinit(AEVO* self){
	if(not self) return;
	if(self->children) for (int i=0; i<8; i++) AEVODeinit(self->children + i);
	free(self->children);
	free(self->voxels);
}

void AEVOApply(AEVO* self, AEVOApplyFunc func, void* userdata){
	const uint8_t w = self->w;
	const uint8_t h = self->h;
	const uint8_t d = self->d;
	
	const AEVec3 octaSize = AEVec3Sub(self->max, self->min);
	const AEVec3 cellSize = AEVec3Div(octaSize, AEVec3From3(w, h, d));
	const AEVec3 halfCellSize = AEVec3Mul(cellSize, AEVec3From1(0.5));
	
	for (int x=0; x<w; x++) 
	for (int y=0; y<h; y++) 
	for (int z=0; z<d; z++) 
	{
		const float density = self->voxels[x+(y*w)+(z*w*h)]/255.0f;
		const AEVec3 cellCenter = AEVec3Add(self->min, AEVec3Add(AEVec3Add(cellSize, AEVec3From3(x, y, z)), halfCellSize));
		self->voxels[x+(y*w)+(z*w*h)]=(uint8_t)(AEClamp(func(density, cellCenter.x, cellCenter.y, cellCenter.z, userdata), 0, 1)*255);
	}
	if(self->children) for (int i=0; i<8; i++) AEVOApply(self->children + i, func, userdata);
}

/*
//Subdivides the octree and gives it children if it doesn't have them.
AEVO* AEVOSubdivide(AEVO* self){
	if(not self) return;
	Subdivide(self);
	AEVOResample(self, self->w, self->h, self->d);
	return self->children;
}
*/

//Changes to a new resolution
void AEVOResample(AEVO* self, uint64_t w, uint64_t h, uint64_t d, uint8_t splitThreshold){
	if(not self) return;
	if(splitThreshold==0) splitThreshold = 255;
	
	if(w==0 and h==0 and d==0) {
		if(self->children) for (int i=0; i<8; i++) AEVODeinit(self->children + i);
		free(self->children);
		free(self->voxels);
		self->w = 0;
		self->h = 0;
		self->d = 0;
		self->children = NULL;
		self->voxels = NULL;
		return;
	}
	
	if((w > splitThreshold) or (h > splitThreshold) or (d > splitThreshold)) {
		Subdivide(self);
		for (int i=0; i<8; i++) 
			AEVOResample(self->children + i, w/2, h/2, d/2, splitThreshold);
		free(self->voxels);
		self->voxels = NULL;
		self->w = 0;
		self->h = 0;
		self->d = 0;
	}
	else {
		uint8_t* voxels = calloc(w*h*d, sizeof(uint8_t));
		
		if(self->children){
			//Do resampling from children
			for (int i=0; i<8; i++) AEVODeinit(self->children + i);
			free(self->children);
			self->children = NULL;
		}
		else if(self->voxels){
			//Do resampling from old voxels
		}
		
		free(self->voxels);
		self->voxels = voxels;
		self->w = w;
		self->h = h;
		self->d = d;
	}
}
/*
//AABB collision
static bool AABBsTouch(AEVec3 center1, AEVec3 size1, AEVec3 center2, AEVec3 size2){
	
}

//Gets the average density within the bounds
float AEVOAverageDensityWithin(AEVO* self, AEVec3 min, AEVec3 max){
	AEVec3 center1, size1, center2, size2;
	
	size1 = AEVec3Mul(AEVec3Sub(self->max, self->min), AEVec3From1(0.5));
	center1 = AEVec3Add(self->min, size1);
	size2 = AEVec3Mul(AEVec3Sub(max, min), AEVec3From1(0.5));
	center2 = AEVec3Add(min, size2);
	if(not AABBsTouch(center1, size1, center2, size2)) return 0;

	double sum = 0;
	double count = 0;
	
	if(self->children) for (int i=0; i<8; i++) {
		sum+=AEVOAverageDensityWithin(self, min, max);
		count++;
	}
	
	if(self->voxels) 
	for (uint8_t x=0; x<self->w; x++) 
	for (uint8_t y=0; y<self->h; y++) 
	for (uint8_t z=0; z<self->d; z++) 
	{
		size1 = AEVec3Mul(AEVec3Sub(self->max, self->min), AEVec3From1(0.5));
		center1 = AEVec3Add(self->min, size1);
		size2 = AEVec3Mul(AEVec3Sub(max, min), AEVec3From1(0.5));
		center2 = AEVec3Add(min, size2);
		if(not AABBsTouch(center1, size1, center2, size2)) continue;
		
		sum+=self->voxels[x+(y*self->w)+(z*self->w*self->h)];
		count++;
	}
	
	return sum/count;
}
*/