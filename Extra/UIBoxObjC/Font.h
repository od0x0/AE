#pragma once
#include "Core.h"

typedef struct{
	struct{unsigned int w,h;unsigned int offsetx,img;}glyph[128-32];
	unsigned int img;
	int size;
}AEFont;

void AEFontInit(void);
AEFont* AEFontNew(char* fontname,int size,float r,float g,float b,float a);
void AEFontRenderChar(AEFont* font,int c,int x,int y,int w,int h);
void AEFontDelete(AEFont* font);
int AEFontSizeGet(AEFont* font);