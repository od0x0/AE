#include "../AE/Core.h"
//#include <SDL/SDL.h>
#include "SDL.h"

typedef struct{
	AEWM super;
	unsigned char* keys;
	unsigned int mouseButtons;
}AESDL;

static int AESDLEventFilter(const SDL_Event* event){
	//So it closes when the user says close
	if(event->type==SDL_QUIT) exit(0);
	return 1;
}

//AESDL* AESDLActive=NULL;

static AEWM* AESDLNew(char* title,AEState* state){
	AEWM* wm=calloc(1,sizeof(AESDL));
	wm->size=sizeof(AESDL);
	
	int error = SDL_Init(SDL_INIT_EVERYTHING);
	if(error){
		puts("SDL failed to start");
		exit(1);
	}
	
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, state->r);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, state->g);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, state->b);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, state->a );
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, state->stencil);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, state->depth);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	SDL_SetVideoMode(state->w, state->h, 0, state->inFullscreen?(SDL_OPENGL | SDL_FULLSCREEN):SDL_OPENGL);
	SDL_WM_SetCaption(title,NULL);
	SDL_SetEventFilter(AESDLEventFilter);
	
	//AESDLActive=(AESDL*)wm;
	
	wm->state=*state;
	return wm;
}

static AEState* AESDLStateGet(AEWM* wm){
	return &(wm->state);
}

static void AESDLStateSet(AEWM* wm,AEState* state){
	wm->state=*state;
}

static void AESDLSwapBuffers(AEWM* wm){
	SDL_GL_SwapBuffers();
}

int AEKey(int key){return SDL_GetKeyState(NULL)[key];}
int AEMouseButton(char button){return (SDL_BUTTON(button)&SDL_GetMouseState(NULL,NULL));}

static int AESDLPollInput(AEWM* wm){
	//if(wm->state.blockKeyInput==0) 
	SDL_PumpEvents();
	AESDL* sdl=(AESDL*)wm;
	
	//AESDLActive=sdl;
	
	//sdl->keys=SDL_GetKeyState(NULL);
	SDL_PumpEvents();
	if((AEKey(SDLK_LMETA)||AEKey(SDLK_LSUPER))&&AEKey(SDLK_q)) exit(0);
		//SDLK_LSUPER for Windoze and SDLK_LMETA for OS X
	//static unsigned char blankKeys[SDLK_LAST];
	//if(wm->state.blockKeyInput) sdl->keys=blankKeys;
	
	//sdl->mouseButtons=
	SDL_GetMouseState(&(wm->state.mouse.x),&(wm->state.mouse.y));
	
	return 1;
}

static void AESDLDelete(AEWM* wm){
	if(wm==NULL) return;
	free(wm);
	SDL_Quit();
}

static double AESDLSecondsGet(AEWM* wm){
	return SDL_GetTicks()*0.001;
}

static void AESDLInit(void){
	AEWMNew=AESDLNew;
	
	AEWMStateGet=AESDLStateGet;
	AEWMStateSet=AESDLStateSet;
	AEWMPollInput=AESDLPollInput;
	AEWMSwapBuffers=AESDLSwapBuffers;
	AEWMDelete=AESDLDelete;
	AEWMSecondsGet=AESDLSecondsGet;
}

int main(int argc,char** argv){
	AESDLInit();
	AEInit("Window",800,500);
	/*
	extern AEWM* (*AEWMNew)(char* title, AEState* state);
extern AEState* (*AEWMStateGet)(AEWM* wm);
extern void (*AEWMStateSet)(AEWM* wm,AEState* state);
extern int (*AEWMPollInput)(AEWM* wm);
extern void (*AEWMSwapBuffers)(AEWM* wm);
extern void (*AEWMDelete)(AEWM* wm);
extern double (*AEWMSecondsGet)(AEWM* wm);
	*/
	//Code here
	AEStart(NULL);
}
