#pragma once
#include "EVUI.h"

typedef struct{
	AERGBA edgeColor;
	AERGBA fillColor;
	AETexture texture;
	AEFont* font;
	AEFontStyle fontStyle;
	int cornerRadius;
	int textboxHorizontalShift;
	int textboxVerticalShift;
	int edgeWidth;
}EVUIRenderStyle;

static inline void EVUIRenderStyleInit(EVUIRenderStyle* self){
	memset(self, 0, sizeof(EVUIRenderStyle));
	self->fillColor.r=1;
	self->fillColor.g=1;
	self->fillColor.b=1;
	self->fillColor.a=0;
	//self->edgeColor.a=1;
	self->textboxHorizontalShift=2;
	self->textboxVerticalShift=2;
	AEFontStyleInit(& self->fontStyle);
}

void EVUIDrawRect(EVUI* self, EVUIRect* bounds, EVUIRenderStyle* style);
void EVUIDrawLine(EVUI* self, AEVec2* points, size_t pointCount, EVUIRenderStyle* style);
void EVUIDrawBezier(EVUI* self, AEVec2* controlPoints, size_t controlPointCount, size_t subdivisionCount, EVUIRenderStyle* style);
void EVUIDrawText(EVUI* self, EVUIRect* bounds, const char* text, EVUIRenderStyle* style);

bool EVUIDoButton(EVUI* self, EVUIRect* bounds, const char* text, EVUIRenderStyle* style, EVUIRenderStyle* downStyle);
bool EVUIDoTextBox(EVUI* self, EVUIRect* bounds, const char* text, EVUIRenderStyle* style, EVUIRenderStyle* activeStyle, size_t ID);
bool EVUIDoSlider(EVUI* self, EVUIRect* bgBounds, EVUIRect* knobBounds, EVUIRenderStyle* bgStyle, EVUIRenderStyle* knobStyle, AEVec2* value);