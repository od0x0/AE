#pragma once
#include "AE.h"
#include "stb_truetype.h"

#define AEFontAlignLeft -1
#define AEFontAlignCenter 0
#define AEFontAlignRight 1

//There are a lot of parameters that go into the rendering of a line of text, so we use a pointer to a structure to pass them.  Ugly, I know.
typedef struct{
	AERGBA textColor;
	float caretAlpha;
	int alignment;
}AEFontStyle;

static inline void AEFontStyleInit(AEFontStyle* self){
	memset(self, 0, sizeof(AEFontStyle));
	self->alignment=-1;
	self->textColor.a=1;
}

typedef struct AEFont AEFont;

float AEFontGetLineHeight(AEFont* self);
float AEFontMeasureTextLength(AEFont* self, const char* text, size_t stop);

void AEFontRenderText(AEFont* self, AEFontStyle* style, float x, float y, float w, float h, const char* text, size_t caret);

AEFont* AEFontLoadFromTTF(const char* filename, float size);
void AEFontDelete(AEFont* self);
