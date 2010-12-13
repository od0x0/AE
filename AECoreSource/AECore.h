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
typedef void (*AEContextCallbackInitFunc)(AEContext* self, const char* title, void* arg);
typedef void (*AEContextCallbackRefreshFunc)(AEContext* self, void* arg);
typedef int (*AEContextCallbackPollInputFunc)(AEContext* self, void* arg);
typedef void (*AEContextCallbackSwapBuffersFunc)(AEContext* self, void* arg);
typedef void (*AEContextCallbackDeinitFunc)(AEContext* self, void* arg);
typedef double (*AEContextCallbackSecondsGetFunc)(AEContext* self, void* arg);
typedef void (*AEContextCallbackFixedUpdateFunc)(AEContext* self, double secondsSinceLastCall, void* arg);
typedef void (*AEContextCallbackFrameUpdateFunc)(AEContext* self, double secondsSinceLastCall, void* arg);

struct AEContext{
	unsigned int w,h;
	unsigned char r, g, b, a, stencil, depth, inFullscreen, multisample;
	void* aux;
	GLbitfield clearedBuffers;
	
	AEContextCallbackInitFunc init;
	void* initArg;
	AEContextCallbackRefreshFunc refresh;
	void* refreshArg;
	AEContextCallbackPollInputFunc pollInput;
	void* pollInputArg;
	AEContextCallbackSwapBuffersFunc swapBuffers;
	void* swapBuffersArg;
	AEContextCallbackDeinitFunc deinit;
	void* deinitArg;
	AEContextCallbackSecondsGetFunc secondsGet;
	void* secondsGetArg;
	
	AEContextCallbackFixedUpdateFunc fixedUpdate;
	void* fixedUpdateArg;
	double fixedUpdateFrameRateMax;
	double fixedUpdateFrameRateMin;
	
	AEContextCallbackFrameUpdateFunc frameUpdate;
	void* frameUpdateArg;
};

void AEContextActiveSet(AEContext* context);
AEContext* AEContextActiveGet(void);

void AEContextInit(AEContext* context,const char* title,int w,int h);
void AEContextRun(AEContext* context);
void AEContextDeinit(AEContext* context);

///////////////////////////////////////////
//////////Utility stuff
//////////////////////////////////////////
typedef GLuint AETexture;
#define AETextureLoadFlagDXTCompression SOIL_FLAG_COMPRESS_TO_DXT
#define AETextureLoadFlagMipmaps SOIL_FLAG_MIPMAPS
#define AETextureLoadFlagInvertY SOIL_FLAG_INVERT_Y
#define AETextureLoadFlagRepeat SOIL_FLAG_TEXTURE_REPEATS

#define AETextureLoadFlagDefault (SOIL_FLAG_COMPRESS_TO_DXT|SOIL_FLAG_MIPMAPS|SOIL_FLAG_TEXTURE_REPEATS)

AETexture AETextureLoadWithFlags(const char* filename,unsigned int flags);
#define AETextureLoad(filename) AETextureLoadWithFlags(filename,AETextureLoadFlagDefault)
AETexture AETextureLoadFromMemoryWithFlags(void* data,size_t dataSize, unsigned int flags);
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
	FILE* file;//If true, we use this instead.
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
void AEMBufferPositionSeek(AEMBuffer* self, long offset, int from);
void AEMBufferPositionSet(AEMBuffer* self, size_t position);
size_t AEMBufferPositionGet(AEMBuffer* self);
size_t AEMBufferLengthGet(AEMBuffer* self);
