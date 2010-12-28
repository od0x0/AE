#include "SDL.h"
#include "AE.h"
#include "ECOperationList.h"
#include "AETextBuffer.h"
#include "EVContext.h"
#include "LuaBinding.h"

int AESDLKey(int);
int AESDLMouseButton(char);

EVContext ViewContext;

static void OnFrameUpdate(AEContext* context, double seconds, void* userdata){
	double difference=(1.0/60.0)-seconds;
	
	EVContextFrameUpdate(& ViewContext, seconds);
	EVContextFixedUpdate(& ViewContext, seconds);
	
	if(difference>0) SDL_Delay(1000*difference);
}

int main(int argc, char** argv){
	AEContext* context=AEContextActiveGet();
	AEContextOpen(context, "Ambition Engine Editor", 800, 500);
	AECameraPositionSet(AECameraActiveGet(), 0, 0, 3);
	
	EVContextInit(& ViewContext);
	
	glClearColor(0, 0.6, 0.9, 1);
	context->frameUpdate=OnFrameUpdate;
	AEContextRun(context);
	return 0;
}
