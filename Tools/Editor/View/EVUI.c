#include "EVUI.h"

const char* EVUITextboxTextGet(EVUI* self){
	return AETextBufferTextGet(self->textbox.textBuffer);
}

void EVUIClearTextbox(EVUI* self){
	//This may often turn out faster.
	AETextBufferDelete(self->textbox.textBuffer);
	self->textbox.textBuffer=AETextBufferNew();
}

void EVUIPushCharacter(EVUI* self, uint32_t character){
	if(not self->textbox.activeID) return;
	if(character > 127) return;
	//printf("%i\n",(int)character);
	if((character != '\n' and character != '\r') and character < 32) return;
	if(character == '\r') character='\n';
	//printf("%i\n",(int)character);
	if (character == '\n' or character == '\r') {
		self->textbox.wasEditted=true;
		return;
	}
	char s[2]={character, 0};
	AETextBufferInsert(self->textbox.textBuffer, self->textbox.caret, s);
	EVUIMoveCaret(self, 1);
	/*if(character){
		textbox.wasEditted=YES;
	}*/
}

void EVUIPushBackspace(EVUI* self){
	if(not self->textbox.activeID) return;
	AETextBufferBackspace(self->textbox.textBuffer, self->textbox.caret-1, 1);
	EVUIMoveCaret(self, -1);
}

void EVUICaretSet(EVUI* self, size_t to){
	if(not self->textbox.activeID) return;
	size_t length=AETextBufferLengthGet(self->textbox.textBuffer);
	if(to > length) to=length;
	//else if(to < 0) to=0;
	self->textbox.caret=to;
}

void EVUIMoveCaret(EVUI* self, int by){
	if(not self->textbox.activeID) return;
	long to=self->textbox.caret+by;
	if(to<0) to=0;
	EVUICaretSet(self, to);
}

void EVUIMouseButtonSet(EVUI* self, EVUIMouseButton button, bool isDown){
	if(button > EVUIMouseButtonLast) return;
	if(isDown) self->mouseButtons[button] = (self->mouseButtons[button] != EVUIMouseButtonStateUp) ? EVUIMouseButtonStateDown : EVUIMouseButtonStateJustPressed;
	else self->mouseButtons[button]=EVUIMouseButtonStateUp;
}

bool EVUIMouseButtonIsDown(EVUI* self, EVUIMouseButton button){
	if(button > EVUIMouseButtonLast) return false;
	return self->mouseButtons[button]!=EVUIMouseButtonStateUp;
}

bool EVUIMouseButtonWasJustPressed(EVUI* self, EVUIMouseButton button){
	if(button > EVUIMouseButtonLast) return false;
	return self->mouseButtons[button]==EVUIMouseButtonStateJustPressed;
}

void EVUIMouseSet(EVUI* self, float x, float y){
	self->mouse.x=x;
	self->mouse.y=y;
}

void EVUIWindowSizeSet(EVUI* self, float w, float h){
	self->windowSize.x=w;
	self->windowSize.y=h;
}

void EVUIMouseGet(EVUI* self, AEVec2* v2){
	v2->x=self->mouse.x;
	v2->y=self->mouse.y;
}

void EVUIWIndowSizeGet(EVUI* self, AEVec2* v2){
	v2->x=self->windowSize.x;
	v2->y=self->windowSize.y;
}

EVUI* EVUINew(void){
	EVUI* self=EUMemoryAllocate(sizeof(EVUI));
	self->textbox.textBuffer=AETextBufferNew();
	return self;
}

void EVUIDelete(EVUI* self){
	if(not self) return;
	AETextBufferDelete(self->textbox.textBuffer);
	EUMemoryFree(self);
}

void EVUIBeginRender(EVUI* self){
	glColor3f(1, 1, 1);
	glPushMatrix();
	glDisable(GL_DEPTH_TEST);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0,AEContextActiveGet()->w,0,AEContextActiveGet()->h,0,1000);
	glMatrixMode(GL_MODELVIEW);
	AETextureBind(0);
}

void EVUIEndRender(EVUI* self){
	AETextureBind(0);
	glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glPopMatrix();
	glColor3f(1, 1, 1);
}