//Extended from Object.c Demo

#include <SDL/SDL.h>
#include "../AE/Core.h"
#include "../AE/Utility.h"
//#include "../AE/Node.h"
#include "../AE/FastMath3D.h"
#define true 1
#define false 0

AEObject* Cube=NULL;
AEVec3 Node={0,10,0};
AEVec3 Gravity={0,-10,0};

void perframe(float step){//Step in seconds since last frame
	AEObjectsSignal(AEObjectEventRender,NULL);// The default behavior of AEStart(NULL);, tells all objects to render
	AEObjectsPhysics(step,Gravity);
	AEObjectAimVelocityAt(Cube,&Node,AEVec3FromCoords(1,1,1));
}

int main(int argc,char** argv){
	AEInit("Window",256,256);
	
	Cube=AEObjectNew(NULL,NULL);//NULL/0 is the default parameters for most AE functions
	Cube->model=AEVBOLoad("Data/TexturedCube.obj",true,false);//.obj path, should cache in VRAM, and should keep normals
	Cube->texture=AETextureLoad("Data/Texture.png");//Good ol' texture
	
	//Move the camera back so we can see the cube
	AECamera.z=10;
	
	//Rotate the cube so we can see more of it
	Cube->rotation.z=Cube->rotation.x=45;
	
	AEStart(perframe);//Never returns, pass our frame updater in
	return 1;//We should never get here
}