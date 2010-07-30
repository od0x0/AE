#pragma once
#include "UIView.h"
#include "TextBuffer.h"

@interface AEUITextView : AEUIView {
	AETextBuffer* textbuffer;
	size_t caret;
	size_t max;
	float scroll;
	size_t lastKnownLinenumber;
	AERGB caretColor;
	AERGBA highlightBorderColor;
	BOOL locked;
}

-init;

-(void)render;

-(void)event:(AEUIViewEvent *)event;

-(void)click;
-(void)focus;
-(void)loseFocus;

-(void)dealloc;

-(size_t)caret;
-(void)setCaret:(size_t)offset;

-(void)insert:(char*)text At:(size_t)offset;
-(void)backspace:(size_t)length At:(size_t)offset;

-(char*)text;
-(size_t)length;

@end
