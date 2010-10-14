#include "SDL.h"
#include "../AE/Core.h"
#include "../AE/VBO.h"
#include "../AE/NodeObjC.h"

#define MAXIMUM_FRAME_RATE 120
#define MINIMUM_FRAME_RATE 15
#define UPDATE_INTERVAL (1.0 / MAXIMUM_FRAME_RATE)
#define MAX_CYCLES_PER_FRAME (MAXIMUM_FRAME_RATE / MINIMUM_FRAME_RATE)

void perframe(float step){
	static double lastFrameTime = 0.0;
	static double cyclesLeftOver = 0.0;
	double currentTime;
	double updateIterations;
	
	currentTime = SDL_GetTicks();
	updateIterations = ((currentTime - lastFrameTime) + cyclesLeftOver);
	
	if (updateIterations > (MAX_CYCLES_PER_FRAME * UPDATE_INTERVAL)) {
		updateIterations = (MAX_CYCLES_PER_FRAME * UPDATE_INTERVAL);
	}
	
	while (updateIterations > UPDATE_INTERVAL) {
		updateIterations -= UPDATE_INTERVAL;
		//AENodeStep(World,UPDATE_INTERVAL);
		
	}
	
	cyclesLeftOver = updateIterations;
	lastFrameTime = currentTime;
	
	//AENodeRender(World);
}

int main(int argc,char** argv){
	AEInit("Window",800,500);
	//Code here
	AECameraPositionSet(AECameraActiveGet(),0,0,7);
	
	AEStart(perframe);
	return 1;
}