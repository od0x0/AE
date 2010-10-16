#include "SDL.h"
#include "Core.h"

void FrameUpdate(AEContext* context, double timeSinceLastCall, void* arg){
	puts("I'm supposed to be rendering, but I decided to SPAMYOURCONSOLEINSTEAD.");
}

int main(int argc, char** argv){
	//AEContextActiveGet()->inFullscreen=true;
	AEContextInit(NULL, "A Simple Window", 800, 500);
	//This is used to set the specified callback, the last argument is the last arg passed back as the last argument of the callback
	AEContextCallbackSet(NULL, AEContextCallbackFrameUpdate, FrameUpdate, NULL);
	AEContextRun(NULL);
	return 0;
}