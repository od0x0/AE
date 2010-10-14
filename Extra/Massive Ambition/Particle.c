#include "Particle.h"

A2ParticleEmitter* A2ParticleEmitterNew(unsigned int particleCount){
	A2ParticleEmitter* emitter=calloc(1,sizeof(A2ParticleEmitter));
	emitter->particleCount=particleCount;
	
	emitter->positions=calloc(particleCount, sizeof(AEVec3));
	for(size_t i=0; i < emitter->particleCount; i++) emitter->positions[i]=AEVec3RandomBetween(emitter->emitterMin,emitter->emitterMax);
			
	emitter->times=calloc(particleCount, sizeof(float));
	for(size_t i=0; i < emitter->particleCount; i++) emitter->times[i]=AERandomBetween(0, emitter->maxtime);
	
	AEVAInit(&(emitter->va), false, AEVAVBOTypeStream, 0, 0);
	
	void* data=NULL;
	data=AEVAMap(&emitter->va, emitter->particleCount*3, GL_WRITE_ONLY);
	memset(data, 0, sizeof(AEVec3)*emitter->particleCount);
	AEVAUnmap(&emitter->va);
	
	AEVAInit(&(emitter->ta),false, AEVAVBOTypeStream,0,0);
	
	data=AEVAMap(&emitter->ta, emitter->particleCount*2, GL_WRITE_ONLY);
	//memset(data, 0, sizeof(AEVec2)*emitter->particleCount);
	AEVec2* ts=data;
	static AEVec2 texcoords[4]={{0,0},{1,0},{1,1},{0,1}};
	for(size_t i=0;i<emitter->particleCount;i++)
		for (char j=0; j<4; j++)
			ts[i*4+j]=texcoords[j];
	AEVAUnmap(&emitter->ta);
	
	return emitter;
}

void A2ParticleEmitterDelete(A2ParticleEmitter* emitter){
	if(emitter==NULL) return;
	AEVADeinit(&emitter->ta);
	AEVADeinit(&emitter->va);
	free(emitter->times);
	free(emitter->positions);
	free(emitter);
}

void A2ParticleEmitterGenerateBillboards(A2ParticleEmitter* emitter){
	float modelview[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

	const AEVec3 right=AEVec3FromCoords(modelview[0], modelview[4], modelview[8]);
	const AEVec3 up=AEVec3FromCoords(modelview[1], modelview[5], modelview[9]);
	
	const AEVec3 topright=AEVec3Add(right, up);
	const AEVec3 topleft=AEVec3Sub(right, up);
	const AEVec3 bottomleft=AEVec3Mul(topright,AEVec3FromCoords(-1, -1, -1));
	const AEVec3 bottomright=AEVec3Mul(topleft,AEVec3FromCoords(-1, -1, -1));
	
	AEVec3* v=AEVAMap(&emitter->va, emitter->particleCount*3, GL_WRITE_ONLY);
	for(size_t i=0; i < emitter->particleCount; i++){
		const AEVec3 center=emitter->positions[i];
		/*v[i*4+0]=AEVec3Sub(center, AEVec3Add(right, up));
		v[i*4+1]=AEVec3Add(center, AEVec3Sub(right, up));
		v[i*4+2]=AEVec3Add(center, AEVec3Add(right, up));
		v[i*4+3]=AEVec3Sub(center, AEVec3Sub(right, up));*/
		v[i*4+0]=AEVec3Add(center, bottomleft);
		v[i*4+1]=AEVec3Add(center, bottomright);
		v[i*4+2]=AEVec3Add(center, topright);
		v[i*4+3]=AEVec3Add(center, topleft);
	}
	AEVAUnmap(&emitter->va);
}

void A2ParticleEmitterStep(A2ParticleEmitter* emitter,float stepInSeconds){
	if(emitter->forceIsOutward) for(size_t i=0; i < emitter->particleCount; i++){
		emitter->times[i]+=stepInSeconds;
		if(emitter->times[i] > emitter->maxtime){
			emitter->positions[i]=AEVec3RandomBetween(emitter->emitterMin,emitter->emitterMax);
			emitter->times[i]=AERandomBetween(0, emitter->maxtime);
			continue;
		}
	
		const AEVec3 oldposition=emitter->positions[i];
		const float factor=emitter->forceFactor*stepInSeconds;
		const AEVec3 weighting={factor, factor, factor};
		const AEVec3 direction=AEVec3Sub(oldposition, weighting);
		const AEVec3 move=AEVec3Mul(direction, weighting);
		emitter->positions[i]=AEVec3Add(oldposition, move);
	}
	else for(size_t i=0; i < emitter->particleCount; i++){
		emitter->times[i]+=stepInSeconds;
		if(emitter->times[i] > emitter->maxtime){
			emitter->positions[i]=AEVec3RandomBetween(emitter->emitterMin,emitter->emitterMax);
			emitter->times[i]=AERandomBetween(0, emitter->maxtime);
			continue;
		}
		
		const AEVec3 oldposition=emitter->positions[i];
		const float factor=emitter->forceFactor*stepInSeconds;
		const AEVec3 weighting={factor, factor, factor};
		const AEVec3 move=AEVec3Mul(emitter->forceVector, weighting);
		emitter->positions[i]=AEVec3Add(oldposition, move);
	}
	A2ParticleEmitterGenerateBillboards(emitter);
}

void A2ParticleEmitterRender(A2ParticleEmitter* emitter){
	glDisable(GL_CULL_FACE);
	AETextureBind(emitter->texture);
	/*AEVABindIndices(NULL);
	AEVABindTexcoord(&emitter->ta);
	AEVABindVertex(&emitter->va);
	//AEVADraw(0,emitter->particleCount);
	glDrawArrays(GL_QUADS,0,emitter->particleCount);
	AEVABindTexcoord(NULL);
	AEVABindVertex(NULL);*/
	//*
	float modelview[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);
	
	glBegin(GL_QUADS);

	const AEVec3 right=AEVec3FromCoords(modelview[0], modelview[4], modelview[8]);
	const AEVec3 up=AEVec3FromCoords(modelview[1], modelview[5], modelview[9]);

	for(size_t i=0; i < emitter->particleCount; i++){
		const AEVec3 center=emitter->positions[i];
		AEVec3 v[4];
		v[0]=AEVec3Sub(center, AEVec3Add(right, up));
		v[1]=AEVec3Add(center, AEVec3Sub(right, up));
		v[2]=AEVec3Add(center, AEVec3Add(right, up));
		v[3]=AEVec3Sub(center, AEVec3Sub(right, up));
		AEVec2 t[4]={{0,0},{1,0},{1,1},{0,1}};
		for(int j=0;j<4;j++){
			glTexCoord2f(t[j].x, t[j].y);
			glVertex3f(v[j].x, v[j].y, v[j].z);
		}
	}
	//*/
	glEnable(GL_CULL_FACE);
}