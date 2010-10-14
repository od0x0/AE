#include "SDL.h"
#include "Node.h"
#include "Math3D.h"
#include <float.h>

void Control(float step){
	float speed=step*5;
	float turnspeed=step*10;
	AEVec3 move={0,0,0};
	AEVec3 turn={0,0,0};
	AECameraRotationGet(AECameraActiveGet(),&turn);
	
	if(SDL_GetKeyState(NULL)['w']) move.z-=speed;
	if(SDL_GetKeyState(NULL)['s']) move.z+=speed;
	if(SDL_GetKeyState(NULL)['d']) move.x+=turnspeed;
	if(SDL_GetKeyState(NULL)['a']) move.x-=turnspeed;
	
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		switch (event.type) {
			case SDL_MOUSEMOTION:{
				turn.y-=0.5*event.motion.xrel;
				turn.x-=0.5*event.motion.yrel;
				if(turn.x > 89) turn.x=89;
				if(turn.x < -89) turn.x=-89;
				//turn.x=0;
				}break;
		}
	}

	
	AECameraRotationSet(AECameraActiveGet(),turn.x,turn.y,turn.z);
	AEVec3 pos={0,0,0};
	AECameraPositionGet(AECameraActiveGet(),&pos);
	pos=AEVec3Add(AEQuatMulVec3(AEQuatFromEuler(turn),move),pos);
	AECameraPositionSet(AECameraActiveGet(),pos.x,pos.y,pos.z);
}

void perframe(float step){
	Control(step);
	AENodesStep(step);
	
	AENodesRender();
}
extern unsigned int AEShadersEnabled;

int main(int argc,char** argv){
	AECameraBoundsSet(AECameraActiveGet(),0,0,60);
	AEInit("Window",800,500);
	AENodesInit(NULL,NULL,NULL,NULL);
	glClearColor(0,0.8,1,1);
	
	AECameraPositionSet(AECameraActiveGet(),0,4,14);
	
	AENode* ent=AENodeNew(AENodeTypeEnt,"An Entity Name");
	AENode* ent2=AENodeNew(AENodeTypeEnt,"An Entity2 Name");
	AENode* mesh=AENodeNew(AENodeTypeMesh,"A Mesh Name");
	AENode* material=AENodeNew(AENodeTypeMaterial,"A Material Name");
	
	AENodeEntPositionSet(ent,0,6,0);
	AENodeEntPositionSet(ent2,4,4,4);
	AENodeEntRotationSet(ent,90,0,0);
	AENodeEntRotationSet(ent2,0,90,0);
	AEShadersEnabled=false;
	
	AENodeMaterialTextureSet(material,"Diffuse",AEImageFromFile("Data/Texture.png"));
	/*char* vertexshader=
	"uniform float time;"
	"void main(){"
	"	gl_FrontColor=gl_Color*vec4(vec3(sin(time)),1);"
	"	gl_Position=ftransform();"
	"	gl_TexCoord[0]=gl_MultiTexCoord0;"
	"}";
	AENodeMaterialShaderSet(material,vertexshader,NULL);
	AENodeMaterialShaderSet(material,NULL,NULL);*/
	AENodeMeshVBOSet(mesh,AEVBOLoad("Data/TexturedCube.obj","tv","static"));
	
	AENodeEntAddChild(ent,mesh);
	AENodeEntAddChild(ent,material);
	AENodeEntAddChild(ent2,mesh);
	AENodeEntAddChild(ent2,material);
	AENodeEntAddChild(ent,ent2);
	
	AEStart(perframe);
	return 1;
}
