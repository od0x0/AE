#include "../../AE/Core.h"
#include "../../AE/Geo.h"

#include "Map.c"

void Control(float step){
	float speed=50*step;
	AEVec3 movement={0,0,0};
	
	if(AEKey('w')) movement.z-=speed;
	if(AEKey('s')) movement.z+=speed;
	if(AEKey('a')) movement.x-=speed;
	if(AEKey('d')) movement.x+=speed;
	if(AEKey(' ')) movement.y+=speed;
	if(AEKey(SDLK_LSHIFT)) movement.y-=speed;
	
	if((movement.x+movement.y+movement.z)==0){
		AECamera.y+=movement.y;
		AEMoveXZ(&AECamera,movement.x,movement.z);
	}
	
	AEUIRenderMode(1);
	
	AEUIRenderMode(0);
}

void Render(float step){
	AEObjectsSignal(AEObjectEventRender,NULL);
}

void Update(float step){
	Render(step);
	Control(step);
}

int main(int argc,char** argv){
	AEInit("AeEd",800,500);
	AEStart(Update);
	return 1;
}