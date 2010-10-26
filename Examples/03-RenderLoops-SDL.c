#include "SDL.h"
#include "AECore.h"

void FrameUpdate(AEContext* context, double timeSinceLastCall, void* arg){
	puts("I'm supposed to be rendering and handling input, but I decided to SPAMYOURCONSOLEINSTEAD.");
}

void FixedUpdate(AEContext* context, double timeSinceLastCall, void* arg){
	puts("I'm supposed to be doing physics right now, but I decided to SPAMYOURCONSOLEINSTEAD.");
}

int main(int argc, char** argv){
	//AEContextActiveGet()->inFullscreen=true;
	AEContextInit(NULL, "A Simple Window", 800, 500);
	//This is used to set the specified callback, the last argument is the last arg passed back as the last argument of the callback
	AEContextCallbackSet(NULL, AEContextCallbackFrameUpdate, FrameUpdate, NULL);
	
	//The max should be divisible by the min/
	AEContextActiveGet()->fixedUpdateFrameRateMax=30;//Default is 60
	AEContextActiveGet()->fixedUpdateFrameRateMin=15;//Default is 15
	//Same function signature as frameupdate
	AEContextCallbackSet(NULL, AEContextCallbackFixedUpdate, FixedUpdate, NULL);
	AEContextRun(NULL);
	return 0;
}