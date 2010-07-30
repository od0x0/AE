#include "UITextView.h"


@implementation AEUITextView

-init{
	self=[super init];
	if(self){
		textbuffer=AETextBufferNew();
		caret=0;
		highlightBorderColor.r=0;
		highlightBorderColor.g=(float)0x90/0xff;
		highlightBorderColor.b=(float)0xd0/0xff;
		highlightBorderColor.a=(float)0xff/0xff;
		//[self insert:"" At:0];
	}
	return self;
}

-(void)render{
	[super render];
	
	/*
		AETextureBind(0);
		glLineWidth(2);
		//#ff9000
		AEUIRect rect=bounds;
		AERGBA bordercolor=borderColor;
		if([self hasFocus]) bordercolor=highlightBorderColor;
		unsigned char bordercolorbytes[4]={0xff,0xff,0xff,0xff};
		bordercolorbytes[0]*=bordercolor.r;
		bordercolorbytes[1]*=bordercolor.g;
		bordercolorbytes[2]*=bordercolor.b;
		bordercolorbytes[3]*=bordercolor.a;
		
		glColor4ubv(bordercolorbytes);
		AEUIDrawRectOutline(rect);
		
		glColor3f(1,1,1);
		
		glLineWidth(1);
	*/
	
	AEUIRect textrect;
	textrect.x=bounds.x+2;
	float balance=((int)bounds.h%(int)font->size)/2;
	textrect.w=bounds.w;
	textrect.h=bounds.h;
	textrect.y=bounds.y+bounds.h-font->size-balance+font->size*(scroll);
	//if(AEKey(' ')) printf("%f=%f+%f-%f\n",(float)textrect.y,(float)bounds.y,(float)bounds.h,(float)font->size);
	glEnable(GL_SCISSOR_TEST);
	glScissor(bounds.x, bounds.y, bounds.w, bounds.h);
	AETextMultilineFitToBounds(AETextBufferText(textbuffer),font,textrect.x,textrect.y,textrect.w,textrect.h);
	
	if(locked==NO && [self hasFocus]){
		size_t length=AETextBufferLength(textbuffer)+1;
		char* tempbuffer=(char*)alloca(length);
		memcpy(tempbuffer,AETextBufferText(textbuffer),caret);
		tempbuffer[caret]=0;
		AEVec2i p=AETextEndFitToBounds(tempbuffer,font,textrect.x,textrect.y,textrect.w,textrect.h);
		lastKnownLinenumber=(size_t)(textrect.y-p.y)/(size_t)font->size;
		//AEText("_",font,p.x,p.y);
		//(((int)SDL_GetTicks())/400)
		glColor4f(caretColor.r, caretColor.g, caretColor.b,(((int)SDL_GetTicks())%400)/400.0f);
		AETextureBind(0);
		glLineWidth(2);
		glBegin(GL_LINES);
			//float offsetx=font->glyph['|'-32].w;
			float offsety=font?font->size/4:0;
			glVertex2f(p.x,p.y);
			glVertex2f(p.x,p.y+(font?font->size:0)+offsety);
		glEnd();
		glColor3f(1, 1, 1);
		
		/*//#ff9000
		glColor4ub(0xff,0x90,0x00,0xff);
		AEUIDrawRectOutline(bounds);
		glColor3f(1,1,1);
		*/
		glLineWidth(1);
	}
	glDisable(GL_SCISSOR_TEST);
	//else AETextMultiline(AETextBufferText(textbuffer),font,bounds.x+2,bounds.y);
}

-(void)focus{
	SDL_EnableKeyRepeat(200, 100);
	AEStateActiveGet()->blockKeyInput=1;
	[super focus];
}

-(void)loseFocus{
	SDL_EnableKeyRepeat(0, 0);
	AEStateActiveGet()->blockKeyInput=0;
	[super loseFocus];
}

-(void)event:(AEUIViewEvent*)event{
	[super event:event];
	switch(event->type){
		case AEUIViewEventTypeKey:{
			if(event->data.key.wasPressed==0) break;
			int key=event->data.key.unicode;
			if([self hasFocus]==NO) break;
			if(key==127) [self backspace:1 At:[self caret]];
			else if(key==63236){scroll--; break;}
			else if(key==63237){scroll++; break;}
			else if(key==63235){if(caret<[self length]) caret++;}
			else if(key==63234){if(caret) caret--;}
			else if(key==13) [self insert:"\n" At:[self caret]];
			else if((32<=key)&&(key<128)){
				char s[2]={key,0};
				[self insert:s At:[self caret]];
			}
			if(scroll > lastKnownLinenumber){
				scroll=lastKnownLinenumber;
				
			}
			//size_t drawablelines=(size_t)(bounds.w)/(size_t)font->size;
			//scroll=lastKnownLinenumber/drawablelines;
			/*
			scroll=lastKnownLinenumber;
			if(scroll > drawablelines) scroll-=(size_t)scroll%(size_t)drawablelines;*/
			}break;
		case AEUIViewEventTypeMouseButton:{
			if(event->data.mousebutton.wasPressed) break;
			int button=event->data.mousebutton.button;
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
			}break;
	}
}

-(void)click{
	[self setCaret:[self length]];
	[super click];
}

-(void)dealloc{
	AETextBufferDelete(textbuffer);
	[super dealloc];
}

-(size_t)caret{
	return caret;
}

-(void)setCaret:(size_t)offset{
	size_t length=[self length];
	if(offset < length) offset=length;
	caret=offset;
}

-(void)moveCaret:(int)shift{
	size_t length=[self length];
	if((shift + caret) < 0) caret=0;
	else if(shift + caret > length) caret=length;
	else caret+=shift;
}

-(void)insert:(char*)text At:(size_t)offset{
	AETextBufferInsert(textbuffer,offset,text);
	
	[self moveCaret:strlen(text)];
}

-(void)backspace:(size_t)length At:(size_t)offset{
	size_t start=AETextBufferLength(textbuffer);
	AETextBufferBackspace(textbuffer,offset-length,length);
	size_t shift=start-AETextBufferLength(textbuffer);
	
	[self moveCaret:-shift];
}

-(char*)text{
	return AETextBufferText(textbuffer);
}

-(size_t)length{
	return AETextBufferLength(textbuffer);
}

@end
