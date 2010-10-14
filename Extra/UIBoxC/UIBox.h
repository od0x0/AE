#pragma once
#include "Image.h"
#include "List.h"

typedef struct AEUIBox{
	int x,y,w,h;
	void* (*event)(struct AEUIBox* self,int event,void* data);
	AEList* children;//AEList<AEUIBox*>
}AEUIBox;

#define AEUIBoxEventInit 1
#define AEUIBoxEventAddChild 2
#define AEUIBoxEventRender 3
#define AEUIBoxEventSDLEvent 4
#define AEUIBoxEventRelease 5
#define AEUIBoxEventMove 6
#define AEUIBoxEventValue 7
#define AEUIBoxEventClicked 8

extern AEUIBox* AEUIBoxActive;
extern AEUIBox* AEUIBoxRoot;

void AEUIDrawBox(int x,int y,int w,int h,char filled);
int AEUIBoxTouchesRect(AEUIBox* box,int x,int y,int w,int h);

void* AEUIBoxSignal(AEUIBox* box,int event,void* data);
void AEUIBoxSignalChildren(AEUIBox* root,int event,void* data);
void AEUIBoxRaise(AEUIBox* root,AEUIBox* box);
void* AEUIBoxDefaultEvent(AEUIBox* self,int event,void* data);
AEUIBox* AEUIBoxAddChild(AEUIBox* root,
	void* (*event)(AEUIBox*,int,void*),
	void* data,int x,int y,int w,int h);