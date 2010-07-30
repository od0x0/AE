#pragma once
#include "UIBox.h"
#include "Text.h"
#include "Font.h"
#include "TextBuffer.h"

typedef struct{
	AEUIBox super;
	AETextBuffer* textbuffer;
	AEFont* font;
	char locked;
}AEUITextbox;

void* AEUITextboxEvent(AEUITextbox* self,int event,void* data);
void AEUITextboxKeyboardSDLEvent(AEUITextbox* self,const SDL_Event* event);