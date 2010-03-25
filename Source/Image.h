#pragma once
#include "SOIL.h"
#include "Core.h"

typedef struct{
	int w,h;
	unsigned char* pixels;
	SDL_Surface* surface;
}AEImage;

AEImage* AEImageNew(int w,int h);
AEImage* AEImageFromFile(char* filename);
unsigned char* AEImagePixel(AEImage* image,int x,int y);
void AEImageBlit(AEImage* to,int offsetx,int offsety,AEImage* from);
void AEImageDelete(AEImage* image);