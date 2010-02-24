#include <SDL/SDL.h>
#include <AE/Core.h>
void perframe(float step){
	float speed=step*50;
	if(AEKey('w')) AEMoveXZ(&AECamera,0,-speed);
	if(AEKey('s')) AEMoveXZ(&AECamera,0,speed);
	if(AEKey('d')) AEMoveXZ(&AECamera,speed,0);
	if(AEKey('a')) AEMoveXZ(&AECamera,-speed,0);
	AEObjectsSignal(AEObjectEventRender,NULL);
	char title[256];
	sprintf(title,"Camera is at {%.1f,0.0,%.1f}",AECamera.x,AECamera.z);
	SDL_WM_SetCaption(title,NULL);
}
int main(int argc,char** argv){
	AEInit("Window",800,500);
	//Setup scene here
	AEStart(perframe);
	return 1;
}
