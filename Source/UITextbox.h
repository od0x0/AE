#pragma once
#include "UIBox.h"
#include "Text.h"
#include "Font.h"

typedef struct{
	AEUIBox_inherit
	char* text;
	AEFont* font;
	unsigned int max;
	unsigned int caret;
	char locked;
}AEUITextbox;

void* AEUITextboxEvent(AEUITextbox* self,int event,void* data);
void AEUITextboxKeyboardSDLEvent(AEUITextbox* self,const SDL_Event* event);