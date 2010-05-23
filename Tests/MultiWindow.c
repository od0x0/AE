#include "../AE/Core.h"

#undef main

SDL_WindowID SubWindow;
SDL_GLContext SubWindowOpenGLContext;

void Close(void){
	SDL_DestroyWindow(SubWindow);
}

int main(int argc,char** argv){
	//AEInit("Main Window",800,500);
	SDL_Init(SDL_INIT_EVERYTHING);
	SubWindow=SDL_CreateWindow("Sub Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,200, 125,SDL_WINDOW_SHOWN);
	atexit(Close);
	//Code here
	
	while(1){
		SDL_PumpEvents();
		char* keys=SDL_GetKeyboardState(NULL);
		if((keys[(SDLK_LMETA)]||keys[(SDLK_LSUPER)])&&keys[(SDLK_q)]) exit(0);
	}
	
	return 1;
	//AEStart(NULL);
}
