#include "Image.h"

typedef struct AEUIBox{
	int x,y,w,h;
	AEImage* image;
	void* (*event)(struct AEUIBox* self,int event,void* data);
	unsigned int subBoxCount;
	struct AEUIBox** subBoxes;
}AEUIBox;

#define AEUIBoxEventInit 1
#define AEUIBoxEventAddChild 2
#define AEUIBoxEventRender 3
#define AEUIBoxEventSDLEvent 4
#define AEUIBoxEventRelease 5
#define AEUIBoxEventMove 6
#define AEUIBoxEventValue 7

extern AEUIBox* AEUIBoxActive;
extern AEUIBox* AEUIBoxRoot;

void AEUIBoxSignalChildren(AEUIBox* root,int event,void* data);
void AEUIBoxRaise(AEUIBox* root,AEUIBox* box);
void* AEUIBoxDefaultEvent(AEUIBox* self,int event,void* data);
int AEUIBoxTouchesRect(AEUIBox* box,int x,int y,int w,int h);
AEUIBox* AEUIBoxAddChild(AEUIBox* root,
	void* (*event)(AEUIBox*,int,void*),
	void* data,int x,int y,int w,int h);