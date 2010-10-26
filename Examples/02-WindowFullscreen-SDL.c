#include "SDL.h"
#include "AECore.h"

int main(int argc, char** argv){
	AEContextActiveGet()->inFullscreen=true;
	//Any changes to the window state must be made before Init
	AEContextInit(NULL, "A Simple Window", 800, 500);
	AEContextRun(NULL);
	return 0;
}