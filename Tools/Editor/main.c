#include "SDL.h"
#include "AE.h"
#include "EVContext.h"
#include "ECContext.h"

int AESDLKey(int);
int AESDLMouseButton(char);

EVContext ViewContext;
//EMContext ModelContext;
ECContext ControllerContext;

static void OnFrameUpdate(AEContext* context, double seconds, void* userdata){
	double difference=(1.0/60.0)-seconds;
	
	EVContextFrameUpdate(& ViewContext, seconds);
	
	if(difference>0) SDL_Delay(1000*difference);
}

static void OnFixedUpdate(AEContext* context, double seconds, void* userdata){
	double difference=(1.0/60.0)-seconds;
	
	EVContextFixedUpdate(& ViewContext, seconds);
	
	if(difference>0) SDL_Delay(1000*difference);
}

int main(int argc, char** argv){
	AEContext* context=AEContextActiveGet();
	AEContextOpen(context, "Ambition Engine Editor", 800, 500);
	AECameraPositionSet(AECameraActiveGet(), 0, 0, 3);
	
	EVContextInit(& ViewContext);
	//EMContextInit(& ModelContext);
	ECContextInit(& ControllerContext);
	
	context->frameUpdate=OnFrameUpdate;
	context->fixedUpdate=OnFixedUpdate;
	
	AEContextRun(context);
	return 0;
}
