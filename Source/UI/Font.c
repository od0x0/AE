#include <SDL_ttf/SDL_ttf.h>
#include <stdio.h>
#include "../Font.h"

SDL_Color AEFontColor = {0,0,0,1};
SDL_Color AEFontBackgoundColor = {1,1,1,1};

void AEFontInit(void){
	if(TTF_Init()==-1){
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(1);
	}
	atexit(TTF_Quit);
	SDL_EnableUNICODE(1);
}

AEFont* AEFontNew(char* fontname,int size){
	TTF_Font* font = TTF_OpenFont(fontname, size);
	if(font==NULL){
		printf("Could not load font %s\n",fontname);
		exit(2);
	}
	Uint32 rmask, gmask, bmask, amask;
	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
	#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
	#endif
	
	AEFont* uifont=(AEFont*)calloc(1,sizeof(AEFont));
	
	uifont->size=size;
	
	glEnable(GL_TEXTURE_RECTANGLE_ARB);
	
	GLuint IDs[128-32];
	glGenTextures(128-32, IDs);
	
	for(unsigned char c=32;c<128;c++){
		char string[2]={c,'\0'};
		SDL_Surface* temp = TTF_RenderText_Solid(font, string,AEFontColor);
		//TTF_RenderText_Blended TTF_RenderText_Solid
		/*
		char buffer[256];
		sprintf(buffer,"Font/%i.bmp",(int)c);
		SDL_SaveBMP(temp,buffer);//Debuging
		//*/
		
		SDL_Surface* textSurface=SDL_CreateRGBSurface(SDL_SWSURFACE,temp->w , temp->h, 32, rmask, gmask, bmask, amask);
		SDL_BlitSurface(temp, NULL, textSurface, NULL);
		SDL_FreeSurface(temp);
		
		glBindTexture(GL_TEXTURE_RECTANGLE_ARB, IDs[c-32]);
		glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, textSurface->w, textSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, textSurface->pixels);
		
		SDL_FreeSurface(textSurface);
		
		uifont->glyph[c].img=IDs[c-32];
		uifont->glyph[c].w=textSurface->w;
		uifont->glyph[c].h=textSurface->h;
	}
	glDisable(GL_TEXTURE_RECTANGLE_ARB);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, 0);
	TTF_CloseFont(font);
	
	uifont->size=size;
	return uifont;
}

void AEFontRelease(AEFont* font){
	for(unsigned char c=32;c<128;c++) glDeleteTextures(1, (GLuint*)&(font->glyph[c].img));
	free(font);
}
