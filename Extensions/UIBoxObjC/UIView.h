#pragma once
#include "UIObject.h"
#include "UIArray.h"
#include "Image.h"
#include "Text.h"
#include "Font.h"
#include "UIPrim.h"

#define AEUIViewEventTypeMouseButton 1
#define AEUIViewEventTypeKey 2
#define AEUIViewEventTypeMouseMove 3
#define AEUIViewEventTypeMove 4

typedef struct{
	int type;
	union{
		struct{
			int unicode;
			int key;
			char wasPressed;//If not pressed, then it was released;
		}key;
		struct{
			int button;
			char wasPressed;//If not pressed, then it was released;
		}mousebutton;
		struct{
			int x,y;
		}mousemove;
		struct{
			int x,y;
		}move;
	}data;
}AEUIViewEvent;

static inline AEUIViewEvent AEUIViewEventFromSDLEvent(SDL_Event* sdlevent){
	AEUIViewEvent event;
	switch (sdlevent->type) {
		case SDL_KEYDOWN:
			event.type=AEUIViewEventTypeKey;
			event.data.key.unicode=sdlevent->key.keysym.unicode;
			event.data.key.key=sdlevent->key.keysym.sym;
			event.data.key.wasPressed=1;
			break;
		case SDL_KEYUP:
			event.type=AEUIViewEventTypeKey;
			event.data.key.unicode=sdlevent->key.keysym.unicode;
			event.data.key.key=sdlevent->key.keysym.sym;
			event.data.key.wasPressed=0;
			break;
		case SDL_MOUSEBUTTONDOWN:
			event.type=AEUIViewEventTypeMouseButton;
			event.data.mousebutton.button=sdlevent->button.button;
			event.data.mousebutton.wasPressed=1;
			break;
		case SDL_MOUSEBUTTONUP:
			event.type=AEUIViewEventTypeMouseButton;
			event.data.mousebutton.button=sdlevent->button.button;
			event.data.mousebutton.wasPressed=0;
			break;
		case SDL_MOUSEMOTION:
			event.type=AEUIViewEventTypeMouseMove;
			event.data.mousemove.x=sdlevent->motion.xrel;
			event.data.mousemove.y=-sdlevent->motion.yrel;
			break;
	}
	return event;
}

@interface AEUIView : AEUIObject {
	AEUIRect bounds;
	AEUIView* parent;
	AEUIArray* subviews;
	AEFont* font;
	unsigned char color[4];
	unsigned char borderColor[4];
	unsigned int backgroundTexture;
}
-init;
-(void)setBounds:(AEUIRect)newbounds;
-(AEUIRect)bounds;
-setParent:(id)newparent;
-parent;
-(void)setFont:(AEFont*)newfont;
-(AEFont*)font;
-(void)setColorR:(float)r G:(float)g B:(float)b A:(float)a;
-(AERGBA)color;
-(void)setBorderColorR:(float)r G:(float)g B:(float)b A:(float)a;
-(AERGBA)borderColor;
-(void)setBackground:(AEImage*)newimage;
-(AEImage*)background;
-(AEUIArray*)subviews;
-(void)event:(AEUIViewEvent*)event;
-(void)focus;
-(void)loseFocus;
-(BOOL)hasFocus;
-(void)render;
-(void)click;
-(void)dealloc;

//Private
-(void)addSubview:(id)child;
-(void)removeSubview:(id)child;
@end

extern AEUIView* AEUIViewActive;

/*
size_t length=[self length];
	for(size_t i=0;i<length;i++)
		[[self at:i] performSelector:selector];
*/