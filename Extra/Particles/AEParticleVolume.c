#include "AEParticleVolume.h"

void AEParticleVolumeInit(AEParticleVolume* self, size_t quadCount){
	memset(self, 0, sizeof(AEParticleVolume));
	AEQuadPatchInit(& self->quadPatch, false, quadCount, NULL, NULL, NULL);
}

void AEParticleVolumeDeinit(AEParticleVolume* self){
	AEQuadPatchDeinit(& self->quadPatch);
	memset(self, 0, sizeof(AEParticleVolume));
}

void AEParticleVolumeStep(AEParticleVolume* self, double seconds, const AEVec3 up, const AEVec3 right){
	const AEVec3 movement = AEVec3Mul(self->movement, AEVec3FromSingle(seconds));
	for (size_t i=0; i<self->quadPatch.quadCount; i++) {
		const AEVec3 p=self->quadPatch.quads[i].center;
		
		if(AEVec3IsBetween(p, self->min, self->max))
			self->quadPatch.quads[i].center=AEVec3Add(p, movement);
		else
			self->quadPatch.quads[i].center=AEVec3RandomBetween(self->min, self->max);
	}
	AEQuadPatchUpdate(& self->quadPatch, up, right);
}

void AEParticleVolumeRender(AEParticleVolume* self){
	AEQuadPatchRender(& self->quadPatch);
}