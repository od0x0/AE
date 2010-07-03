#include <SDL_ttf/SDL_ttf.h>
#include <SDL/SDL.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

SDL_Color FontColor = {1,0,0,0};
SDL_Color FontBackgoundColor = {1,1,1,0};

void FontInit(void){
	if(TTF_Init()==-1){
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(1);
	}
	atexit(TTF_Quit);
	SDL_EnableUNICODE(1);
}

void FontConvert(char* fontname,int size){
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
	
	unsigned int length=strlen(fontname)+1;
	char* outputfontname=alloca(length);
	char* s=outputfontname;
	unsigned int j=0;
	for(unsigned int i=0;i<length;i++){
		if(fontname[i]==0 || fontname[i]=='.'){
			s[j++]=0;
			break;
		}
		else if(fontname[i]>32 && fontname[i]<128) s[j++]=fontname[i];
		
	}
	
	mkdir(outputfontname,0777);
	printf("Made \"%s\"",outputfontname);
	
	for(unsigned char c=32;c<128;c++){
		char string[2]={c,'\0'};
		SDL_Surface* image = TTF_RenderText_Solid(font, string,FontColor);
		//TTF_RenderText_Blended TTF_RenderText_Solid
		char* buffer=alloca(strlen(outputfontname)+10);
		sprintf(buffer,"%s/%i.bmp",outputfontname,(int)c);
		SDL_SaveBMP(image,buffer);
		SDL_FreeSurface(image);
	}
	TTF_CloseFont(font);
}


int main(int argc,char** argv){
	/*int error;
	error = SDL_Init(SDL_INIT_EVERYTHING);
	if(error){
		puts("SDL failed to start");
		exit(1);
	}
	
	SDL_Quit();*/
	printf("LINE: %i\n",__LINE__);
	if(argc<2){
		printf("Usage: ./ttf2bmp fontpath.ttf\n");
		return 1;
	}
	
	FontInit();
	FontConvert(argv[1],50);
	
	return 0;
}