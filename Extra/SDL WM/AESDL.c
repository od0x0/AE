#include "AESDL.h"

/*int AESDLEventFilter(const SDL_Event* event){
	//So it closes when the user says close
	if(event->type==SDL_QUIT) exit(0);
	return 1;
}

//AESDL* AESDLActive=NULL;

void AESDLOpen(AEContext* context,const char* title){
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
	if(context->multisample){
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, context->multisample);
	}
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	SDL_SetVideoMode(context->w, context->h, 0, context->inFullscreen?(SDL_OPENGL | SDL_FULLSCREEN):SDL_OPENGL);
	SDL_WM_SetCaption(title,NULL);
	SDL_SetEventFilter(AESDLEventFilter);
	
	if(context->multisample) glEnable(GL_MULTISAMPLE);
}

void AESDLSwapBuffers(AEContext* context){
	SDL_GL_SwapBuffers();
}

void AESDLRefresh(AEContext* context){
	//A dud, we don't actually do anything here.
}

int AESDLKey(int key){return SDL_GetKeyState(NULL)[key];}
int AESDLMouseButton(char button){return (SDL_BUTTON(button)&SDL_GetMouseState(NULL,NULL));}

int AESDLPollInput(AEContext* context){
	SDL_PumpEvents();
	if((AESDLKey(SDLK_LMETA)||AESDLKey(SDLK_LSUPER))&&AESDLKey(SDLK_q)) exit(0);
		//SDLK_LSUPER for Windoze and SDLK_LMETA for OS X
	return 1;
}

void AESDLClose(AEContext* context){
	SDL_Quit();
}

double AESDLSecondsGet(AEContext* context){
	return SDL_GetTicks()*0.001;
}

void AESDLSetupContext(AEContext* context){
	context->open=AESDLOpen;
	context->pollInput=AESDLPollInput;
	context->swapBuffers=AESDLSwapBuffers;
	context->close=AESDLClose;
	context->seconds=AESDLSecondsGet;
	context->refresh=AESDLRefresh;
}

void AESDLRefreshContext(void){
	//[[NSOpenGLContext currentContext] update];
}*/

int AESDLEventFilter(const SDL_Event* event){
	//So it closes when the user says close
	if(event->type==SDL_QUIT) exit(0);
	return 1;
}

//AESDL* AESDLActive=NULL;

void AESDLOpen(AEWindow* window,const char* title){
	int error = SDL_Init(SDL_INIT_EVERYTHING);
	if(error){
		puts("SDL failed to start");
		exit(1);
	}
	
	AEWindowFormat format;
	AEWindowGetFormat(window, & format);
	
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, format.colorBits.r);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, format.colorBits.g);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, format.colorBits.b);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, format.colorBits.a );
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, format.stencilBits);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, format.depthBits);
	if(format.multisample){
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, format.multisample);
	}
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);
	
	int w, h;
	AEWindowGetSize(window, &w, &h);
	
	SDL_SetVideoMode(w, h, 0, format.inFullscreen?(SDL_OPENGL | SDL_FULLSCREEN):SDL_OPENGL);
	SDL_WM_SetCaption(title,NULL);
	SDL_SetEventFilter(AESDLEventFilter);
	
	if(format.multisample) glEnable(GL_MULTISAMPLE);
}

void AESDLSwapBuffers(AEWindow* window){
	SDL_GL_SwapBuffers();
}

void AESDLRefresh(AEWindow* window){
	//A dud, we don't actually do anything here.
}

int AESDLKey(int key){return SDL_GetKeyState(NULL)[key];}
int AESDLMouseButton(char button){return (SDL_BUTTON(button)&SDL_GetMouseState(NULL,NULL));}

int AESDLPollInput(AEWindow* window){
	SDL_PumpEvents();
	if((AESDLKey(SDLK_LMETA)||AESDLKey(SDLK_LSUPER))&&AESDLKey(SDLK_q)) exit(0);
		//SDLK_LSUPER for Windoze and SDLK_LMETA for OS X
	return 1;
}

void AESDLClose(AEWindow* window){
	SDL_Quit();
}

double AESDLGetSeconds(AEWindow* window){
	return SDL_GetTicks()*0.001;
}

const AEWindowContextCallbacks* AESDLWindowContextCallbacks(void){
	static const AEWindowContextCallbacks callbacks={
		.open=AESDLOpen,
		.refresh=AESDLRefresh,
		.pollInput=AESDLPollInput,
		.swapBuffers=AESDLSwapBuffers,
		.close=AESDLClose,
		.seconds=AESDLGetSeconds
	};
	return & callbacks;
}