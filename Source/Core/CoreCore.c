#ifdef __cplusplus
extern "C"{
#endif

#include "../Core.h"
#include "../SOIL/SOIL.h"
#include <math.h>
#include <string.h>

AECameraType AECamera;

AEScreenType AEScreen;

AEVec2i AEMouse={0,0};

int AEBlockKeyInput=0;

unsigned int AETextureLoadFlags=SOIL_FLAG_COMPRESS_TO_DXT|SOIL_FLAG_INVERT_Y|SOIL_FLAG_MIPMAPS;

///////////////////////////////////////////////////			Texture Stuff
unsigned int AETextureLoad(const char* filename){
	//SOIL is EPIC, no denial.
	//Handles EVERYTHING, beautifully too
	GLuint texid = SOIL_load_OGL_texture
		(
			filename,
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			AETextureLoadFlags
		);
		if(texid==0) printf("AETextureLoad: Texture loading of %s failed because %s\n",filename,SOIL_last_result());
	AETextureBind(texid);
	return texid;
}

void AETextureBind(unsigned int texture){	
	//From what I heard on idevgames.com/forum , OpenGL doesn't take this optimization
	//Just checks to see if the current texture is the same as the last, doesn't bother binding it if it is
	static GLuint current=0;
	//if(current!=texture) 
	glBindTexture(GL_TEXTURE_2D,current=texture);
}

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

void AEMoveXZ(void* object,float x,float z){
	AECameraType* o=(AECameraType*)object;//As long as we don't use the fov/near/far, we should be fine
	const float piover180=M_PI/180;
	o->x += sinf(o->rotation.y*piover180) * z;
	o->z += cosf(o->rotation.y*piover180) * z;
	o->x += sinf((o->rotation.y+90)*piover180) * x;
	o->z += cosf((o->rotation.y+90)*piover180) * x;
}

AEVec3 AEAngleTo(float x,float y,float z){
	const float piunder180=180/M_PI;
	AEVec3 angle= {atanf(y/z)*piunder180,atanf(z/x)*piunder180,atanf(y/x)*piunder180};
	return angle;
}
////////////////////////////////////////////////////			SDL Interaction

static unsigned char* AEKeys=NULL;
static unsigned char AEMouseButtons=0;

void AEPollInput(void){
	SDL_PumpEvents();
	
	AEKeys=SDL_GetKeyState(NULL);
	if((AEKey(SDLK_LMETA)||AEKey(SDLK_LSUPER))&&AEKey(SDLK_q)) exit(0);
		//SDLK_LSUPER for Windoze and SDLK_LMETA for OS X
	static unsigned char blankKeys[256];
	if(AEBlockKeyInput) AEKeys=blankKeys;
	
	AEMouseButtons=SDL_GetMouseState(&AEMouse.x,&AEMouse.y);
}

int AEKey(int key){return AEKeys[key];}
int AEMouseButton(char button){return (SDL_BUTTON(button)&AEMouseButtons);}

static int AEEventFilter(const SDL_Event* event){
	//So it closes when the user says close
	if(event->type==SDL_QUIT) exit(0);
	return 1;
} 

////////////////////////////////////////////////////			View Stuff

void AERefreshViewport(char in2d){
	glViewport(0,0,AEScreen.w,AEScreen.h);
	glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if(in2d) glOrtho(0,AEScreen.w,0,AEScreen.h,AECamera.near,AECamera.far);
		else gluPerspective (AECamera.fov, (float)AEScreen.w/(float)AEScreen.h, AECamera.near, AECamera.far);
	glMatrixMode(GL_MODELVIEW);
}

void AEInit(char* title,int w,int h){
	//atexit(AEQuit);
	int error;
	error = SDL_Init(SDL_INIT_EVERYTHING);
	if(error){
		puts("SDL failed to start");
		exit(1);
	}
	
	if(AECamera.fov==0) AECamera.fov=60;
	if(AECamera.far==0) AECamera.far=3000;
	if(AECamera.near==0) AECamera.near=1;
	if(AEScreen.r==0) AEScreen.r=8;
	if(AEScreen.g==0) AEScreen.g=8;
	if(AEScreen.b==0) AEScreen.b=8;
	if(AEScreen.a==0) AEScreen.a=8;
	if(AEScreen.depth==0) AEScreen.depth=8;
	
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, AEScreen.r);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, AEScreen.g);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, AEScreen.b);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,AEScreen.a );
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, AEScreen.stencil);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, AEScreen.depth);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	
	SDL_SetVideoMode(w, h, 0, AEScreen.inFullscreen?(SDL_OPENGL | SDL_FULLSCREEN):SDL_OPENGL);
	AEScreen.w=w;AEScreen.h=h;
	
	SDL_WM_SetCaption(title,NULL);
	
	AERefreshViewport(0);
	
	glClearColor(0,0,0,1);
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable(GL_DEPTH_TEST);
	glEnable( GL_TEXTURE_2D );
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	SDL_SetEventFilter(AEEventFilter);
	
	/*glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);*/
}

static void AEDefaultPerframeFunc(float step){
	AEObjectsSignal(AEObjectEventRender,NULL);
}

void AEStart(void (*perframe)(float)){
	//0 is a magical number, simply acts as the default
	if(perframe==NULL) perframe=AEDefaultPerframeFunc;

	
	float now=SDL_GetTicks()*0.001;
	float then=now;
	while(1){
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		
		glPushMatrix();
		glRotatef(-AECamera.rotation.x,	1,0,0);
		glRotatef(-AECamera.rotation.y,	0,1,0);
		glRotatef(-AECamera.rotation.z,	0,0,1);
		glTranslatef(-AECamera.x,-AECamera.y,-AECamera.z);
		
		AEPollInput();
		
		now=SDL_GetTicks()*0.001;
		(*perframe)((now-then));
		then=now;
		//Sounds....  Poetic
		
		glPopMatrix();
		SDL_GL_SwapBuffers();
	}
}

void AEQuit(void){
	SDL_Quit();
}

#ifdef __cplusplus
} //extern "C"
#endif