#include "SDL.h"
#include "AE.h"
#include "AECore.h"
//Core.h includes Camera.h, but it doesn't hurt to include it here
#include "AECamera.h"
#include "AEVA.h"

int AESDLKey(int key);
int AESDLMouseButton(char button);

//Yes, globals are bad, but this is a simple example of something, not production-ready software.
AETexture Texture;
AEVA VertexArray, IndexArray;


void FrameUpdate(AEContext* context, double timeSinceLastCall, void* arg){
	AETextureBind(Texture);
	AEVABindIndices(& IndexArray);
	AEVABindInterleavedTNV(& VertexArray);
	AEVADraw(0, IndexArray.length);
	AEVABindIndices(NULL);
	AEVABindInterleavedTNV(NULL);
	AETextureBind(0);
	
	AEVec3 move={0,0,0};
	double speed=timeSinceLastCall*5;
	
	if(AESDLKey('w')) move.z-=speed;
	if(AESDLKey('s')) move.z+=speed;
	if(AESDLKey('a')) move.x-=speed;
	if(AESDLKey('d')) move.x+=speed;
	
	AEVec2i mouse;
	SDL_GetMouseState(&mouse.x, &mouse.y);
	
	AEVec3 turn={0,0,0};
	double turnspeed=10*timeSinceLastCall;
	AECameraRotationGet(AECameraActiveGet(),&turn);
	
	AEVec2i screenCenter={
		.x= AEContextActiveGet()->w/2,
		.y= AEContextActiveGet()->h/2
	};
	turn.y-=(mouse.x-screenCenter.x);
	turn.x-=(mouse.y-screenCenter.y);
	if(turn.x > 90) turn.x = 90;
	if(turn.x < -90) turn.x = -90;
	
	SDL_WarpMouse(screenCenter.x,screenCenter.y);
	
	AECameraRotationSet(AECameraActiveGet(),turn.x,turn.y,turn.z);
	AEVec3 pos={0,0,0};
	AECameraPositionGet(AECameraActiveGet(),&pos);
	pos=AEVec3Add(AEQuatMulVec3(AEQuatFromEuler(turn),move),pos);
	AECameraPositionSet(AECameraActiveGet(),pos.x,pos.y,pos.z);
}

void Cleanup(void){
	AEVADeinit(& VertexArray);
	AEVADeinit(& IndexArray);
	AETextureDelete(Texture);
}

int main(int argc, char** argv){
	//AEContextActiveGet()->inFullscreen=true;
	AEContextInit(NULL, "A Simple Window", 800, 500);
	
	//Move the camera back 7 units (OpenGL coordinates, +x is right, +y is up, -z is forward)
	AECameraPositionSet(AECameraActiveGet(), 0, 0, 7);
	//Change the near plane to 1, the far plane to 100, and the fov to 90, the defaults are 1, 3000, and 60
	AECameraBoundsSet(AECameraActiveGet(),1, 100, 90);
	
	AEVec2i screenCenter={
		.x= AEContextActiveGet()->w/2,
		.y= AEContextActiveGet()->h/2
	};
	SDL_WarpMouse(screenCenter.x,screenCenter.y);
	
	Texture=AETextureLoad("Data/Texture.png");
	
	AEVAInit(& VertexArray, false, AEVAVBOTypeStatic, 1, AEVADataFormat3NF3VF);
	AEVAInit(& IndexArray, true, AEVAVBOTypeStatic, 0, 0);
	AEVALoadFromObj(& VertexArray, & IndexArray, "Data/Cube.obj");
	
	atexit(Cleanup);
	AEContextCallbackSet(NULL, AEContextCallbackFrameUpdate, FrameUpdate, NULL);
	
	AEContextRun(NULL);
	return 0;
}