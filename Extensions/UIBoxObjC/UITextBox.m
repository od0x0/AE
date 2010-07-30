#include "UITextBox.h"

AEVec2i AETextEndFitToBounds(char* string,AEFont* font,int x,int y,int w,int h);

@implementation AEUITextBox

-init{
	self=[super init];
	if(self){
		self->textbuffer=AETextBufferNew();
		self->caret=0;
		self->borderColor.a=1;
		self->highlightBorderColor.r=0;
		self->highlightBorderColor.g=(float)0x90/0xff;
		self->highlightBorderColor.b=(float)0xd0/0xff;
		self->highlightBorderColor.a=(float)0xff/0xff;
		//[self insert:"" At:0];
	}
	return self;
}

-resizeToFit:(char*)text{
	size_t length=strlen(text)+1;
	size_t lineCount=1;
	size_t maxwidth=0;
	size_t currentlinewidth=0;
	for(size_t i=0;i<length;i++){
		switch(text[i]){
			case '\n':
				lineCount++;
				if(currentlinewidth > maxwidth) maxwidth=currentlinewidth;
				currentlinewidth=0;
				break;
			default:
				if(text[i] > 32 && text[i] < 128)
					currentlinewidth+=self->font->glyph[text[i]-32].w;
				break;
		}
	}
	self->rectangle.w=maxwidth;
	self->rectangle.h=self->font->size*lineCount;
	return self;
}

-render{
	[super render];
	
	//if([self hasFocus]){
		AETextureBind(0);
		glLineWidth(2);
		//#ff9000
		AEUIBoxRect rect=self->rectangle;
		AERGBA bordercolor=self->borderColor;
		if([self hasFocus]) bordercolor=self->highlightBorderColor;
		unsigned char bordercolorbytes[4]={0xff,0xff,0xff,0xff};
		bordercolorbytes[0]*=bordercolor.r;
		bordercolorbytes[1]*=bordercolor.g;
		bordercolorbytes[2]*=bordercolor.b;
		bordercolorbytes[3]*=bordercolor.a;
		//glColor4ub(0x00,0x90,0xd0,0xff);
		glColor4ubv(bordercolorbytes);
		AEUIDrawRectOutline(rect);
		//int blur=2;
		/*for(int i=1;i<(blur+1);i++){
			glColor4ub(0xff,0x90,0x00,0xff/(float)i);
			AEUIBoxRect lines=AEUIBoxRectNew(rect.x-i*2, rect.y-i*2, rect.w-i*2, rect.h-i*2);
			AEUIDrawRectOutline(lines);
		}*/
		glColor3f(1,1,1);
		
		glLineWidth(1);
	//}
	
	AEUIBoxRect textrect;
	textrect.x=self->rectangle.x+2;
	float balance=((int)self->rectangle.h%(int)self->font->size)/2;
	textrect.y=self->rectangle.y+self->rectangle.h-self->font->size-balance+self->font->size*scroll;
	//if(AEKey(' ')) printf("%f=%f+%f-%f\n",(float)textrect.y,(float)self->rectangle.y,(float)self->rectangle.h,(float)self->font->size);
	textrect.w=self->rectangle.w;
	textrect.h=self->rectangle.h;
	glEnable(GL_SCISSOR_TEST);
	glScissor(self->rectangle.x, self->rectangle.y, self->rectangle.w, self->rectangle.h);
	AETextMultilineFitToBounds(AETextBufferText(self->textbuffer),self->font,textrect.x,textrect.y,textrect.w,textrect.h);
	
	if(self->locked==NO && [self hasFocus]){
		size_t length=AETextBufferLength(self->textbuffer)+1;
		char* tempbuffer=(char*)alloca(length);
		memcpy(tempbuffer,AETextBufferText(self->textbuffer),self->caret);
		tempbuffer[self->caret]=0;
		AEVec2i p=AETextEndFitToBounds(tempbuffer,self->font,textrect.x,textrect.y,textrect.w,textrect.h);
		self->lastKnownLinenumber=(size_t)(textrect.y-p.y)/(size_t)self->font->size;
		//AEText("_",self->font,p.x,p.y);
		//(((int)SDL_GetTicks())/400)
		glColor4f(self->caretColor.r, self->caretColor.g, self->caretColor.b,(((int)SDL_GetTicks())%400)/400.0f);
		AETextureBind(0);
		glLineWidth(2);
		glBegin(GL_LINES);
			//float offsetx=self->font->glyph['|'-32].w;
			float offsety=self->font?self->font->size/4:0;
			glVertex2f(p.x,p.y);
			glVertex2f(p.x,p.y+(self->font?self->font->size:0)+offsety);
		glEnd();
		glColor3f(1, 1, 1);
		
		/*//#ff9000
		glColor4ub(0xff,0x90,0x00,0xff);
		AEUIDrawRectOutline(self->rectangle);
		glColor3f(1,1,1);
		*/
		glLineWidth(1);
	}
	glDisable(GL_SCISSOR_TEST);
	//else AETextMultiline(AETextBufferText(self->textbuffer),self->font,self->rectangle.x+2,self->rectangle.y);
	return self;
}

