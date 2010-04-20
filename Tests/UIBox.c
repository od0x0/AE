#include "../AE/Core.h"
#include "../AE/UIBox.h"
#include "../AE/Image.h"

AEUIBox* Box=NULL;
AEImage* Image=NULL;

void* ImageBoxEvent(AEUIBox* self,int event,void* data){
	switch(event){
	
		case AEUIBoxEventInit:
			Image=AEImageFromFile("Data/Texture.png");
			return self;
			break;
		case AEUIBoxEventRender:
			AEImageBlit(NULL,self->x,self->y,Image);
			AEUIBoxSignalChildren(self,event,data);
			break;
		
		case AEUIBoxEventRelease:
			AEUIBoxSignalChildren(self,event,data);
			free(self->subBoxes);
			free(Image);
			free(self);
			break;
			
		case AEUIBoxEventClicked:{
			AEVec2i move={5,5};
			AEUIBoxDefaultEvent(Box,AEUIBoxEventMove,&move);
			}break;
		
		default:
			return AEUIBoxDefaultEvent(self,event,data);
			break;
			
	}
	return NULL;

}

void perframe(float step){
	if(AEKey(' ')){
		AEVec2i move={50,50};
		AEUIBoxDefaultEvent(Box,AEUIBoxEventMove,&move);
	//	Box->x+=move.x;
	}
	AEUIBoxSignal(Box,AEUIBoxEventRender,NULL);
}

int main(int argc,char** argv){
	AEInit("UIBox Test",800,500);
	//Code here
	Box=AEUIBoxAddChild(NULL,ImageBoxEvent,NULL,0,0,800,500);
	AERefreshViewport(1);
	AEStart(perframe);
}
