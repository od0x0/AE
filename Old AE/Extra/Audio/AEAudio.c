#include "AEAudio.h"

void AEAudioStreamInit(AEAudioStream* self){
	memset(self, 0, sizeof(AEAudioStream));
	alGenSources(1, & self->source);
	alGenBuffers(2, self->buffers);
	self->bufferSize=4096*8;
	self->shouldLoop=true;//We loop by default
}

void AEAudioStreamDeinit(AEAudioStream* self){
	alDeleteSources(1, & self->source);
	alDeleteBuffers(2, self->buffers);
	stb_vorbis_close(self->stream);
	memset(self, 0, sizeof(AEAudioStream));
}

bool AEAudioStreamStream(AEAudioStream* self, ALuint buffer){
	//Uncomment this to avoid VLAs
	//#define BUFFER_SIZE 4096*32
	#ifndef BUFFER_SIZE//VLAs ftw
	#define BUFFER_SIZE (self->bufferSize)
	#endif
	ALshort pcm[BUFFER_SIZE];
	int  size = 0;
	int  result = 0;
	
	while(size < BUFFER_SIZE){
		result = stb_vorbis_get_samples_short_interleaved(self->stream, self->info.channels, pcm+size, BUFFER_SIZE-size);
		if(result > 0) size += result*self->info.channels;
		else break;
	}
	
	if(size == 0) return false;
	
	alBufferData(buffer, self->format, pcm, size*sizeof(ALshort), self->info.sample_rate);
	self->totalSamplesLeft-=size;
	#undef BUFFER_SIZE
	
	return true;
}

bool AEAudioStreamOpen(AEAudioStream* self, const char* filename){
	self->stream = stb_vorbis_open_filename((char*)filename, NULL, NULL);
	if(not self->stream) return false;
	// Get file info
	self->info = stb_vorbis_get_info(self->stream);
	if(self->info.channels == 2) self->format = AL_FORMAT_STEREO16;
	else self->format = AL_FORMAT_MONO16;
	
	if(not AEAudioStreamStream(self, self->buffers[0])) return false;
	if(not AEAudioStreamStream(self, self->buffers[1])) return false;
	alSourceQueueBuffers(self->source, 2, self->buffers);
	alSourcePlay(self->source);
	
	self->totalSamplesLeft=stb_vorbis_stream_length_in_samples(self->stream) * self->info.channels;
	
	return true;
}

bool AEAudioStreamUpdate(AEAudioStream* self){
	ALint processed=0;
	
    alGetSourcei(self->source, AL_BUFFERS_PROCESSED, &processed);

    while(processed--){
        ALuint buffer=0;
        
        alSourceUnqueueBuffers(self->source, 1, &buffer);
		
		if(not AEAudioStreamStream(self, buffer)){
			bool shouldExit=true;
			
			if(self->shouldLoop){
				stb_vorbis_seek_start(self->stream);
				self->totalSamplesLeft=stb_vorbis_stream_length_in_samples(self->stream) * self->info.channels;
				shouldExit=not AEAudioStreamStream(self, buffer);
			}
			
			if(shouldExit) return false;
		}
		alSourceQueueBuffers(self->source, 1, &buffer);
	}
	
	return true;
}

void AEAudioContextInit(AEAudioContext* self){
	memset(self, 0, sizeof(AEAudioContext));
	self->alcdevice=alcOpenDevice(NULL);
	self->alccontext=alcCreateContext(self->alcdevice, NULL);
	alcMakeContextCurrent(self->alccontext);
	AEArrayInit(& self->sources);
	AEArrayInit(& self->streams);
}

void AEAudioContextDeinit(AEAudioContext* self){
	size_t length=AEArrayLength(& self->sources);
	if(length) alDeleteSources(length, AEArrayAsCArray(& self->sources));
	AEArrayDeinit(& self->sources);
	AEArrayDeinit(& self->streams);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(self->alccontext);
	alcCloseDevice(self->alcdevice);
	memset(self, 0, sizeof(AEAudioContext));
}

