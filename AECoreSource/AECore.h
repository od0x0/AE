/*
Writen by Oliver Daids, 2009.

Ambition Engine Core.
*/

#pragma once

//Add known systems to make it easier.
#ifdef __APPLE_CC__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
//Most portable option, just add a header search path
#include "gl.h"
#include "glu.h"
#endif

#include <stdlib.h>
#include <stdio.h>
//If your compiler doesn't have this, then that sucks for you. *cough* MSVC *cough*
#include <stdint.h>
#ifndef __cplusplus
#include <stdbool.h>
#include <iso646.h>
#endif

//Like, stdbool & iso646 zomgftwbbq

#include <math.h>
#include <string.h>
#include "AEMath.h"
#include "SOIL.h"

char* AEStringDuplicate(const char* string);

typedef struct AEContext AEContext;
typedef void (*AEContextCallbackOpenFunc)(AEContext* self, const char* title);
typedef void (*AEContextCallbackRefreshFunc)(AEContext* self);
typedef int (*AEContextCallbackPollInputFunc)(AEContext* self);
typedef void (*AEContextCallbackSwapBuffersFunc)(AEContext* self);
typedef void (*AEContextCallbackCloseFunc)(AEContext* self);
typedef double (*AEContextCallbackSecondsFunc)(AEContext* self);
typedef void (*AEContextCallbackFixedUpdateFunc)(AEContext* self, double secondsSinceLastCall);
typedef void (*AEContextCallbackFrameUpdateFunc)(AEContext* self, double secondsSinceLastCall);

struct AEContext{
	unsigned int w,h;
	unsigned char r, g, b, a, stencil, depth, inFullscreen, multisample;
	void* userdata;
	GLbitfield clearedBuffers;
	
	AEContextCallbackOpenFunc open;
	AEContextCallbackRefreshFunc refresh;
	AEContextCallbackPollInputFunc pollInput;
	AEContextCallbackSwapBuffersFunc swapBuffers;
	AEContextCallbackCloseFunc close;
	AEContextCallbackSecondsFunc seconds;
	AEContextCallbackFixedUpdateFunc fixedUpdate;
	double fixedUpdateFrameRateMax;
	double fixedUpdateFrameRateMin;
	
	AEContextCallbackFrameUpdateFunc frameUpdate;
};

void AEContextsSetActive(AEContext* context);
AEContext* AEContextsGetActive(void);

void AEContextOpen(AEContext* context,const char* title,int w,int h);
void AEContextRun(AEContext* context);
void AEContextClose(AEContext* context);

///////////////////////////////////////////
//////////Utility stuff
//////////////////////////////////////////
typedef GLuint AETexture;
typedef GLuint AETextureFlag;

#define AETextureFlagForcePowerOfTwo SOIL_FLAG_POWER_OF_TWO
#define AETextureFlagMipMap SOIL_FLAG_MIPMAPS
#define AETextureFlagRepeat SOIL_FLAG_TEXTURE_REPEATS
#define AETextureFlagPremultiplyAlpha SOIL_FLAG_MULTIPLY_ALPHA
#define AETextureFlagCompressDXT SOIL_FLAG_COMPRESS_TO_DXT
#define AETextureFlagLoadDXT SOIL_FLAG_DDS_LOAD_DIRECT
#define AETextureFlagNTSC SOIL_FLAG_NTSC_SAFE_RGB
#define AETextureFlagDefault (AETextureFlagCompressDXT|AETextureFlagMipMap|AETextureFlagRepeat)

AETexture AETextureLoadWithFlags(const char* filename, AETextureFlag flags);
#define AETextureLoad(filename) AETextureLoadWithFlags(filename,AETextureFlagDefault)
AETexture AETextureLoadFromMemoryWithFlags(void* data,size_t dataSize, AETextureFlag flags);
static inline void AETextureBind(AETexture texture){glBindTexture(GL_TEXTURE_2D, texture);}
void AETextureDelete(AETexture texture);

//We need a better method of handling errors such as missing files
/*
#define AEExceptionTypeGeneric NULL
#define AEExceptionTypeFile "File Exception"

typedef struct AEException AEException;

AEException* AEExceptionsRootGet(void);
AEException* AEExceptionsPush(void);
void AEExceptionsPop(void);
void AEExceptionsThrow_internal(const char* type, char* message, const char* functionOfOrigin);
#define AEExceptionThrow(type, message) AEExceptionsThrow_internal(type, message, __func__)
AEException* AEExceptionsCatch(const char* type);
*/
#define AEError(msg) {printf("Error in %s(): %s\n",__func__,(msg)); abort();}
//AEExceptionThrow(NULL, msg)

uint64_t AEHostU64FromNet(uint64_t netu64);
uint64_t AENetU64FromHost(uint64_t hostu64);

uint32_t AEHostU32FromNet(uint32_t netu32);
uint32_t AENetU32FromHost(uint32_t hostu32);

typedef struct {
	FILE* file;//If this is non-null, we use this instead.
	size_t length;
	size_t allocated;
	size_t position;
	void* bytes;
}AEMBuffer;

void AEMBufferInit(AEMBuffer* self);
void AEMBufferDeinit(AEMBuffer* self);
//void* AEMBufferBytesGet(AEMBuffer* self, size_t size);
void AEMBufferRead(AEMBuffer* self, void* data, size_t size);
void AEMBufferWrite(AEMBuffer* self, void* data, size_t size);
void AEMBufferSeek(AEMBuffer* self, long offset, int from);
void AEMBufferSetPosition(AEMBuffer* self, size_t position);
size_t AEMBufferGetPosition(AEMBuffer* self);
size_t AEMBufferGetLength(AEMBuffer* self);
