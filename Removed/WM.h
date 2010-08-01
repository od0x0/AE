#pragma once
#include "HeaderBegin.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>

typedef struct{
	unsigned int w,h,r,g,b,a,stencil,depth,inFullscreen;
	void* data;
	size_t dataSize;
}AEWM;

typedef struct{
	int type;
}AEWMEvent;

typedef int (*AEWMEventFunc)(const AEWMEvent* event,void* data);

extern AEWMEventFunc AEWMEventFilter;

AEWM* AEWMOpen(char* title,int w,int h);
void AEWMReset(AEWM* wm);
void AEWMClose(AEWM* wm);

void AEWMPoll(AEWM* wm);
unsigned char* AEWMGetKeys(void);
int* AEWMMouse(int* newPosition);//Returns static array of 2 ints (x,y), sets mouse pos if arg is not null

double AEWMGetSeconds(void);
void AEWMResetSeconds(double to);

#include "HeaderEnd.h"