void AEAudioContextSetPosition(AEAudioContext* self, float x, float y, float z){
	AEVec3 v3={x,y,z};
	alListenerfv(AL_POSITION, (ALfloat*)&v3);
	self->position=v3;
}

void AEAudioContextGetPosition(AEAudioContext* self, AEVec3* v3){
	*v3=self->position;
}

void AEAudioContextSetRotation(AEAudioContext* self, float x, float y, float z){
	AEVec3 v3={x,y,z};
	self->rotation=v3;
	AEQuat q=AEQuatFromEuler(v3);
	AEVec3 up=AEQuatMulVec3(q, AEVec3From3(0, 1, 0));
	AEVec3 at=AEQuatMulVec3(q, AEVec3From3(0, 0, -1));
	ALfloat orientation[] = {at.x, at.y, at.z , up.x, up.y, up.z};
	alListenerfv(AL_ORIENTATION, orientation);
}

void AEAudioContextGetRotation(AEAudioContext* self, AEVec3* v3){
	*v3=self->rotation;
}

void AEAudioContextSetVelocity(AEAudioContext* self, float x, float y, float z){
	AEVec3 v3={x,y,z};
	alListenerfv(AL_VELOCITY, (ALfloat*)&v3);
	self->velocity=v3;
}

void AEAudioContextGetVelocity(AEAudioContext* self, AEVec3* v3){
	*v3=self->velocity;
}

ALuint AEAudioContextBufferLoad(AEAudioContext* self, const char* filename){
	ALuint buffer=0;
	alGenBuffers(1, &buffer);
	
	stb_vorbis *stream = stb_vorbis_open_filename((char*)filename, NULL, NULL);
	if(not stream) return 0;

	stb_vorbis_info info = stb_vorbis_get_info(stream);
	ALenum format;
	if(info.channels == 2) format = AL_FORMAT_STEREO16;
	else format = AL_FORMAT_MONO16;

	size_t sampleCount = stb_vorbis_stream_length_in_samples(stream) * info.channels;
	void* data = malloc(sizeof(ALushort)*sampleCount);

	stb_vorbis_get_samples_short_interleaved(stream, info.channels, data, (int)sampleCount);
	stb_vorbis_close(stream);

	alBufferData(buffer, format, data, sampleCount * sizeof(ALushort), info.sample_rate);
	free(data);
	return buffer;
}

void AEAudioContextBufferDelete(AEAudioContext* self, ALuint buffer){
	alDeleteBuffers(1, &buffer);
}

ALuint AEAudioContextStreamLoad(AEAudioContext* self, const char* filename){
	ALuint streamID=0;
	AEAudioStream stream;
	AEAudioStreamInit(& stream);
	if(not AEAudioStreamOpen(& stream, filename)){
		AEAudioStreamDeinit(& stream); 
		return 0;
	}
	size_t length=AEArrayLength(& self->streams);
	for (size_t i=0; i<length; i++) 
		if(streamID < AEArrayAsCArray(& self->streams)[i].ID) streamID=AEArrayAsCArray(& self->streams)[i].ID;
	streamID++;
	if(streamID==0) streamID++;
	stream.ID=streamID;
	AEArrayAddBytes(& self->streams, & stream);
	return streamID;
}

AEAudioStream* AEAudioContextGetStreamForID(AEAudioContext* self, ALuint streamID){
	size_t length=AEArrayLength(& self->streams);
	for (size_t i=0; i<length; i++) {
		//printf("#%i: %i\n", (int)i, (int)AEArrayAsCArray(& self->streams)[i].ID);
		if(streamID==AEArrayAsCArray(& self->streams)[i].ID) return AEArrayAsCArray(& self->streams)+i;
	}
	return NULL;
}

