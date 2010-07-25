#pragma once
#include "Core.h"

typedef struct{
	struct{unsigned int img,w,h;}glyph[128-32];
	int size;
}AEFont;

void AEFontInit(void);
AEFont* AEFontNew(char* fontname,int size);
void AEFontDelete(AEFont* font);