-focus{
	SDL_EnableKeyRepeat(200, 100);
	[super focus];
	return self;
}

-cursorDown:(int)button{
	[super cursorDown:button];
	//printf("%i is down.",button);
	if([self hasFocus]){
		if(button==4){
			scroll--;
			if(scroll < 0) scroll=0;
		}
		else if(button==5){
			scroll++;
			if(scroll > lastKnownLinenumber) scroll=lastKnownLinenumber;
		}
	}
	return self;
}

-cursorUp:(int)button{
	[super cursorUp:button];
	return self;
}

-cursorMoveX:(int)x Y:(int)y{
	[super cursorMoveX:x Y:y];
	return self;
}

-addUnicodeChar:(int)key{return self;}

-keyDown:(int)key{
	if([self hasFocus]==NO) return self;
	if(key==127) [self backspace:1 At:[self caret]];
	else if(key==63236){scroll--; return self;}
	else if(key==63237){scroll++; return self;}
	else if(key==63235){if(self->caret<[self length]) self->caret++;}
	else if(key==63234){if(self->caret) self->caret--;}
	else if(key==13) [self insert:"\n" At:[self caret]];
	else if((32<=key)&&(key<128)){
		char s[2]={key,0};
		[self insert:s At:[self caret]];
	}
	scroll=lastKnownLinenumber;
	[super keyDown:key];
	return self;
}

-click{
	[self setCaret:[self length]];
	[super click];
	return self;
}

-(void)dealloc{
	AETextBufferDelete(self->textbuffer);
	[super dealloc];
}

-(size_t)caret{
	return self->caret;
}

-(void)setCaret:(size_t)offset{
	size_t length=[self length];
	if(offset < length) offset=length;
	self->caret=offset;
}

-(void)moveCaret:(int)shift{
	size_t length=[self length];
	if((shift + self->caret) < 0) self->caret=0;
	else if(shift + self->caret > length) self->caret=length;
	else self->caret+=shift;
}

-(void)setFont:(AEFont*)newfont{
	self->font=newfont;
}

-insert:(char*)text At:(size_t)offset{
	//size_t start=AETextBufferLength(self->textbuffer);
	AETextBufferInsert(self->textbuffer,offset,text);
	//size_t shift=AETextBufferLength(self->textbuffer)-start;
	[self moveCaret:strlen(text)];
	//[self setCaret:[self length]];
	return self;
}

-backspace:(size_t)length At:(size_t)offset{
	
	size_t start=AETextBufferLength(self->textbuffer);
	AETextBufferBackspace(self->textbuffer,offset-length,length);
	size_t shift=start-AETextBufferLength(self->textbuffer);
	[self moveCaret:-shift];
	//[self setCaret:[self length]];
	return self;
}

-(char*)text{
	return AETextBufferText(self->textbuffer);
}

-(size_t)length{
	return AETextBufferLength(self->textbuffer);
}

@end