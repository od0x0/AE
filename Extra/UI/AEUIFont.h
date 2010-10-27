#pragma once
#include "AECore.h"
typedef struct AEUIFont AEUIFont;

AEUIFont* AEUIFontLoad(char* filename, int size);

#define AEUIFontAlignLeft -1
#define AEUIFontAlignCenter 0
#define AEUIFontAlignRight 1

//This function takes a lot of parameters, necessarily.
//It's not designed to be friendly, just do a particular job.

typedef struct{
	float x, y, w, h;
	char* text;
	size_t caret;
	AERGBA textColor;
	float caretAlpha;
	char alignment;
}AEUIFontDrawParameters;

void AEUIFontDrawParametersInit(AEUIFontDrawParameters* self);

void AEUIFontDraw(AEUIFont* self, AEUIFontDrawParameters* params);

void AEUIFontDelete(AEUIFont* self);