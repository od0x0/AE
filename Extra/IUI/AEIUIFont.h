#pragma once
#include "AECore.h"
typedef struct AEIUIFont AEIUIFont;

AEIUIFont* AEIUIFontLoad(char* filename, int size);

#define AEIUIFontAlignLeft -1
#define AEIUIFontAlignCenter 0
#define AEIUIFontAlignRight 1

//This function takes a lot of parameters, necessarily.
//It's not designed to be friendly, just do a particular job.

typedef struct{
	float x, y, w, h;
	char* text;
	size_t caret;
	AERGBA textColor;
	float caretAlpha;
	char alignment;
}AEIUIFontDrawParameters;

void AEIUIFontDrawParametersInit(AEIUIFontDrawParameters* self);

void AEIUIFontDraw(AEIUIFont* self, AEIUIFontDrawParameters* params);

void AEIUIFontDelete(AEIUIFont* self);