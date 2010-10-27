#include "AEIUI.h"

static void EnterUIDrawMode(void){
	glColor3f(1, 1, 1);
	glPushMatrix();
	glDisable(GL_DEPTH_TEST);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0,AEContextActiveGet()->w,0,AEContextActiveGet()->h,0,1000);
	glMatrixMode(GL_MODELVIEW);
}

static void ExitUIDrawMode(void){
	glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glPopMatrix();
	glColor3f(1, 1, 1);
}

static AEIUIFont* Font;

static void ButtonDrawFunc(AEIUI* self, AEIUIRect* rect, char* text, int style, bool state){
	EnterUIDrawMode();
	float x=rect->x;
	float y=rect->y;
	float w=rect->w;
	float h=rect->h;
	glColor3f(0, 0, 0);
	glLineWidth(2);
	glBegin(GL_LINE_LOOP);
		glVertex2f(x,y);
		glVertex2f(x+w,y);
		glVertex2f(x+w,y+h);
		glVertex2f(x,y+h);
	glEnd();
	glColor4f(1, 1, 1, 0.1);
	glBegin(GL_QUADS);
		//glTexCoord2f(0,0);
		if(not state) glColor4f(0.8, 0.8, 0.8, 0.4);
		glVertex2f(x,y);
		//glTexCoord2f(1,0);
		glVertex2f(x+w,y);
		//glTexCoord2f(1,1);
		if(not state) glColor4f(1, 1, 1, 0.6);
		glVertex2f(x+w,y+h);
		//glTexCoord2f(0,1);
		glVertex2f(x,y+h);
	glEnd();
	AEIUIFontDrawParameters params;
	AEIUIFontDrawParametersInit(&params);
	params.text=text;
	params.alignment=-1;
	params.x=rect->x;
	params.y=rect->y;
	params.w=0;
	params.h=0;
	params.textColor.r=1;
	params.textColor.g=1;
	params.textColor.b=1;
	params.textColor.a=1;
	AEIUIFontDraw(Font, &params);
	ExitUIDrawMode();
}

static void LabelDrawFunc(AEIUI* self, AEIUIRect* rect, char* text, int style){
	EnterUIDrawMode();
	AEIUIFontDrawParameters params;
	AEIUIFontDrawParametersInit(&params);
	params.text=text;
	params.alignment=-1;
	params.x=rect->x;
	params.y=rect->y;
	params.w=rect->w;
	params.h=rect->h;
	params.textColor.r=1;
	params.textColor.g=1;
	params.textColor.b=1;
	params.textColor.a=1;
	AEIUIFontDraw(Font, &params);
	ExitUIDrawMode();
}

void AEIUIInit(AEIUI* self){
	memset(self, 0, sizeof(AEIUI));
	self->labelDrawFunc=LabelDrawFunc;
	self->buttonDrawFunc=ButtonDrawFunc;
	Font=AEIUIFontLoad("Data/ARIAL.TTF", 14);
}

void AEIUIDeinit(AEIUI* self){
	AEIUIFontDelete(Font);
}

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
		abort();
		state=self->mouseButtons & AEIUIMouseButtonLeft;
	}
	self->buttonDrawFunc(self, rect, text, style, state);
	return state;
}