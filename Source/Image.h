#pragma once
#include "SOIL.h"
#include "Core.h"

#include "HeaderBegin.h"

///Do not directly access this
typedef struct{
	int w,h;
	unsigned char* pixels;
}AEImage;

///Allocates an image with w*h pixels
AEImage* AEImageNew(int w,int h);

///Loads a file using SOIL and returns it in an image
AEImage* AEImageFromFile(char* filename);

///Returns a pointer to a an array of color components for a pixel (r is 0, g is 1, b is 2, a is 3)
unsigned char* AEImagePixel(AEImage* image,int x,int y);

///Blits "from" into "to" at "offsetx" and "offsety" using blending, this is very slow right now (6 fps on my G4 for 4 128x128 blits)
void AEImageBlit(AEImage* to,int offsetx,int offsety,AEImage* from);

///Returns the image as an OpenGL texture with mipmaps
unsigned int AEImageToTexture(AEImage* image);

///The only way in which an image will be destroyed
void AEImageDelete(AEImage* image);

#include "HeaderEnd.h"