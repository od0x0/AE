#include "SDL.h"
#include "AECore.h"

//There's no header for it, but AESDLMain.m defines these simple wrappers
int AESDLKey(int key);
int AESDLMouseButton(char button);

void FrameUpdate(AEContext* context, double timeSinceLastCall, void* arg){
	if(AESDLKey(' ')) puts("PLZPLZPZLPZLZPLZ letmego, iwannabefreeeeeeeeeee.");
	else puts("IRSTILLSPAMURCONSOLE!oloololololololololo");
}

int main(int argc, char** argv){
	//AEContextActiveGet()->inFullscreen=true;
	AEContextInit(NULL, "A Simple Window", 800, 500);
	AEContextCallbackSet(NULL, AEContextCallbackFrameUpdate, FrameUpdate, NULL);
	AEContextRun(NULL);
	return 0;
}