#include "../AE/Core.h"
#include "../AE/Camera.h"
#include "../Extra/Grass.h"
#include "../AE/FastMath3D.h"

const unsigned int GrassLocationCount=256;
AEVec3 GrassLocations[512];
GrassPatch* Patch=NULL;

float Random(void){
	int seed=rand();
	srand(seed);
	return (float)seed/RAND_MAX;
}

void perframe(float step){
	static float delta=0;
	delta+=10*step;
	glRotatef(AEActiveState.mouse.y,1,0,0);
	glRotatef(delta,0,1,0);
	AETextureBind(Patch->texture);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	GrassPatchRender(Patch);
}

int main(int argc,char** argv){
	AEInit("Window",800,500);
	AECamera* cam=AECameraActiveGet();
	cam->z=10;
	glDisable(GL_CULL_FACE);
	int size=sqrt(GrassLocationCount);
	for(unsigned int x=-size/2;x<size/2;x++){
		for(unsigned int y=-size/2;y<size/2;y++)
			GrassLocations[x*size+y]=AEVec3FromCoords(x*0.01,0,y*0.01);
	}
	
	unsigned int texture=AETextureLoad("/Users/Oliver/Documents/Dev/Projects/Games/C4/Assets/Textures/Outdoors/Grass/GrassStanding2.png");
	printf("%s() %i\n",__func__,__LINE__);
	Patch=GrassPatchNew(GrassLocations,GrassLocationCount,texture);
	printf("%s() %i\n",__func__,__LINE__);
	GrassPatchUpdate(Patch,1);
	
	AEStart(perframe);
	return 1;
}
