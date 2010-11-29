#include "SDL.h"
#include "AE.h"
#include "EVContext.h"
#include "EMContext.h"

static EMContext ModelContext;
static EVContext ViewContext;

static void OnFixedUpdate(AEContext* context, double seconds, void* userdata){
	EMContextFixedUpdate(& ModelContext, seconds);
}

static void OnExit(void){
	EMContextDisconnectFromServer(& ModelContext);
	printf("Total Memory Allocations: %lu (%lu bytes)\n",(unsigned long)EUMemoryAllocationsTotal, (unsigned long)EUMemoryAllocationBytesTotal);
}

static void OnFrameUpdate(AEContext* context, double seconds, void* userdata){
	double difference=(1.0/60.0)-seconds;
	if(difference>0) SDL_Delay(1000*difference);
}

int main(int argc, char** argv){
	atexit(OnExit);
	AEContext* context=AEContextActiveGet();
	AEContextInit(context, "Ambition Engine Editor", 800, 500);
	EVContextInit(& ViewContext);
	EMContextInit(& ModelContext);
	EMContextConnectToServer(& ModelContext, "127.0.0.1", "Anonymous", "");
	AEContextCallbackSet(context, AEContextCallbackFixedUpdate, OnFixedUpdate, NULL);
	AEContextCallbackSet(context, AEContextCallbackFixedUpdate, OnFrameUpdate, NULL);
	AEContextRun(context);
	return 0;
}