void AEAudioContextStreamDelete(AEAudioContext* self, ALuint streamID){
	AEAudioStream* stream=AEAudioContextGetStreamForID(self, streamID);
	AEAudioStreamDeinit(stream);
	AEArrayRemoveBytes(& self->streams, stream);
}

ALuint AEAudioContextSourceNew(AEAudioContext* self, ALuint buffer){
	ALuint source=0;
	alGenSources(1, &source);
	if (alGetError() != AL_NO_ERROR) return 0;
	AEArrayAddBytes(& self->sources, &source);
	
	alSourcei(source, AL_BUFFER, buffer);
	AEAudioContextSourceSetLooping(self, source, false);
	AEAudioContextSourceSetVelocity(self, source, 0, 0, 0);
	AEAudioContextSourceSetPosition(self, source, 0, 0, 0);
	AEAudioContextSourceSetPitch(self, source, 1);
	AEAudioContextSourceSetVolume(self, source, 1);
	AEAudioContextSourceSetPaused(self, source, false);
	return source;
}

void AEAudioContextSourceDelete(AEAudioContext* self, ALuint source){
	AEArrayRemoveBytes(& self->sources, &source);
	alSourceStop(source);
	alDeleteSources(1, &source);
}

void AEAudioContextUpdateStreams(AEAudioContext* self){
	size_t length=AEArrayLength(& self->streams);
	for (size_t i=0; i<length; i++){
		AEAudioStream* stream=AEArrayAsCArray(& self->streams)+i;
		if(not AEAudioStreamUpdate(stream)) AEAudioContextStreamDelete(self, stream->ID);
	}
}

void AEAudioContextSourceSetPaused(AEAudioContext* self, ALuint source, bool to){
	if(not to) alSourcePlay(source);
	else alSourcePause(source);
}

bool AEAudioContextSourceGetPaused(AEAudioContext* self, ALuint source){
	ALint state=AL_PLAYING;
	alGetSourcei(source, AL_SOURCE_STATE, &state);
	return state not_eq AL_PLAYING;
}

void AEAudioContextSourceSetVolume(AEAudioContext* self, ALuint source, float amount){
	alSourcef(source, AL_GAIN, amount);
}

float AEAudioContextSourceGetVolume(AEAudioContext* self, ALuint source){
	ALfloat amount=1;
	alGetSourcef(source, AL_GAIN, &amount);
	return amount;
}

void AEAudioContextSourceSetPitch(AEAudioContext* self, ALuint source, float amount){
	alSourcef(source, AL_PITCH, amount);
}

float AEAudioContextSourceGetPitch(AEAudioContext* self, ALuint source){
	ALfloat amount=1;
	alGetSourcef(source, AL_PITCH, &amount);
	return amount;
}

void AEAudioContextSourceSetPosition(AEAudioContext* self, ALuint source, float x, float y, float z){
	ALfloat sourcePosition[]={x,y,z};
	alSourcefv(source, AL_POSITION, sourcePosition);
}

void AEAudioContextSourceGetPosition(AEAudioContext* self, ALuint source, AEVec3* v3){
	alGetSourcefv(source, AL_POSITION, (ALfloat*)v3);
}

void AEAudioContextSourceSetVelocity(AEAudioContext* self, ALuint source, float x, float y, float z){
	ALfloat sourceVelocity[]={x,y,z};
	alSourcefv(source, AL_VELOCITY, sourceVelocity);
}

void AEAudioContextSourceGetVelocity(AEAudioContext* self, ALuint source, AEVec3* v3){
	alGetSourcefv(source, AL_VELOCITY, (ALfloat*)v3);
}

void AEAudioContextSourceSetLooping(AEAudioContext* self, ALuint source, bool loop){
	alSourcei(source, AL_LOOPING, loop);
}

bool AEAudioContextSourceGetLooping(AEAudioContext* self, ALuint source){
	ALint looping=false;
	alGetSourcei(source, AL_LOOPING, &looping);
	return looping;
}

