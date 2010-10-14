#include <SDL_ttf/SDL_ttf.h>
#include <stdio.h>
#include "Font.h"
#include "osc simpletext/simple-text.h"
#include "SOIL.h"

SDL_Color AEFontColor = {255,255,255,255};
SDL_Color AEFontBackgoundColor = {0,0,0,0};

static int AEFontIsInitialized=0;

void AEFontInit(void){
	AEFontIsInitialized=1;
	/*if(TTF_Init()==-1){
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(1);
	}
	atexit(TTF_Quit);*/
	SDL_EnableUNICODE(1);
}

AEFont* AEFontNew(char* fontname,int size,float r,float g,float b,float a){
	if(not AEFontIsInitialized){
		AEFontInit();
	}
	/*TTF_Font* font = TTF_OpenFont(fontname, size);
	if(font==NULL){
		printf("Could not load font %s\n",fontname);
		exit(2);
	}
	SDL_Color fontcolor={r*255,g*255,b*255,a*255};//a*255
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
		SDL_Surface* temp = TTF_RenderText_Blended(font, string,fontcolor);
	
		SDL_Surface* textSurface=temp;
		
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
	return uifont;*/
	return NULL;
}


/*AEFont* AEFontNew(char* fontname,int size,float r,float g,float b,float a){
	if(AEFontIsInitialized==0) AEFontInit();
	TTF_Font* font = TTF_OpenFont(fontname, size);
	if(font==NULL){
		printf("Could not load font %s\n",fontname);
		exit(2);
	}
	SDL_Color fontcolor={r*255,g*255,b*255,a*255};//a*255
	//SDL_Color fontcolor={255,0,0,255};
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
	
	GLuint IDs[1];
	glGenTextures(1, IDs);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, IDs[0]);
	
	char allchars[129-32]={0};
	SDL_Surface* surfaces[129-32]={NULL};
	
	int offsetx=0;
	int totalwidth=0;
	int height=0;
	for(int i=0;i<(129-32);i++){
		allchars[i]=i+32;
		allchars[i+1]='\0';
		char chars[2]={i+32,'\0'};
		//int w=0,h=0;
		surfaces[i]=TTF_RenderText_Blended(font,chars,fontcolor);
		uifont->glyph[i].offsetx=offsetx;
		printf("%c %i %i\n",i+32,i,offsetx);
		offsetx+=surfaces[i]->w;
		//uifont->glyph[c].img=IDs[c-32];
		uifont->glyph[i].w=surfaces[i]->w;
		totalwidth+=uifont->glyph[i].w;
		height=uifont->glyph[i].h=surfaces[i]->h;
	}
	//allchars[128-32]='\0';
	//SDL_Surface* total = TTF_RenderText_Blended(font, allchars, fontcolor);
	//if(temp==NULL){
	//	printf("Failed to make font. TTF's Reason: %s\n",TTF_GetError());
	//}
	//SDL_SaveBMP(temp,"Font/Atlas.bmp");
	
	glTexImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, GL_RGBA, totalwidth, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	
	for(int i=0;i<(129-32);i++){
	
		uifont->glyph[i].offsetx;
		uifont->glyph[i].w;
		uifont->glyph[i].h;
		glBindTexture(GL_TEXTURE_RECTANGLE_ARB, IDs[0]);
		glTexSubImage2D(GL_TEXTURE_RECTANGLE_ARB, 0, uifont->glyph[i].offsetx, 0, uifont->glyph[i].w, uifont->glyph[i].h, GL_RGBA, GL_UNSIGNED_BYTE, surfaces[i]->pixels);
		//SDL_SaveBMP(surfaces[i],"Font/Char.bmp");
		SOIL_save_image
		(
			"Font/Char.tga",
			SOIL_SAVE_TYPE_TGA,
			surfaces[i]->w, surfaces[i]->h, 4,
			surfaces[i]->pixels
		);
		SDL_FreeSurface(surfaces[i]);
	}
	
	void* data=alloca(totalwidth*height*4);
	glGetTexImage(GL_TEXTURE_RECTANGLE_ARB,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		data);

	
	SOIL_save_image
	(
		"Font/Atlas.tga",
		SOIL_SAVE_TYPE_TGA,
		totalwidth, height, 4,
		data
	);
	
	//SDL_FreeSurface(temp);
	//abort();
	
	uifont->img=IDs[0];
	
	
	//for(unsigned char c=32;c<128;c++){
	//	char chars[2]={c,'\0'};
	//	int w=0,h=0;
	//	TTF_SizeText(font,chars,&w,&h);
	//	uifont->glyph[c].offsetx=offsetx;
	//	offsetx+=w;
	//	//uifont->glyph[c].img=IDs[c-32];
	//	uifont->glyph[c].w=w;
	//	uifont->glyph[c].h=h;
	//}
	glDisable(GL_TEXTURE_RECTANGLE_ARB);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB, 0);
	TTF_CloseFont(font);
	
	uifont->size=size;
	return uifont;
}*/

void AEFontRenderChar(AEFont* font,int c,int x,int y,int w,int h){
	/*glBindTexture(GL_TEXTURE_RECTANGLE_ARB,font->glyph[c].img);
	glBegin(GL_QUADS);
		unsigned int offsetx=0;//font->glyph[c].offsetx;
		glTexCoord2d(0+offsetx, font->glyph[c].h); glVertex3i(x, y+0, 0);
		glTexCoord2d(font->glyph[c].w+offsetx, font->glyph[c].h); glVertex3i(x+font->glyph[c].w,y+ 0, 0);
		glTexCoord2d(font->glyph[c].w+offsetx, 0); glVertex3i(x+font->glyph[c].w, y+font->glyph[c].h, 0);
		glTexCoord2d(0+offsetx, 0); glVertex3i(x, y+font->glyph[c].h, 0);
	glEnd();*/
}

void AEFontDelete(AEFont* font){
	/*//for(unsigned char c=32;c<128;c++) 
	glDeleteTextures(1, (GLuint*)&(font->img));
	free(font);*/
}


int AEFontSizeGet(AEFont* font){
	if(font) return font->size;
	return -1;
}