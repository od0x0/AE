#include "../AE/Core.h"
#include "../AE/Camera.h"
#include "../AE/VBO.h"
#include "../AE/Node.h"
#include "../AE/Physics.h"

void AENodeEntAddChild(AENode* node,AENode* node2);

AENode* Cube=NULL;

AEPBody* CubeBody=NULL;

AENode* Root=NULL;

AEPEnv* Env=NULL;

int OnGround=1;

int CollideCallback(AEPBody* b1,AEPBody* b2){
	//puts("Collided");
	OnGround=1;
	return 1;
}

#define MAXIMUM_FRAME_RATE 120
#define MINIMUM_FRAME_RATE 15
#define UPDATE_INTERVAL (1.0 / MAXIMUM_FRAME_RATE)
#define MAX_CYCLES_PER_FRAME (MAXIMUM_FRAME_RATE / MINIMUM_FRAME_RATE)

int FixedUpdate(void* data){
	int done = 0;
	int fps = 30;
	int delay = 1000/fps;
	int thenTicks = -1;
	int nowTicks=0;
	
	float step=0;
	
	while(1){
	
		/*if(OnGround){//&& AEKey(' ')
			AEPBodyVelocitySet(AENodeEntBodyGet(Cube),0,1,0);
			OnGround=0;
		}*/
		
		if(step) AENodeStep(Cube,step);
		
		step=nowTicks*0.001 - thenTicks*0.001;
		
		// Time how long each draw-swap-delay cycle takes
		// and adjust delay to get closer to target framerate
		if (thenTicks > 0) {
			nowTicks = SDL_GetTicks ();
			delay += (1000/fps - (nowTicks-thenTicks));
			thenTicks = nowTicks;
			if (delay < 0)	delay = 1000/fps;
		}
		else {
			thenTicks = SDL_GetTicks ();
		}

		SDL_Delay (delay);
	}
	
	return 0;
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
		AENodeStep(Root,UPDATE_INTERVAL*0.01);
		/*
		AEPEnvCheck(Env);
		AEPStep(UPDATE_INTERVAL);
		*/
		AEVec3 p={0,0,0};
		AEPBodyPositionGet(CubeBody,&p);
		AENodeEntPositionSet(Cube,p.x,p.y,p.z);
	}
	
	cyclesLeftOver = updateIterations;
	lastFrameTime = currentTime;
	
	AENodeRender(Cube);
	
	if(OnGround && AEKey(' ')){
		//AEPBodyVelocitySet(CubeBody,0,10,0);
		AENodeEntVelocitySet(Cube,0,10,0);
		//printf("%p | %p\n",AENodeEntBodyGet(Cube),CubeBody);
		
		//exit(0);
		OnGround=0;
	}

/*
	static int done = 0;
	const static int fps = 30;
	static int delay = 1000/fps;
	static int thenTicks = -1;
	static int nowTicks=0;
	
	if(OnGround && AEKey(' ')){
		AEPBodyVelocitySet(AENodeEntBodyGet(Cube),0,1,0);
		OnGround=0;
	}
	
	if(step) AENodeStep(Cube,delay);
	
	//step=nowTicks*0.001 - thenTicks*0.001;
	
	// Time how long each draw-swap-delay cycle takes
	// and adjust delay to get closer to target framerate
	if (thenTicks > 0) {
		nowTicks = SDL_GetTicks ();
		delay += (1000/fps - (nowTicks-thenTicks));
		thenTicks = nowTicks;
		if (delay < 0)	delay = 1000/fps;
	}
	else {
		thenTicks = SDL_GetTicks ();
	}
	SDL_Delay (delay);*/
}

int main(int argc,char** argv){
	AEInit("Window",800,500);
	AEPInit();
	AECamera* cam=AECameraActiveGet();
	cam->position.z=10;
	
	AEPGravitySet(0,-9.1,0);
	
	AEVBO* vbo=AEVBOLoad("Data/TexturedCube.obj","tnvi","stream");
	
	AEVec3 min={0,0,0};
	AEVec3 max={0,0,0};
	
	AEVBOCalculateAABB(vbo,&min,&max);
	
	//AEPEnv* 
	Env=AEPEnvNewSimple(NULL);
	AEVec3 size=AEVec3Sub(max,min);//{2,2,2};
	
	//Ignored
	AEPBody* body=AEPBodyNew(NULL,AEPGeomTypeBox,&size);
	AEPBodyMassSet(body,size,0);
	AEPBodyPositionSet(body,0,0,0);
	AEPBodyFlagSet(body,AEPBodyFlagMovable,0);
	AEPEnvAddBody(Env,body);
	
	//Added to Cube
	body=AEPBodyNew(NULL,AEPGeomTypeBox,&size);
	AEPBodyCollisionCallbackSet(body,CollideCallback);
	AEPBodyMassSet(body,size,1);
	//
	//AEPBodyPositionSet(body,0,5,0);
	AEPBodyFlagSet(body,AEPBodyFlagBounciness,0.4);
	AEPBodyFlagSet(body,AEPBodyFlagBounceVelocity,0.2);
	//AEPEnvAddBody(Env,body);
	
	CubeBody=body;
	
	//Texture=AETextureLoad("Data/Texture.png");
	
	Cube=AENodeNew(AENodeTypeEnt,"Cube");
	
	AENodeEntBodySet(Cube,CubeBody);
	
	AENodeEntPositionSet(Cube,0,5,0);
	
	//AEPBodyVelocitySet(AENodeEntBodyGet(Cube),0,1,0);
	
	Root=AENodeNew(AENodeTypeEnt,"Root");
	
	AENodeEntBodySet(Root,(AEPBody*)Env);
	
	AENodeEntAddChild(Root,Cube);
	
	AENode* node=NULL;
	
	node=AENodeNew(AENodeTypeMesh,"CubeMesh");
	AENodeMeshVBOSet(node,vbo);
	AENodeEntAddChild(Cube,node);
	
	node=AENodeNew(AENodeTypeMaterial,"CubeMaterial");
	AEImage* image=AEImageFromFile("Data/Texture.png");
	AENodeMaterialTextureSet(node,NULL,image);
	AEImageDelete(image);
	AENodeEntAddChild(Cube,node);
	
	//SDL_CreateThread(FixedUpdate, NULL);
	
	AEStart(perframe);
	return 1;
}
