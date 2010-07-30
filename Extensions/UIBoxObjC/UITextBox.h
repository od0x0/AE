#pragma once
#include "UIBox.h"
#include "TextBuffer.h"

@interface AEUITextBox : AEUIBox{
@public
	AETextBuffer* textbuffer;
	size_t caret;
	size_t max;
	float scroll;
	size_t lastKnownLinenumber;
	AERGB caretColor;
	AERGBA highlightBorderColor,borderColor;
	BOOL locked;
}

-init;
-resizeToFit:(char*)text;

-render;

-cursorDown:(int)button;
-cursorUp:(int)button;
-cursorMoveX:(int)x Y:(int)y;

-keyDown:(int)key;
-addUnicodeChar:(int)key;

-click;

-(void)dealloc;

-(size_t)caret;
-(void)setCaret:(size_t)offset;

-(void)setFont:(AEFont*)newfont;

-insert:(char*)text At:(size_t)offset;
-backspace:(size_t)length At:(size_t)offset;

-(char*)text;
-(size_t)length;

@end