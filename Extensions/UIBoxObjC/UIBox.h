#pragma once
#include "Image.h"
#include "List.h"
#include "UIObject.h"
#include "Text.h"
#include "Font.h"

typedef struct{
	float x,y,w,h;
}AEUIBoxRect;

#include "Core.h"

static void AEUIBoxRectDraw(AEUIBoxRect rect){
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

static void AEUIDrawRectOutline(AEUIBoxRect rect){
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

static void AEUIDrawLine(float x,float y,float x2,float y2){
	glBegin(GL_LINE);
		glVertex2f(x,y);
		glVertex2f(x2,y2);
	glEnd();
}

static inline AEUIBoxRect AEUIBoxRectNew(float x,float y,float w,float h){
	AEUIBoxRect rect={x,y,w,h};
	return rect;
}

static inline int AEUIBoxRectsCollide(AEUIBoxRect a,AEUIBoxRect b){
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


@interface AEUIBox : AEUIObject{
@public
	AEUIBox* parent;
	AEUIBoxRect rectangle;
	AEList* children;
	AERGBA color;
	AEFont* font;
}

-init;

-setRect:(AEUIBoxRect)rect;
-(AEUIBoxRect)rect;

-addChild:(AEUIBox*)child;
-removeChild:(AEUIBox*)child;
-(AEList*)childList;
-raiseChild:(AEUIBox*)child;
-raise;

-focus;
-(BOOL)hasFocus;
-render;

-cursorDown:(int)button;
-cursorUp:(int)button;
-cursorMoveX:(int)x Y:(int)y;

-keyDown:(int)key;
-keyUp:(int)key;

-moveX:(int)x Y:(int)y;
-click;

-(void)dealloc;

@end

extern AEUIBox* AEUIBoxActive;