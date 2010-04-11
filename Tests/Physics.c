#include "../AE/Core.h"
#include "../AE/FastMath3D.h"
#include "../AE/Utility.h"
#define true 1
#define false 0

AEVec3 Gravity={-9.8,0,0};
AEObject* Cube=NULL;
AEObject* StaticCube=NULL;

void perframe(float step){
	AEObjectsPhysics(step,Gravity);
	if(AEKey(' ')){
		Cube->x=10;
		Cube->z=1;
	}
	//Cube->y=2;
	AEObjectsSignal(AEObjectEventRender,NULL);
}

int main(int argc,char** argv){
	AEInit("Physics Test",800,500);

	Cube=AEObjectNew(NULL,NULL);//NULL/0 is the default parameters for most AE functions
	Cube->model=AEVBOLoad("Data/TexturedCube.obj",true,false);//.obj path, should cache in VRAM, and should keep normals
	Cube->texture=AETextureLoad("Data/Texture.png");//Good ol' texture
	Cube->x=10;
	Cube->size=AEVBOAABB(Cube->model);
	Cube->mass=1;
	
	StaticCube=AEObjectNew(NULL,NULL);//NULL/0 is the default parameters for most AE functions
	StaticCube->model=AEVBOLoad("Data/TexturedCube.obj",true,false);//.obj path, should cache in VRAM, and should keep normals
	StaticCube->texture=AETextureLoad("Data/Texture.png");//Good ol' texture
	StaticCube->isStatic=1;
	StaticCube->size=AEVBOAABB(StaticCube->model);
	
	//Move camera back so we can see the boxes
	AECamera.z=8;
	AECamera.y=10;
	AECamera.rotation.x=-70;
	
	//Cube->rotation.z=Cube->rotation.x=StaticCube->rotation.z=StaticCube->rotation.x=45;
	
	AEStart(perframe);//Never returns
	return 1;//We should never get here
}

