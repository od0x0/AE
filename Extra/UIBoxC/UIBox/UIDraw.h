#include "Core.h"

static void AEUIDrawRect(float x,float y,float w,float h){
	glBegin(GL_QUADS);
		glTexCoord2f(0,0);
		glVertex2f(x,y);
		glTexCoord2f(1,0);
		glVertex2f(x+w,y);
		glTexCoord2f(1,1);
		glVertex2f(x+w,y+h);
		glTexCoord2f(0,1);
		glVertex2f(x,y+h);
	glEnd();
}

static void AEUIDrawRectOutline(float x,float y,float w,float h){
	glBegin(GL_LINE_LOOP);
		glVertex2f(x,y);
		glVertex2f(x+w,y);
		glVertex2f(x+w,y+h);
		glVertex2f(x,y+h);
	glEnd();
}

static void AEUIDrawLine(float x,float y,float x2,float y2){
	glBegin(GL_LINE);
		glVertex2f(x,y);
		glVertex2f(x2,y2);
	glEnd();
}