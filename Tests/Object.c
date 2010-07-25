#include "../AE/Core.h"
#include "../AE/Camera.h"
#include "../AE/VBO.h"
#include "../AE/Physics.h"

unsigned int Texture=0;
AEVBO* VBO=NULL;
AEPBody* Body=NULL;
AEPBody* Body2=NULL;
AEPEnv* Env=NULL;

int OnGround=1;

#define MAXIMUM_FRAME_RATE 120
#define MINIMUM_FRAME_RATE 15
#define UPDATE_INTERVAL (1.0 / MAXIMUM_FRAME_RATE)
#define MAX_CYCLES_PER_FRAME (MAXIMUM_FRAME_RATE / MINIMUM_FRAME_RATE)

int CollideCallback(AEPBody* b1,AEPBody* b2){
	//puts("Collided");
	OnGround=1;
	return 1;
}

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
		AEPEnvCheck(Env);
		AEPStep(UPDATE_INTERVAL);
	}
	
	if(OnGround && AEKey(' ')){
		AEPBodyVelocitySet(Body2,0,1,0);
		OnGround=0;
	}
	
	cyclesLeftOver = updateIterations;
	lastFrameTime = currentTime;

	static float delta=0;
	delta+=10*step;
	glPushMatrix();{
		AEVec3 p;
		AEPBodyPositionGet(Body2,&p);
		//if(p.y!=0) exit(0);
		glTranslatef(p.x,p.y,p.z);
		//glRotatef(delta,1,0,0);
		//glRotatef(delta,0,1,0);
		AETextureBind(Texture);
		AEVBOBind(VBO);
		AEVBODraw(VBO);
	}glPopMatrix();
	glPushMatrix();{
		AEVec3 p={0,0,0};
		AEPBodyPositionGet(Body,&p);
		glTranslatef(p.x,p.y,p.z);
		//glRotatef(delta,1,0,0);
		//glRotatef(delta,0,1,0);
		AETextureBind(Texture);
		AEVBOBind(VBO);
		AEVBODraw(VBO);
	}glPopMatrix();
}

int main(int argc,char** argv){
	AEInit("Window",800,500);
	AEPInit();
	AECamera* cam=AECameraActiveGet();
	cam->position.z=10;
	
	//AEPGravitySet(0,-9.8,0);
	AEPGravitySet(0,-0.1,0);
	
	Texture=AETextureLoad("Data/Texture.png");
	VBO=AEVBOLoad("Data/TexturedCube.obj","tnvi","stream");
	
	AEVec3 min={0,0,0};
	AEVec3 max={0,0,0};
	
	AEVBOCalculateAABB(VBO,&min,&max);
	
	Env=AEPEnvNew(NULL,AEPEnvTypeSimple,NULL);
	AEVec3 size=AEVec3Sub(max,min);//{2,2,2};
	
	Body=AEPBodyNew(NULL,AEPGeomTypeBox,&size);
	AEPBodyMassSet(Body,size,0);
	AEPBodyPositionSet(Body,0,0,0);
	AEPBodyFlagSet(Body,AEPBodyFlagMovable,0);
	AEPEnvAddBody(Env,Body);
	
	Body2=AEPBodyNew(NULL,AEPGeomTypeBox,&size);
	AEPBodyCollisionCallbackSet(Body2,CollideCallback);
	AEPBodyMassSet(Body2,size,1);
	AEPBodyPositionSet(Body2,0,3,0);
	AEPBodyFlagSet(Body2,AEPBodyFlagBounciness,0.4);
	AEPBodyFlagSet(Body2,AEPBodyFlagBounceVelocity,0.2);
	AEPEnvAddBody(Env,Body2);	

	void* ptr=AEPBodyNew(NULL,AEPGeomTypeBox,&size);
	AEPBodyDelete(ptr);
	
	AEStart(perframe);
	return 1;
}
