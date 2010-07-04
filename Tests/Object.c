#include "../AE/Core.h"
#include "../AE/Camera.h"
#include "../AE/VBO.h"

unsigned int Texture=0;
AEVBO* VBO=NULL;

void perframe(float step){
	static float delta=0;
	delta+=10*step;
	glRotatef(delta,1,0,0);
	glRotatef(delta,0,1,0);
	AETextureBind(Texture);
	AEVBOBind(VBO);
	AEVBODraw(VBO);
}

int main(int argc,char** argv){
	AEInit("Window",800,500);
	AECamera* cam=AECameraActiveGet();
	cam->z=10;
	
	Texture=AETextureLoad("/Users/Oliver/Documents/Dev/Projects/AE/Junk/Old/Tests/Data/Texture.png");
	VBO=AEVBOLoad("/Users/Oliver/Documents/Dev/Projects/AE/Junk/Old/Tests/Data/TexturedCube.obj","tnvi","stream");
	
	AEStart(perframe);
	return 1;
}
