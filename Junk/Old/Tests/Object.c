#include <SDL/SDL.h>
#include "../AE/Core.h"
#define true 1
#define false 0

int main(int argc,char** argv){
	AEInit("Window",256,256);

	AEObject* o=AEObjectNew(NULL,NULL);//NULL/0 is the default parameters for most AE functions
	o->model=AEVBOLoad("Data/TexturedCube.obj",true,false);//.obj path, should cache in VRAM, and should keep normals
	o->texture=AETextureLoad("Data/Texture.png");//Good ol' texture

	//Move it so we can see it
	o->z=-5;
	o->rotation.z=o->rotation.x=45;
	
	AEStart(NULL);//Never returns
	return 1;//We should never get here
}

