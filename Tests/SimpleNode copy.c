#include "SDL.h"
#include "Node.h"
#include "Math3D.h"
#include <float.h>

AETexture WaterReflectionTexture=0;
AETexture WaterDepthTexture=0;
int WaterDepthTextureSize=512,WaterReflectionTextureSize=512;

void WaterPrepareBegin(void){
	/*glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	glViewport(0,0,WaterReflectionTextureSize,WaterReflectionTextureSize);
	glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluPerspective (60, 1, 1, 1000);
	glMatrixMode(GL_MODELVIEW);*/
	
	double plane[4] = {0.0, -1.0, 0.0, 0.0}; 
	glEnable(GL_CLIP_PLANE0);
	glClipPlane(GL_CLIP_PLANE0, plane);
	glFrontFace(GL_CW);
	glPushMatrix();
		glScalef(1,-1,1);
}

void WaterPrepareEnd(void){
	glPopMatrix();
	glFrontFace(GL_CCW);
	glDisable(GL_CLIP_PLANE0);
	
	/*glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	
	glViewport(0,0,AEContextActiveGet()->w,AEContextActiveGet()->h);
	
	
	if(WaterDepthTexture==0){
		glBindTexture(GL_TEXTURE_2D, WaterReflectionTexture);
		glGenTextures(1,&WaterReflectionTexture);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,WaterReflectionTextureSize,WaterReflectionTextureSize,GL_RGB,0,GL_UNSIGNED_BYTE,NULL);
		//glTexImage2D
	}
	glBindTexture(GL_TEXTURE_2D, WaterReflectionTexture);
   	//glCopyTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT, 0,0, WaterDepthTextureSize,WaterDepthTextureSize, 0);
	glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0,WaterReflectionTextureSize, WaterReflectionTextureSize);
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);*/
}

void WaterDraw(void){
	glUseProgram(0);
	AETextureBind(WaterDepthTexture);
	
	glColor4f(1,1,1,0.4);
	//glDisable(GL_CULL_FACE);
	//glDisable(GL_DEPTH_TEST);
	
	glPushMatrix();
		static float translationNullingMatrix[16]={1,1,1,1, 1,1,1,1, 1,1,1,1, 0,0,0,1};
		//glMultMatrixf(translationNullingMatrix);
		//glLoadIdentity();
		
		AEVec3 rot={0,0,0};
		AECameraRotationGet(AECameraActiveGet(),&rot);
		glRotatef(rot.y,0,1,0);
		/*AEVec3 pos={0,0,0};
		AECameraPositionGet(AECameraActiveGet(),&pos);
		glTranslatef(-pos.x,-pos.y,-pos.z);*/
		
		glColor4f(0.0,0.2,0.9,0.8);
		glBegin(GL_QUADS);
		float infinity=1000;//FLT_MAX/2;//20000;//1.0/0.0;
		//glColor4f(0,0.8,1,0.9);
		glTexCoord2f(1,1);
		glVertex3f(infinity,0,-infinity);
		glTexCoord2f(0,1);
		glVertex3f(-infinity,0,-infinity);
		//glColor4f(0,0.8,1,0.4);
		glTexCoord2f(0,0);
		glVertex3f(-infinity,0,infinity);
		glTexCoord2f(1,0);
		glVertex3f(infinity,0,infinity);
		glEnd();
	glPopMatrix();
	
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glColor4f(1,1,1,1);

}

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
	
	glColor3f(0.3,0.3,0.3);
	WaterPrepareBegin();
	AENodesRender();
	WaterPrepareEnd();
	glColor3f(1,1,1);
	
	WaterDraw();
	
	/*glEnable(GL_FOG);
	glFogi (GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_START, 0.0f);
	glFogf(GL_FOG_END, 0.0f);
	float fogColor[4]={0.1,0.1,0.1,1};
	glFogfv (GL_FOG_COLOR, fogColor);
	glFogf (GL_FOG_DENSITY, 1);
	glHint (GL_FOG_HINT, GL_NICEST);*/
	AENodesRender();
	//glDisable(GL_FOG);
}
extern unsigned int AEShadersEnabled;

int main(int argc,char** argv){
	AECameraBoundsSet(AECameraActiveGet(),0,0,60);
	AEInit("Window",800,500);
	AENodesInit(NULL,NULL,NULL,NULL);
	glClearColor(0,0.8,1,1);
	//SphereVBO=AEVBOLoad("Data/GeoDome.obj","tvi","static");
	
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
	//AENodeMaterialShaderSet(material,"void main(){gl_FrontColor=gl_Color;gl_Position=ftransform();gl_TexCoord[0] =gl_MultiTexCoord0;}",NULL);
	//AENodeMaterialShaderSet(material,NULL,NULL);
	AENodeMeshVBOSet(mesh,AEVBOLoad("Data/TexturedCube.obj","tv","static"));
	
	AENodeEntAddChild(ent,mesh);
	AENodeEntAddChild(ent,material);
	AENodeEntAddChild(ent2,mesh);
	AENodeEntAddChild(ent2,material);
	AENodeEntAddChild(ent,ent2);
	
	AEStart(perframe);
	return 1;
}
