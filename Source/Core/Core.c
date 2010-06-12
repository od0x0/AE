#include "../Core.h"
#include "../SOIL/SOIL.h"
#include <math.h>
#include <string.h>

struct{
	unsigned int w,h,r,g,b,a,stencil,depth,inFullscreen;
	unsigned int blockKeyInput,textureLoadFlags;
	unsigned char* keys;
	unsigned char mouseButtons;
	float fov,near,far;
	AEVec2i mouse;
}AEInternal;

///////////////////////////////////////////////////			Texture Stuff
unsigned int AETextureLoad(const char* filename){
	//SOIL is EPIC, no denial.
	//Handles EVERYTHING, beautifully too
	GLuint texid = SOIL_load_OGL_texture
		(
			filename,
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			AEInternal.textureLoadFlags
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

void AEPollInput(void){
	SDL_PumpEvents();
	
	AEInternal.keys=SDL_GetKeyState(NULL);
	if((AEKey(SDLK_LMETA)||AEKey(SDLK_LSUPER))&&AEKey(SDLK_q)) exit(0);
		//SDLK_LSUPER for Windoze and SDLK_LMETA for OS X
	static unsigned char blankKeys[256];
	if(AEBlockKeyInput) AEInternal.keys=blankKeys;
	
	AEInternal.mouseButtons=SDL_GetMouseState(&AEInternal.mouse.x,&AEInternal.mouse.y);
}

int AEKey(int key){return AEInternal.keys[key];}
int AEMouseButton(char button){return (SDL_BUTTON(button)&AEInternal.mouseButtons);}

static int AEEventFilter(const SDL_Event* event){
	//So it closes when the user says close
	if(event->type==SDL_QUIT) exit(0);
	return 1;
} 

////////////////////////////////////////////////////			View Stuff

void AEInit(char* title,int w,int h){
	//atexit(AEQuit);
	int error;
	error = SDL_Init(SDL_INIT_EVERYTHING);
	if(error){
		puts("SDL failed to start");
		exit(1);
	}
	
	if(AEInternal.fov==0) AEInternal.fov=60;
	if(AEInternal.far==0) AEInternal.far=3000;
	if(AEInternal.near==0) AEInternal.near=1;
	if(AEInternal.r==0) AEInternal.r=8;
	if(AEInternal.g==0) AEInternal.g=8;
	if(AEInternal.b==0) AEInternal.b=8;
	if(AEInternal.a==0) AEInternal.a=8;
	if(AEInternal.depth==0) AEInternal.depth=8;
	
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, AEInternal.r);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, AEInternal.g);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, AEInternal.b);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,AEInternal.a );
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, AEInternal.stencil);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, AEInternal.depth);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	SDL_SetVideoMode(w, h, 0, AEInternal.inFullscreen?(SDL_OPENGL | SDL_FULLSCREEN):SDL_OPENGL);
	AEInternal.w=w;AEInternal.h=h;
	
	SDL_WM_SetCaption(title,NULL);
	
	glViewport(0,0,AEInternal.w,AEInternal.h);
	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective (AEInternal.fov, (float)AEInternal.w/(float)AEInternal.h, AEInternal.near, AEInternal.far);
	glMatrixMode(GL_MODELVIEW);
	
	glClearColor(0,0,0,1);
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable(GL_DEPTH_TEST);
	glEnable( GL_TEXTURE_2D );
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	AEInternal.textureLoadFlags=SOIL_FLAG_COMPRESS_TO_DXT|SOIL_FLAG_INVERT_Y|SOIL_FLAG_MIPMAPS;
	
	SDL_SetEventFilter(AEEventFilter);
}

static void AEDefaultPerframeFunc(float step){}

void AEStart(void (*perframe)(float)){
	//0 is a magical number, simply acts as the default
	if(perframe==NULL) perframe=AEDefaultPerframeFunc;

	
	float now=SDL_GetTicks()*0.001;
	float then=now;
	while(1){
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		
		AEPollInput();
		
		now=SDL_GetTicks()*0.001;
		(*perframe)((now-then));
		then=now;
		//Sounds....  Poetic
		
		SDL_GL_SwapBuffers();
	}
}

void AEQuit(void){
	SDL_Quit();
}