#include <string.h>
#include <stdlib.h>
#include "../UITextbox.h"

void AEUITextboxKeyboardSDLEvent(AEUITextbox* self,const SDL_Event* event){
	if(event->type!=SDL_KEYDOWN) return;
	else if(event->key.type==SDL_KEYUP) return;
	else if(event->key.keysym.sym==SDLK_BACKSPACE){AEUITextboxBackspace(self);return;}
	else if(event->key.keysym.sym==SDLK_UP)return;
	else if(event->key.keysym.sym==SDLK_DOWN)return;
	else if(event->key.keysym.sym==SDLK_RIGHT){if(self->caret<(strlen(self->text))) self->caret++;return;}
	else if(event->key.keysym.sym==SDLK_LEFT){if(self->caret) self->caret--;return;}
	else if(event->key.keysym.sym==SDLK_RETURN) AEUITextboxAddChar(self,'\n');
	else if((32<=event->key.keysym.unicode)&&(event->key.keysym.unicode<128)){
		if((32<=event->key.keysym.unicode)&&(event->key.keysym.unicode<128)) AEUITextboxAddChar(self,event->key.keysym.unicode);
	}
	/*else{
		int c=0;
		if ( (event->key.keysym.unicode & 0xFF80) == 0 ) c = event->key.keysym.unicode & 0x7F;
		if((32<=c)&&(c<128)) OUTextboxAdd(self,c);
	}*/
}

void* AEUITextboxEvent(AEUITextbox* self,int event,void* data){
	switch(event){
	
		case AEUIBoxEventInit:
			self=realloc(self,sizeof(AEUITextbox));
			self->textbuffer=NULL;
			self->font=NULL;
			return self;
			
		case AEUIBoxEventRender:
			//AETextMultiline(self->text,self->font,self->x,self->y);
			if(self->locked) AETextMultiline(AETextBufferText(self->text),self->font,self->super.x,self->super.y);
			else if(AEUIBoxActive==(AEUIBox*)self){
				static double lastBlinkTime=0;
				size_t length=AETextBufferLength(self->textbuffer)+2;
				char* tempbuffer=(char*)alloca(length);
				if(((SDL_GetTicks()-lastBlinkTime)>400.0)) lastBlinkTime=SDL_GetTicks();
				if(((SDL_GetTicks()-lastBlinkTime)<300.0)){
					//sprintf(tempbuffer,"%s|",self->text);
					strcpy(tempbuffer,AETextBufferText(self->text));
					tempbuffer[length-2]='|';
					tempbuffer[length-1]='\0';
					AETextMultiline(tempbuffer,self->font,self->super.x,self->super.y);
				}else AETextMultiline(AETextBufferText(self->text),self->font,self->super.x,self->super.y);
			}
			break;
			
		case AEUIBoxEventSDLEvent:{
			if(self->locked) break;
			const SDL_Event* event=(const SDL_Event*)data;
			switch(event->type){
				case SDL_KEYDOWN:
					AEUITextboxKeyboardSDLEvent(self,event);
					break;
			}
			}break;
		
		case AEUIBoxEventClicked:
			AEUIBoxActive=(AEUIBox*)self;
			break;
		
		case AEUIBoxEventRelease:
			AETextBufferDelete(self->textbuffer);
			break;
		
		case AEUIBoxEventValue:
			return self->textbuffer;
	}
	return AEUIBoxDefaultEvent((AEUIBox*)self,event,data);
}