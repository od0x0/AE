#pragma once
#include "AE.h"

typedef struct AEImage3D AEImage3D;
struct AEImage3D{
	uint32_t w, h, d;
	uint8_t* voxels;
};

void AEImage3DInit(AEImage3D* self, uint32_t w, uint32_t h, uint32_t d);
void AEImage3DDeinit(AEImage3D* self);

void AEImage3DBlit(AEImage3D* self, uint32_t x, uint32_t y, uint32_t z, AEImage3D* other);
void AEImage3DResampleFrom(AEImage3D* self, AEImage3D* other);
void AEImage3DResize(AEImage3D* self, uint32_t w, uint32_t h, uint32_t d);

void AEImage3DSetVoxel(AEImage3D* self, uint32_t x, uint32_t y, uint32_t z, uint8_t to);
uint8_t AEImage3DGetVoxel(AEImage3D* self, uint32_t x, uint32_t y, uint32_t z);
