#include "AEImage3D.h"

#define AEImage3DWrapEnabled 0

void AEImage3DInit(AEImage3D* self, uint32_t w, uint32_t h, uint32_t d){
	memset(self, 0, sizeof(AEImage3D));
	self->w=w;
	self->h=h;
	self->d=d;
	self->voxels=calloc(w*h*d, sizeof(uint8_t));
}

void AEImage3DDeinit(AEImage3D* self){
	if(not self) return;
	free(self->voxels);
}

void AEImage3DBlit(AEImage3D* self, uint32_t xStart, uint32_t yStart, uint32_t zStart, AEImage3D* other){
	for (uint32_t x=0; x<other->w; x++)
	for (uint32_t y=0; y<other->h; y++)
	for (uint32_t z=0; z<other->d; z++)
	{
		AEImage3DSetVoxel(self, x+xStart, y+yStart, z+zStart, AEImage3DGetVoxel(other, x, y, z));
	}
}

void AEImage3DResampleFrom(AEImage3D* self, AEImage3D* other){
	struct{double x,y,z;} ratio={(double)self->w/other->w, (double)self->h/other->h, (double)self->d/other->d};
	for (uint32_t x=0; x<self->w; x++)
	for (uint32_t y=0; y<self->h; y++)
	for (uint32_t z=0; z<self->d; z++)
	{
		AEImage3DSetVoxel(self, x, y, z, AEImage3DGetVoxel(other, x*ratio.x, y*ratio.y, z*ratio.z));
	}
}

void AEImage3DResize(AEImage3D* self, uint32_t w, uint32_t h, uint32_t d){
	AEImage3D resized;
	AEImage3DInit(& resized, w, h, d);
	AEImage3DResampleFrom(& resized, self);
	AEImage3DDeinit(self);
	memcpy(self, & resized, sizeof(AEImage3D));
}

void AEImage3DSetVoxel(AEImage3D* self, uint32_t x, uint32_t y, uint32_t z, uint8_t to){
	if(AEImage3DWrapEnabled){
		x %= self->w;
		y %= self->h;
		z %= self->d;
	}
	else if(x > self->w or y > self->h or z > self->d) return;
	self->voxels[x+(y*self->w)+(z*self->w*self->h)] = to;
}

uint8_t AEImage3DGetVoxel(AEImage3D* self, uint32_t x, uint32_t y, uint32_t z){
	if(AEImage3DWrapEnabled){
		x %= self->w;
		y %= self->h;
		z %= self->d;
	}
	else if(x > self->w or y > self->h or z > self->d) return 0;
	return self->voxels[x+(y*self->w)+(z*self->w*self->h)];
}
