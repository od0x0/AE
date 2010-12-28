#pragma once
#include "AE.h"
#include "AEFont.h"
#include "AETextBuffer.h"
#include "EUMemory.h"

typedef struct{
	float x, y, w, h;
}EVUIRect;

static inline EVUIRect EVUIRectNew(float x, float y, float w, float h){
	EVUIRect rect={.x=x,.y=y,.w=w,.h=h};
	return rect;
}

static inline bool EVUIRectIntersects(EVUIRect self, EVUIRect other){
	int leftA = self.x;
	int rightA = self.x + self.w;
	int topA = self.y;
	int bottomA = self.y + self.h;
		
	int leftB = other.x;
	int rightB = other.x + other.w;
	int topB = other.y;
	int bottomB = other.y + other.h;
	
	if( bottomA <= topB ) return 0;
	if( topA >= bottomB ) return 0;
	if( rightA <= leftB ) return 0;
	if( leftA >= rightB ) return 0;
	
	return 1;
}

typedef enum {
	EVUIMouseButtonRight,
	EVUIMouseButtonMiddle,
	EVUIMouseButtonLeft,
	EVUIMouseButtonUpWheel,
	EVUIMouseButtonDownWheel,
	EVUIMouseButtonLast=EVUIMouseButtonDownWheel
}EVUIMouseButton;

typedef enum{
	EVUIMouseButtonStateUp,
	EVUIMouseButtonStateJustPressed,
	EVUIMouseButtonStateDown
}EVUIMouseButtonState;

typedef struct EVUI EVUI;

EVUI* EVUINew(void);
void EVUIDelete(EVUI* self);

void EVUIClearTextbox(EVUI* self);
const char* EVUITextboxTextGet(EVUI* self);
void EVUIPushCharacter(EVUI* self, uint32_t character);
void EVUIPushBackspace(EVUI* self);
void EVUICaretSet(EVUI* self, size_t to);
void EVUIMoveCaret(EVUI* self, int by);

void EVUIMouseButtonSet(EVUI* self, EVUIMouseButton button, bool isDown);
bool EVUIMouseButtonIsDown(EVUI* self, EVUIMouseButton button);
bool EVUIMouseButtonWasJustPressed(EVUI* self, EVUIMouseButton button);

void EVUIMouseSet(EVUI* self, float x, float y);
void EVUIMouseGet(EVUI* self, AEVec2* v2);
void EVUIWindowSizeSet(EVUI* self, float w, float h);
void EVUIWIndowSizeGet(EVUI* self, AEVec2* v2);

void EVUIBeginRender(EVUI* self);
void EVUIEndRender(EVUI* self);


///Do not touch!
struct EVUI{
//Read only
	AEVec2 windowSize;
	AEVec2 mouse;
	bool mouseClipping;
	EVUIRect mouseClipBounds;
	EVUIMouseButtonState mouseButtons[EVUIMouseButtonLast+1];
	struct{
		AETextBuffer* textBuffer;
		size_t caret;
		size_t activeID;
		bool wasEditted;
	}textbox;
};