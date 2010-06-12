#pragma once
#include "Core.h"

extern SDL_Color AEFontColor;
extern SDL_Color AEFontBackgoundColor;

typedef struct{
	struct{unsigned int img,w,h;}glyph[128-32];
	int size;
}AEFont;

void AEFontInit(void);
AEFont* AEFontNew(char* fontname,int size);
void AEFontRelease(AEFont* font);