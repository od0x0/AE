/*
Writen by Oliver Daids, 2009.

Ambition Engine Core.
*/

#pragma once
#include "HeaderBegin.h"
//Might not always use SDL, but just to avoid extra work

//Most portable option, just add a header search path
#ifdef __APPLE_CC__
#include "OpenGL/gl.h"
#include "OpenGL/glu.h"
#else
#include "gl.h"
#include "glu.h"
#endif

#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "string.h"


/*#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>*/

typedef struct{float x,y;}AEVec2;
typedef struct{float x,y,z;}AEVec3;
typedef struct{float x,y,z,w;}AEVec4;
typedef struct{int x,y;}AEVec2i;
typedef struct{int x,y,z;}AEVec3i;
typedef struct{int x,y,z,w;}AEVec4i;

typedef struct{float r,g,b;}AERGB;
typedef struct{float r,g,b,a;}AERGBA;

typedef struct AEState{
	unsigned int w,h,r,g,b,a,stencil,depth,inFullscreen;
	unsigned int blockKeyInput,textureLoadFlags;
	/*unsigned char* keys;
	unsigned int mouseButtons;//Packed by bit (in other words, each button takes up a bit)*/
	float fov,near,far;
	AEVec2i mouse;
}AEState;

extern AEState AEActiveState;

typedef struct{
	size_t size;//May have hidden data, and so this is the size of the whole thing (if size=0, then size=sizeof(AEWM))
	AEState state;
}AEWM;

extern AEWM* (*AEWMNew)(char* title, AEState* state);
extern AEState* (*AEWMStateGet)(AEWM* wm);
extern void (*AEWMStateSet)(AEWM* wm,AEState* state);
extern int (*AEWMPollInput)(AEWM* wm);
extern void (*AEWMSwapBuffers)(AEWM* wm);
extern void (*AEWMDelete)(AEWM* wm);
extern double (*AEWMSecondsGet)(AEWM* wm);

static inline void AEStateActiveSet(AEState* state){AEActiveState=*state;}
static inline AEState* AEStateActiveGet(void){return &AEActiveState;}

void AEInit(char* title,int w,int h);
void AEStart(void (*func)(float));
void AEQuit(void);

unsigned int AETextureLoad(const char* filename);
static inline void AETextureBind(unsigned int texture){glBindTexture(GL_TEXTURE_2D,(GLuint)texture);}
void AETextureDelete(unsigned int texture);

unsigned int AELinearSearch_internal(void* value,void* array,int length,int size);
#define AELinearSearch(val,array,len) AELinearSearch_internal(val,array,len,sizeof(*val))

static inline void AEError_internal(char* message,const char* function){
	printf("ERROR in %s():\n\t%s\n",function,message);
	abort();
}
#define AEError(msg) AEError_internal(msg,__func__)

#include "HeaderEnd.h"