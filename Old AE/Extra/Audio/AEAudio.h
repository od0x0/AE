#pragma once
#include "AE.h"

//Add known systems
#ifdef __APPLE_CC__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#else
//Most portable option, just add a header search path
#include "al.h"
#include "alc.h"
#include "alu.h"
#endif

#define STB_VORBIS_HEADER_ONLY
#include "stb_vorbis.c"

typedef struct{
	ALuint ID;
	
	stb_vorbis* stream;
	stb_vorbis_info info;
	
	ALuint buffers[2];
	ALuint source;
	ALenum format;
	
	size_t bufferSize;
	
	size_t totalSamplesLeft;
	
	bool shouldLoop;
}AEAudioStream;

//You will probably never need to call any of these manually.
void AEAudioStreamInit(AEAudioStream* self);
void AEAudioStreamDeinit(AEAudioStream* self);
bool AEAudioStreamOpen(AEAudioStream* self, const char* filename);
bool AEAudioStreamUpdate(AEAudioStream* self);

typedef struct{
	ALCdevice* alcdevice;
	ALCcontext* alccontext;
	
	AEVec3 position;
	AEVec3 rotation;
	AEVec3 velocity;
	
	AEArray(AEAudioStream) streams;
	AEArray(ALuint) sources;
}AEAudioContext;

void AEAudioContextInit(AEAudioContext* self);
void AEAudioContextDeinit(AEAudioContext* self);

void AEAudioContextSetPosition(AEAudioContext* self, float x, float y, float z);
void AEAudioContextGetPosition(AEAudioContext* self, AEVec3* v3);
void AEAudioContextSetRotation(AEAudioContext* self, float x, float y, float z);
void AEAudioContextGetRotation(AEAudioContext* self, AEVec3* v3);
void AEAudioContextSetVelocity(AEAudioContext* self, float x, float y, float z);
void AEAudioContextGetVelocity(AEAudioContext* self, AEVec3* v3);

ALuint AEAudioContextBufferLoad(AEAudioContext* self, const char* filename);
void AEAudioContextBufferDelete(AEAudioContext* self, ALuint buffer);

ALuint AEAudioContextStreamLoad(AEAudioContext* self, const char* filename);
AEAudioStream* AEAudioContextGetStreamForID(AEAudioContext* self, ALuint streamID);
void AEAudioContextStreamDelete(AEAudioContext* self, ALuint streamID);

ALuint AEAudioContextSourceNew(AEAudioContext* self, ALuint buffer);
void AEAudioContextSourceDelete(AEAudioContext* self, ALuint source);

void AEAudioContextUpdateStreams(AEAudioContext* self);

void AEAudioContextSourceSetPaused(AEAudioContext* self, ALuint source, bool to);
bool AEAudioContextSourceGetPaused(AEAudioContext* self, ALuint source);
void AEAudioContextSourceSetVolume(AEAudioContext* self, ALuint source, float amount);
float AEAudioContextSourceGetVolume(AEAudioContext* self, ALuint source);
void AEAudioContextSourceSetPitch(AEAudioContext* self, ALuint source, float amount);
float AEAudioContextSourceGetPitch(AEAudioContext* self, ALuint source);
void AEAudioContextSourceSetPosition(AEAudioContext* self, ALuint source, float x, float y, float z);
void AEAudioContextSourceGetPosition(AEAudioContext* self, ALuint source, AEVec3* v3);
void AEAudioContextSourceSetVelocity(AEAudioContext* self, ALuint source, float x, float y, float z);
void AEAudioContextSourceGetVelocity(AEAudioContext* self, ALuint source, AEVec3* v3);
void AEAudioContextSourceSetLooping(AEAudioContext* self, ALuint source, bool loop);
bool AEAudioContextSourceGetLooping(AEAudioContext* self, ALuint source);
