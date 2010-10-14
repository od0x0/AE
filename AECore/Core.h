/*
Writen by Oliver Daids, 2009.

Ambition Engine Core.
*/

#pragma once
#include "AE.h"
#include "SOIL.h"

typedef struct AEContext{
	unsigned int w,h,r,g,b,a,stencil,depth,inFullscreen;
	void* aux;
	
	void (*init)(struct AEContext* self,char* title);
	void (*refresh)(struct AEContext* self);
	int (*pollinput)(struct AEContext* self);
	void (*swapbuffers)(struct AEContext* self);
	void (*deinit)(struct AEContext* self);
	double (*seconds)(struct AEContext* self);
}AEContext;

void AEContextActiveSet(AEContext* context);
AEContext* AEContextActiveGet(void);

void AEInit(AEContext* context,char* title,int w,int h);
void AEStart(AEContext* context,void (*func)(float));
void AEQuit(AEContext* context);

typedef GLuint AETexture;
#define AETextureLoadFlagDXTCompression SOIL_FLAG_COMPRESS_TO_DXT
#define AETextureLoadFlagMipmaps SOIL_FLAG_MIPMAPS
#define AETextureLoadFlagInvertY SOIL_FLAG_INVERT_Y

AETexture AETextureLoadWithFlags(const char* filename,unsigned int flags);
#define AETextureLoad(filename) AETextureLoadWithFlags(filename,SOIL_FLAG_COMPRESS_TO_DXT|SOIL_FLAG_MIPMAPS)
AETexture AETextureLoadFromMemoryWithFlags(void* data,size_t dataSize, unsigned int flags);
static inline void AETextureBind(AETexture texture){glBindTexture(GL_TEXTURE_2D, texture);}
void AETextureDelete(AETexture texture);

void AEError_internal(char* message,const char* function);
#define AEError(msg) AEError_internal(msg,__func__)

uint64_t AEUInt64From8Bytes(uint8_t* bytes,bool bigendian);
void AEUInt64To8Bytes(uint64_t integer,uint8_t* bytes,bool bigendian);

uint32_t AEUInt32From4Bytes(uint8_t* bytes,bool bigendian);
void AEUInt32To4Bytes(uint32_t integer,uint8_t* bytes,bool bigendian);

/*typedef struct AEFile AEFile;
struct AEFile{
	void* data;
	void (*read)(AEFile*, void*, size_t);
	void (*write)(AEFile*, void*, size_t);
	void (*close)(AEFile*);
	void (*positionSet)(AEFile*, uint64_t);
	uint64_t (*positionGet)(AEFile*);
	uint64_t (*lengthGet)(AEFile*);
	bool (*eof)(AEFile*);
};

void AEFileInit(AEFile* self);
void AEFileDeinit(AEFile* self);
void AEFileRead(AEFile* self, void* to, size_t size);
void AEFileWrite(AEFile* self, void* from, size_t size);
void AEFilePositionSet(AEFile* self, uint64_t position);
uint64_t AEFilePositionGet(AEFile* self);
uint64_t AEFileLengthGet(AEFile* self);
bool AEFileIsAtEnd(AEFile* self);*/

typedef struct {
	size_t length;
	size_t allocated;
	size_t position;
	void* bytes;
}AEMBuffer;

void* AEMBufferBytesGet(AEMBuffer* self, size_t size);
void AEMBufferRead(AEMBuffer* self, void* data, size_t size);
void AEMBufferWrite(AEMBuffer* self, void* data, size_t size);
void AEMBufferPositionSet(AEMBuffer* self, size_t position);
size_t AEMBufferPositionGet(AEMBuffer* self);
size_t AEMBufferLengthGet(AEMBuffer* self);