#pragma once
#include "AEQuadPatch.h"

typedef struct{
	AEQuadPatch quadPatch;
	AEVec3 min, max;
	AEVec3 movement;
}AEParticleVolume;

void AEParticleVolumeInit(AEParticleVolume* self, size_t quadCount);
void AEParticleVolumeDeinit(AEParticleVolume* self);
void AEParticleVolumeStep(AEParticleVolume* self, double seconds, const AEVec3 cameraPosition, const AEVec3 up, const AEVec3 right);
void AEParticleVolumeRender(AEParticleVolume* self);
