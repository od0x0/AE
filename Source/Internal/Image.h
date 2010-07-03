#pragma once
#include "../SOIL.h"
#include "../Core.h"

#include "../HeaderBegin.h"

typedef struct{
	int w,h;
	unsigned char* pixels;
}AEImage;

AEImage* AEImageNew(int w,int h);
AEImage* AEImageFromFile(char* filename);
unsigned char* AEImagePixel(AEImage* image,int x,int y);
void AEImageBlit(AEImage* to,int offsetx,int offsety,AEImage* from);
unsigned int AEImageToTexture(AEImage* image);
void AEImageDelete(AEImage* image);

#include "../HeaderEnd.h"