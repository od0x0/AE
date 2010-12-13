#include "SDL.h"
#include "AE.h"

int AESDLKey(int);

static void OnFrameUpdate(AEContext* context, double seconds, void* userdata){
	double difference=(1.0/60.0)-seconds;
	if(difference>0) SDL_Delay(1000*difference);
}

int main(int argc, char** argv){
	AEContext* context=AEContextActiveGet();
	AEContextInit(context, "Ambition Engine Editor", 800, 500);
	AECameraPositionSet(AECameraActiveGet(), 0, 10, 10);
	AEContextRun(context);
	return 0;
}