#pragma once
#include "AE.h"
#include "List.h"
#include "TextBuffer.h"

typedef struct{
	int x,y,w,h;
}AEIUIRect;

typedef struct AEIUI AEIUI;
struct AEIUI{
	bool disableRendering;
	AEVec2i mouse;
	AEVec2i screenSize;
	int mouseButtons;
	
	void (*buttonDrawFunc)(AEIUI* self, AEIUIRect* rect, char* text, int style, bool state);
	void (*labelDrawFunc)(AEIUI* self, AEIUIRect* rect, char* text, int style);
};

#define AEIUIMouseButtonLeft (1<<0)
#define AEIUIMouseButtonMiddle (1<<1)
#define AEIUIMouseButtonRight (1<<2)
#define AEIUIMouseButtonUpWheel (1<<3)
#define AEIUIMouseButtonDownWheel (1<<4)

void AEIUIInit(AEIUI* self);
void AEIUIDeinit(AEIUI* self);

void AEIUIDrawLabel(AEIUI* self, AEIUIRect* rect, char* text, int style);
//Returns true if clicked
bool AEIUIDoButton(AEIUI* self, AEIUIRect* rect, char* text, int style);