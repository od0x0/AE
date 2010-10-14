#include "AESDL.h"
int AESDLEventFilter(const SDL_Event* event){
	//So it closes when the user says close
	if(event->type==SDL_QUIT) exit(0);
	return 1;
}

//AESDL* AESDLActive=NULL;

void AESDLInit(AEContext* context,char* title){
	int error = SDL_Init(SDL_INIT_EVERYTHING);
	if(error){
		puts("SDL failed to start");
		exit(1);
	}
	
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, context->r);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, context->g);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, context->b);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, context->a );
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, context->stencil);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, context->depth);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	SDL_SetVideoMode(context->w, context->h, 0, context->inFullscreen?(SDL_OPENGL | SDL_FULLSCREEN):SDL_OPENGL);
	SDL_WM_SetCaption(title,NULL);
	SDL_SetEventFilter(AESDLEventFilter);
}

void AESDLSwapBuffers(AEContext* context){
	SDL_GL_SwapBuffers();
}

void AESDLRefresh(AEContext* context){

}

int AESDLKey(int key){return SDL_GetKeyState(NULL)[key];}
int AESDLMouseButton(char button){return (SDL_BUTTON(button)&SDL_GetMouseState(NULL,NULL));}

int AESDLPollInput(AEContext* context){
	SDL_PumpEvents();
	if((AESDLKey(SDLK_LMETA)||AESDLKey(SDLK_LSUPER))&&AESDLKey(SDLK_q)) exit(0);
		//SDLK_LSUPER for Windoze and SDLK_LMETA for OS X
	return 1;
}

void AESDLDeInit(AEContext* context){
	SDL_Quit();
}

double AESDLSecondsGet(AEContext* context){
	return SDL_GetTicks()*0.001;
}

void AESDLBridge(void){
	AEContext* context=AEContextActiveGet();
	context->init=AESDLInit;
	context->pollinput=AESDLPollInput;
	context->swapbuffers=AESDLSwapBuffers;
	context->deinit=AESDLDeInit;
	context->seconds=AESDLSecondsGet;
	context->refresh=AESDLRefresh;
}