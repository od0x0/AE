#include "AEIUI.h"

void AEIUIInit(AEIUI* self){}
void AEIUIDeinit(AEIUI* self){}

void AEIUIDrawLabel(AEIUI* self, AEIUIRect* rect, char* text, int style){
	self->labelDrawFunc(self, rect, text, style);
}
/*void AEIUIDrawRect(AEIUI* self, AEIUIRect* rect, bool outlineonly){
	glBegin(outlineonly ? GL_QUADS : GL_LINES);
	glTexCoord2f(0, 0);
	glVertex2f(rect->x, rect->y);
	glTexCoord2f(0, 1);
	glVertex2f(rect->x, rect->y+rect->h);
	glTexCoord2f(1, 1);
	glVertex2f(rect->x+rect->w, rect->y+rect->h);
	glTexCoord2f(1, 0);
	glVertex2f(rect->x+rect->w, rect->y);
	glEnd();
}*/
//Returns true if clicked
bool AEIUIDoButton(AEIUI* self, AEIUIRect* rect, char* text, int style){
	bool state=false;
	const AEVec2i mouse={self->mouse.x, self->screenSize.y-self->mouse.y};
	if(rect->x < mouse.x and mouse.x < rect->x + rect->w and rect->y < mouse.y and mouse.y < rect->y+rect->h){
		state=self->mouseButtons & AEIUIMouseButtonLeft;
	}
	self->buttonDrawFunc(self, rect, text, style, state);
	return state;
}