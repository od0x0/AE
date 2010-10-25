#pragma once
#include "AE.h"
typedef struct AEIUIFont AEIUIFont;

AEIUIFont* AEIUIFontLoad(char* filename, int size);

#define AEIUIFontAlignLeft -1
#define AEIUIFontAlignCenter 0
#define AEIUIFontAlignRight 1

void AEIUIFontDraw(AEIUIFont* self, char* text, int alignment, float x, float y, float w, float h, size_t caret);
void AEIUIFontDelete(AEIUIFont* self);