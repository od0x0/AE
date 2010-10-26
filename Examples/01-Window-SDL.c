#include "SDL.h"
//AESDLMain.m does some magic set up for AE that fills in the context for us, you could always use your own set up if you wanted, look at AESDLMain.m to see how it is done.
#include "AECore.h"

int main(int argc, char** argv){
	//NULL is synonymous with AEContextActiveGet() for Init and Run
	//Opens our window
	AEContextInit(NULL, "A Simple Window", 800, 500);
	//Enter our renderloop
	AEContextRun(NULL);
	//We probably won't get here because the AE-SDL bridging likes to exit(0) when the window is closed, and so AEContextStart will not return.  However, if we used some other bridging, it would be entirely possible that AEContextStart could return.
	return 0;
}