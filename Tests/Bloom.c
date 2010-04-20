#include "../AE/Core.h"
#include "../AE/UI.h"
#define true 1
#define false 0

unsigned int ScreenTexture;
AEVec2i ScreenTextureSize;

unsigned int BloomTexture;
AEVec2i BloomTextureSize;

void BloomInit(int w,int h){
	ScreenTextureSize.x=AEScreen.w;
	ScreenTextureSize.y=AEScreen.h;
	
	glGenTextures(1,&ScreenTexture);
	glBindTexture(GL_TEXTURE_2D,ScreenTexture);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,ScreenTextureSize.x,ScreenTextureSize.y,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
	
	BloomTextureSize.x=w;
	BloomTextureSize.y=h;
	
	glGenTextures(1,&BloomTexture);
	glBindTexture(GL_TEXTURE_2D,BloomTexture);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,BloomTextureSize.x,BloomTextureSize.y,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
}

void BloomPerframe(float step){
	AEObjectsSignal(AEObjectEventRender,NULL);
	
	glBindTexture(GL_TEXTURE_2D,ScreenTexture);
	glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0,ScreenTextureSize.x,ScreenTextureSize.y);
	
	
	glViewport(0,0,BloomTextureSize.x,BloomTextureSize.y);
	glClear(GL_COLOR_BUFFER_BIT);
	
	AEUIRenderMode(1);
	glBegin(GL_QUADS);
	glTexCoord2f(0,1);
	glVertex3i(0,BloomTextureSize.y,-1);
	glTexCoord2f(0,0);
	glVertex3i(0,0,-1);
	glTexCoord2f(1,0);
	glVertex3i(BloomTextureSize.x,0,-1);
	glTexCoord2f(1,1);
	glVertex3i(BloomTextureSize.x,BloomTextureSize.y,-1);
	glEnd();
	AEUIRenderMode(0);
	
	glBindTexture(GL_TEXTURE_2D,BloomTexture);
	glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0,BloomTextureSize.x,BloomTextureSize.y);
	
	glViewport(0,0,ScreenTextureSize.x,ScreenTextureSize.y);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	
	glColor4f(1,1,1,0.5);
	
	AEUIRenderMode(1);
	glBegin(GL_QUADS);
	glTexCoord2f(0,1);
	glVertex3i(0,ScreenTextureSize.y,-1);
	glTexCoord2f(0,0);
	glVertex3i(0,0,-1);
	glTexCoord2f(1,0);
	glVertex3i(ScreenTextureSize.x,0,-1);
	glTexCoord2f(1,1);
	glVertex3i(ScreenTextureSize.x,ScreenTextureSize.y,-1);
	glEnd();
	AEUIRenderMode(0);
	
	glBindTexture(GL_TEXTURE_2D,ScreenTexture);
	glColor4f(1,1,1,1);
	
	AEUIRenderMode(1);
	glBegin(GL_QUADS);
	glTexCoord2f(0,1);
	glVertex3i(0,ScreenTextureSize.y,-1);
	glTexCoord2f(0,0);
	glVertex3i(0,0,-1);
	glTexCoord2f(1,0);
	glVertex3i(ScreenTextureSize.x,0,-1);
	glTexCoord2f(1,1);
	glVertex3i(ScreenTextureSize.x,ScreenTextureSize.y,-1);
	glEnd();
	AEUIRenderMode(0);
}

int main(int argc,char** argv){
	AEInit("Window",512,512);
	BloomInit(64,64);
	
	AEObject* o=AEObjectNew(NULL,NULL);//NULL/0 is the default parameters for most AE functions
	o->model=AEVBOLoad("Data/TexturedCube.obj",true,false);//.obj path, should cache in VRAM, and should keep normals
	o->texture=AETextureLoad("Data/Texture.png");//Good ol' texture
	
	//Move it so we can see it
	o->z=-5;
	o->rotation.z=o->rotation.x=45;
	
	AEStart(BloomPerframe);//Never returns
	return 1;//We should never get here
}