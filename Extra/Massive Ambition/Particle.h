#pragma once
#include "AEMaterial.h"
#include "AEVA.h"

typedef struct{
	float maxtime;
	
	AEVec3 emitterMin, emitterMax, forceVector;
	float forceFactor;
	unsigned int texture;
	
	unsigned int particleCount;
	AEVec3* positions;
	float* times;
	
	AEVA va, ta;
	
	char forceIsOutward;
}A2ParticleEmitter;

A2ParticleEmitter* A2ParticleEmitterNew(unsigned int particleCount);

void A2ParticleEmitterDelete(A2ParticleEmitter* emitter);

void A2ParticleEmitterGenerateBillboards(A2ParticleEmitter* emitter);

void A2ParticleEmitterStep(A2ParticleEmitter* emitter,float stepInSeconds);

void A2ParticleEmitterRender(A2ParticleEmitter* emitter);