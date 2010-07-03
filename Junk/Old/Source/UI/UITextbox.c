#include <string.h>
#include <stdlib.h>
#include "../UITextbox.h"

static void AEUITextboxAddChar(AEUITextbox* self,char c){
	/*if((self->caret+1)>self->max) return;
	size_t end=strlen(self->text)+1;
	if(end>self->max) return;
	self->text[end+1]='\0';
	for(unsigned int i=end;i>self->caret;i--){
		self->text[i+1]=self->text[i];
	}
	self->text[self->caret]=c;*/
	self->caret++;
	unsigned int index=strlen(self->text);
	if((index+1)>self->max) return;
	self->text[index]=c;
	self->text[index+1]='\0';
	
	/*unsigned int length=strlen(self->text);
	self->text[length]=c;
	self->text[length+1]='\0';*/
}

static void AEUITextboxBackspace(AEUITextbox* self){
	/*if(self->caret<=0) return;
	char* string=self->text+self->caret;
	while(string[0] || string[1]){
		string[1]=string[0];
		string++;
	}
	string[0]='\0';
	self->caret--;*/
	//unsigned int index=strlen(self->text)-1+1;//index of the NULL byte
	unsigned int length=strlen(self->text);
	if(length) self->text[length-1]='\0';
	//if(index<=0) return;
	//self->text[index]='\0';
}

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
			self->text=NULL;
			self->max=0;
			self->caret=0;
			self->font=NULL;
			return self;
			
		case AEUIBoxEventRender:
			//AETextMultiline(self->text,self->font,self->x,self->y);
			if(self->locked) AETextMultiline(self->text,self->font,self->x,self->y);
			else if(AEUIBoxActive==(AEUIBox*)self){
				static double lastBlinkTime=0;
				size_t length=strlen(self->text)+2;
				char* tempbuffer=(char*)alloca(length);
				if(((SDL_GetTicks()-lastBlinkTime)>400.0)) lastBlinkTime=SDL_GetTicks();
				if(((SDL_GetTicks()-lastBlinkTime)<300.0)){
					//sprintf(tempbuffer,"%s|",self->text);
					strcpy(tempbuffer,self->text);
					tempbuffer[length-2]='|';
					tempbuffer[length-1]='\0';
					AETextMultiline(tempbuffer,self->font,self->x,self->y);
				}else AETextMultiline(self->text,self->font,self->x,self->y);
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
			AEUIBoxSignalChildren((AEUIBox*)self,event,data);
			free(self->subBoxes);
			free(self->text);
			free(self);
			break;
		
		case AEUIBoxEventValue:
			return self->text;
	}
	return AEUIBoxDefaultEvent((AEUIBox*)self,event,data);
}