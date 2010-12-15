#pragma once
#include "AECore.h"

//Somewhat based off of pursuade's idea since it pwn3d'd my own

//Always interleaved as 4CUB 2TF* 3NF 3VF
typedef struct{
	GLuint//since this is guarranteed to be 32 bits.
		isAnIndexArray :1,//2 possible values
		storageType :2,//4 possible values
		textureCoordsPerVertex :4,//16 possible
		hasNormals :1,//2 possible values
		hasColors :1,//2 possible
		hasVertices :1,
		unused :32-10;
}AEVAFormat;

//Should be alright to read from, it's pretty much standard now
typedef struct AEVA{
	GLuint length;
	union{
		void* data;
		GLuint vbo;
	}data;
	/*unsigned char isAnIndexArray;
	unsigned char vbotype;
	unsigned char tunit;
	unsigned char dataformat;*/
	AEVAFormat format;
}AEVA;
/*
#define AEVADataFormat3VF 0
#define AEVADataFormat3NF3VF 1
#define AEVADataFormat4CUB3VF 2
#define AEVADataFormat4CUB3NF3VF 3
*/
#define AEVAVBOTypeNone 0
#define AEVAVBOTypeStream 1
#define AEVAVBOTypeDynamic 2
#define AEVAVBOTypeStatic 3

size_t AEVABytesPerVertex(AEVA* self);

void AEVAInit(AEVA* self);
void AEVADeinit(AEVA* self);
void AEVAInitCopy(AEVA* self,AEVA* from);

void* AEVAMap(AEVA* self, unsigned int length,unsigned int writereadmode);
void AEVAUnmap(AEVA* self);

void AEVADraw(AEVA* va, AEVA* ia);
void AEVADrawRange(AEVA* va, AEVA* ia, unsigned long start, unsigned long end);

void AEVASerializeToMBuffer(AEVA* self,AEMBuffer* mbuffer);
void AEVAUnserializeFromMBuffer(AEVA* self,AEMBuffer* mbuffer);

//A little note about serialization...  By the nature of serialization, the data that is serialized is highly unportable.  While it is possible to make serialized index arrays cross platform (which deal with integers), it is another matter for vertex arrays (which deal with 32bit floats), as the C standard makes no real guarrantee about the format of a floating point number.  As such, in order for this to be used properly, you should serialize the vertex array for every possible architecture you plan to run on.

void AEVALoadFromObj(AEVA* va, AEVA* ia, const char* objfilename);

//A few notes about loading from .obj's, 1) It will only pickup triangulated meshes, there are just too many variants to deal with for it to bother with triangulating your stuff, you should be manually triangulating anyway.  2) Leaving the "ia" argument as NULL will generate va as ready for glDrawArrays() 3) This loader will only pickup normals, positions, and texture coordinates, if you want more, write your own loader.