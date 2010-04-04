#include "../UIBox.h"

void AEUIDrawBox(int x,int y,int w,int h,char filled){
	if(filled){
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); 
		glVertex3i(x, y,-1);
		glTexCoord2f(1, 0); 
		glVertex3i(x+w,y,-1);
		glTexCoord2f(1, 1); 
		glVertex3i(x+w, y+h,-1);
		glTexCoord2f(0, 1); 
		glVertex3i(x, y+h,-1);
		glEnd();
	}
	else{
		glBegin(GL_LINE_LOOP);
		glVertex3i(x, y,-1);
		glVertex3i(x+w,y,-1);
		glVertex3i(x+w, y+h,-1);
		glVertex3i(x, y+h,-1);
		glVertex3i(x, y,-1);
		glEnd();
	}
}

AEUIBox* AEUIBoxActive=NULL;
AEUIBox* AEUIBoxRoot=NULL;

void AEUIBoxSignalChildren(AEUIBox* root,int event,void* data){
	if(root==NULL) return;
	for(unsigned int i=0;i<root->subBoxCount;i++){
		AEUIBox* box=root->subBoxes[i];
		(*box->event)(box,event,data);
	}
}

void* AEUIBoxSignal(AEUIBox* box,int event,void* data){
	if(box) return (*box->event)(box,event,data);
	return NULL;
}

void AEUIBoxRaise(AEUIBox* root,AEUIBox* box){
	unsigned int b=0;
	for(b=0;b<root->subBoxCount;b++){
		if(root->subBoxes[b]==box) break;
	}
	AEUIBox* top=root->subBoxes[root->subBoxCount-1];
	root->subBoxes[root->subBoxCount-1]=box;
	root->subBoxes[b]=top;
}

void* AEUIBoxDefaultEvent(AEUIBox* self,int event,void* data){
	switch(event){
	
		case AEUIBoxEventInit:
			return self;
			break;
			
		case AEUIBoxEventAddChild:
			self->subBoxCount++;
			self->subBoxes=realloc(self->subBoxes,self->subBoxCount*sizeof(AEUIBox*));
			self->subBoxes[self->subBoxCount-1]=data;
			break;
			
		case AEUIBoxEventRender:
			AEUIBoxSignalChildren(self,event,data);
			break;
			
		case AEUIBoxEventSDLEvent:
			//	if(AEUIBoxTouchesRect(self,)) AEUIBoxActive=self;
			{
			const SDL_Event* event=(const SDL_Event*)data;
			switch(event->type){
				case SDL_MOUSEBUTTONDOWN:{
					float mouseY=AEScreen.h-AEMouse.y;
					char conditions=0;
					conditions+= self->x < AEMouse.x;
					conditions+= self->y < mouseY;
					conditions+= (self->x+self->w) > AEMouse.x;
					conditions+= (self->y+self->h) > mouseY;
					if(conditions==4){
						(*self->event)(self,AEUIBoxEventClicked,data);
					}
					}break;
			}
			}
			AEUIBoxSignalChildren(self,event,data);
			break;
		
		case AEUIBoxEventRelease:
			AEUIBoxSignalChildren(self,event,data);
			free(self->subBoxes);
			free(self);
			break;
		
		case AEUIBoxEventMove:{
			AEVec2i* move=data;
			self->x+=move->x;
			self->y+=move->y;
			AEUIBoxSignalChildren(self,event,data);
			}break;
		
		case AEUIBoxEventValue:
			return NULL;
		
		default:
			AEUIBoxSignalChildren(self,event,data);
			break;
			
	}
	return NULL;
}

AEUIBox* AEUIBoxAddChild(AEUIBox* root,
	void* (*event)(AEUIBox*,int,void*),
	void* data,int x,int y,int w,int h){
	
	AEUIBox* box=calloc(1,sizeof(AEUIBox));
	box->x=x;
	box->y=y;
	box->w=w;
	box->h=h;
	if(event==NULL) event=AEUIBoxDefaultEvent;
	box->event=event;
	box=(*box->event)(box,AEUIBoxEventInit,data);
	if(root) (*root->event)(root,AEUIBoxEventAddChild,box);
	if(AEUIBoxRoot==NULL) AEUIBoxRoot=box;
	return box;
}

int AEUIBoxTouchesRect(AEUIBox* box,int x,int y,int w,int h){
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

	SDL_Rect A={box->x,box->y,box->w,box->h};
	SDL_Rect B={x,y,w,h};
	
	leftA = A.x;
	rightA = A.x + A.w;
	topA = A.y;
	bottomA = A.y + A.h;
        
	leftB = B.x;
	rightB = B.x + B.w;
	topB = B.y;
	bottomB = B.y + B.h;
	
	if( bottomA <= topB ) return 0;
	if( topA >= bottomB ) return 0;
	if( rightA <= leftB ) return 0;
	if( leftA >= rightB ) return 0;
	
	return 1;
}