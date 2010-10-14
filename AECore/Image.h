#pragma once
#include "SOIL.h"
#include "Core.h"


#define AEImageSerializeToFILETemporaryDirectory ""

///Do not directly access this
typedef struct{
	int w,h,channels;
	short refcount;
	unsigned char* pixels;
	unsigned int flags;
	char* metastring;
}AEImage;

///Allocates an image with w*h pixels
AEImage* AEImageNew(int w,int h);

//Retain/Cloning an image
AEImage* AEImageRetain(AEImage* image);
//AEImage* AEImageClone(AEImage* image);

//Serialization, internal format
void AEImageSerializeToFILE(AEImage* image,FILE* file);
void AEImageUnserializeFromFILE(AEImage* image,FILE* file);

///Loads a file using SOIL and returns it in an image
AEImage* AEImageLoad(char* filename);

AEImage* AEImageLoadFromMemory(void* data,size_t dataSize);

//Control textureflags and metastring
char* AEImageMetaStringGet(AEImage* image);
void AEImageMetaStringSet(AEImage* image,char* string);
unsigned int AEImageTextureFlagsGet(AEImage* image);
void AEImageTextureFlagsSet(AEImage* image,unsigned int flags);

///Returns a pointer to a an array of color components for a pixel (r is 0, g is 1, b is 2, a is 3)
unsigned char* AEImagePixelGet(AEImage* image,int x,int y);
void AEImagePixelSet(AEImage* image,int x,int y,unsigned char* rgba);

int AEImageChannelsPerPixelGet(AEImage* image);

///Blits "from" into "to" at "offsetx" and "offsety" using blending, this is very slow right now (6 fps on my G4 for 4 128x128 blits), does not scale!
void AEImageBlit(AEImage* to,int offsetx,int offsety,AEImage* from);

///Returns the image as an OpenGL texture with mipmaps
AETexture AEImageToTextureWithFlags(AEImage* image,unsigned int flags);
#define AEImageToTexture(image) AEImageToTextureWithFlags(image,AETextureLoadFlagDXTCompression|AETextureLoadFlagMipmaps)

///The only way in which an image will be destroyed
void AEImageDelete(AEImage* image);