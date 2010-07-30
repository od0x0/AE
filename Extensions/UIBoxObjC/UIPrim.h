#pragma once
#include "Core.h"

typedef struct{
	float x,y,w,h;
}AEUIRect;

static inline void AEUIRectDraw(AEUIRect rect){
	float x=rect.x;
	float y=rect.y;
	float w=rect.w;
	float h=rect.h;
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

static inline void AEUIRectDrawOutline(AEUIRect rect){
	float x=rect.x;
	float y=rect.y;
	float w=rect.w;
	float h=rect.h;
	glBegin(GL_LINE_LOOP);
		glVertex2f(x,y);
		glVertex2f(x+w,y);
		glVertex2f(x+w,y+h);
		glVertex2f(x,y+h);
	glEnd();
}

static inline void AEUILineDraw(float x,float y,float x2,float y2){
	glBegin(GL_LINE);
		glVertex2f(x,y);
		glVertex2f(x2,y2);
	glEnd();
}

static inline AEUIRect AEUIRectNew(float x,float y,float w,float h){
	AEUIRect rect={x,y,w,h};
	return rect;
}

static inline int AEUIRectsCollide(AEUIRect a,AEUIRect b){
	int leftA = a.x;
	int rightA = a.x + a.w;
	int topA = a.y;
	int bottomA = a.y + a.h;
		
	int leftB = b.x;
	int rightB = b.x + b.w;
	int topB = b.y;
	int bottomB = b.y + b.h;
	
	if( bottomA <= topB ) return 0;
	if( topA >= bottomB ) return 0;
	if( rightA <= leftB ) return 0;
	if( leftA >= rightB ) return 0;
	
	return 1;
}
