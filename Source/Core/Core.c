#include "../Core.h"
#include "../SOIL/SOIL.h"
#include <math.h>
#include <string.h>
#include "../Camera.h"

static AEWM* AEContext=NULL;

AEWM* (*AEWMNew)(char* title, AEState* state)=NULL;
AEState* (*AEWMStateGet)(AEWM* wm)=NULL;
void (*AEWMStateSet)(AEWM* wm,AEState* state)=NULL;
int (*AEWMPollInput)(AEWM* wm)=NULL;
void (*AEWMSwapBuffers)(AEWM* wm)=NULL;
void (*AEWMDelete)(AEWM* wm)=NULL;
double (*AEWMSecondsGet)(AEWM* wm)=NULL;

AEState AEActiveState={
	.w=800,.h=500,
	.r=8,
	.g=8,
	.b=8,
	.a=8,
	.stencil=8,
	.depth=8,
	.inFullscreen=0,
	.blockKeyInput=0,
	.textureLoadFlags=SOIL_FLAG_COMPRESS_TO_DXT|SOIL_FLAG_INVERT_Y|SOIL_FLAG_MIPMAPS,
/*	.keys=NULL,
	.mouseButtons=0,
	*/
	.fov=60,
	.near=1,
	.far=3000,
	.mouse={400,250}
};

///////////////////////////////////////////////////			Texture Stuff
unsigned int AETextureLoad(const char* filename){
	//SOIL is EPIC, no denial.
	//Handles EVERYTHING, beautifully too
	GLuint texid = SOIL_load_OGL_texture
		(
			filename,
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			AEActiveState.textureLoadFlags
		);
		if(texid==0) printf("AETextureLoad: Texture loading of %s failed because %s\n",filename,SOIL_last_result());
	AETextureBind(texid);
	return texid;
}

/*void AETextureBind(unsigned int texture){	
	//From what I heard on idevgames.com/forum , OpenGL doesn't take this optimization
	//Just checks to see if the current texture is the same as the last, doesn't bother binding it if it is
	static GLuint current=0;
	//if(current!=texture) 
	glBindTexture(GL_TEXTURE_2D,current=texture);
}*/

void AETextureDelete(unsigned int texture){
	glDeleteTextures(1,(GLuint*)&texture);
}
/////////////////////////////////////////////////////			Utility
//C has no template functions, so we have to do this ugly thing

unsigned int AELinearSearch_internal(void* value,void* array,int length,int size){
	if(array==NULL || value==NULL) return 0;
	for(unsigned int i=0;i<length;i++)
		if(memcmp(((char*)array+(i*size)),value,size)==0) return i+1;
	
	return 0;
}

#define AELinearSearch(val,array,len) AELinearSearch_internal(val,array,len,sizeof(*val))
////////////////////////////////////////////////////			SDL Interaction

/*void AEPollInput(void){
	
	
	SDL_PumpEvents();
	
	AEActiveState.keys=SDL_GetKeyState(NULL);
	if((AEKey(SDLK_LMETA)||AEKey(SDLK_LSUPER))&&AEKey(SDLK_q)) exit(0);
		//SDLK_LSUPER for Windoze and SDLK_LMETA for OS X
	static unsigned char blankKeys[256];
	if(AEActiveState.blockKeyInput) AEActiveState.keys=blankKeys;
	
	AEActiveState.mouseButtons=SDL_GetMouseState(&AEActiveState.mouse.x,&AEActiveState.mouse.y);
	
}*/
/*
int AEKey(int key){return AEActiveState.keys[key];}
int AEMouseButton(char button){return (SDL_BUTTON(button)&AEActiveState.mouseButtons);}
*/
/*
static int AEEventFilter(const SDL_Event* event){
	//So it closes when the user says close
	if(event->type==SDL_QUIT) exit(0);
	return 1;
} */

////////////////////////////////////////////////////			View Stuff

void AEInit(char* title,int w,int h){
	if(AEWMNew==NULL || AEWMStateGet==NULL || AEWMStateSet==NULL || AEWMPollInput==NULL || AEWMSwapBuffers==NULL || AEWMDelete==NULL || AEWMSecondsGet==NULL) AEError("AEWM function pointers need to all be filled before you can use the engine.");
	//atexit(AEQuit);
	/*int error;
	error = SDL_Init(SDL_INIT_EVERYTHING);
	if(error){
		puts("SDL failed to start");
		exit(1);
	}*/
	
	if(AEActiveState.fov==0) AEActiveState.fov=60;
	if(AEActiveState.far==0) AEActiveState.far=3000;
	if(AEActiveState.near==0) AEActiveState.near=1;
	if(AEActiveState.r==0) AEActiveState.r=8;
	if(AEActiveState.g==0) AEActiveState.g=8;
	if(AEActiveState.b==0) AEActiveState.b=8;
	if(AEActiveState.a==0) AEActiveState.a=8;
	if(AEActiveState.depth==0) AEActiveState.depth=8;
	
	/*SDL_GL_SetAttribute(SDL_GL_RED_SIZE, AEActiveState.r);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, AEActiveState.g);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, AEActiveState.b);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,AEActiveState.a );
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, AEActiveState.stencil);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, AEActiveState.depth);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);*/
	
	if(w==0) w=AEActiveState.w;
	if(h==0) h=AEActiveState.h;
	
	if(AEContext) AEWMDelete(AEContext);
	AEContext=NULL;
	AEContext=AEWMNew(title,&AEActiveState);
	//SDL_SetVideoMode(w, h, 0, AEActiveState.inFullscreen?(SDL_OPENGL | SDL_FULLSCREEN):SDL_OPENGL);
	AEActiveState.w=w;AEActiveState.h=h;
	
	//SDL_WM_SetCaption(title,NULL);
	
	glViewport(0,0,AEActiveState.w,AEActiveState.h);
	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective (AEActiveState.fov, (float)AEActiveState.w/(float)AEActiveState.h, AEActiveState.near, AEActiveState.far);
	glMatrixMode(GL_MODELVIEW);
	
	glClearColor(0,0,0,1);
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable(GL_DEPTH_TEST);
	glEnable( GL_TEXTURE_2D );
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	//AEActiveState.textureLoadFlags=SOIL_FLAG_COMPRESS_TO_DXT|SOIL_FLAG_INVERT_Y|SOIL_FLAG_MIPMAPS;
	
	//SDL_SetEventFilter(AEEventFilter);
}

static void AEDefaultPerframeFunc(float step){}

void AEStart(void (*perframe)(float)){
	//0 is a magical number, simply acts as the default
	if(perframe==NULL) perframe=AEDefaultPerframeFunc;

	
	float now=AEWMSecondsGet(AEContext);
	float then=now;
	while(AEWMPollInput(AEContext)){
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		
		AECamera* cam=AECameraActiveGet();
		
		glLoadIdentity();
		glRotatef(-cam->rotation.x,	1,0,0);
		glRotatef(-cam->rotation.y,	0,1,0);
		glRotatef(-cam->rotation.z,	0,0,1);
		glTranslatef(-cam->position.x,-cam->position.y,-cam->position.z);
	
		//AECameraVFCalculate(cam);
		
		now=AEWMSecondsGet(AEContext);
		(*perframe)((now-then));
		then=now;
		//Sounds....  Poetic
		
		AEWMSwapBuffers(AEContext);
	}
}

void AEQuit(void){
	AEWMDelete(AEContext);
}