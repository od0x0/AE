#pragma once
#include "SOIL.h"
#include "AECore.h"

//It is safe to assume that once "pixels" is allocated, it is never freed until deinit.
typedef struct{
	uint32_t w,h,channels;
	unsigned char* pixels;
}AEImage;

static void AEImageInit(AEImage* self){memset(self, 0, sizeof(AEImage));}
void AEImageInitWithSize(AEImage* self, int w,int h);
///Loads a file using SOIL and returns it in an image
void AEImageInitFromFile(AEImage* self, const char* filename);
void AEImageInitFromMemory(AEImage* self, void* data,size_t dataSize);
void AEImageDeinit(AEImage* self);

///Returns a pointer to an array of color components for a pixel (r is 0, g is 1, b is 2, a is 3)
void AEImageGetPixel(AEImage* self,int x,int y,unsigned char* rgba);
void AEImageSetPixel(AEImage* self,int x,int y,unsigned char* rgba);

///Blits "from" into "to" at "offsetx" and "offsety" using blending, this is very slow right now, as it's only here for the sake of usefulness.  Does not dilate!
void AEImageBlit(AEImage* self,int offsetx,int offsety,AEImage* from);

///Returns the image as an OpenGL texture
AETexture AEImageToTextureWithFlags(AEImage* self, AETextureFlag textureLoadFlags);
