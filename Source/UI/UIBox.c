#include "../UIBox.h"

AEUIBox* AEUIBoxActive=NULL;
AEUIBox* AEUIBoxRoot=NULL;

void AEUIBoxSignalChildren(AEUIBox* root,int event,void* data){
	if(!root){
		(*AEUIBoxRoot->event)(AEUIBoxRoot,event,data);
		return;
	}
	for(unsigned int i=0;i<root->subBoxCount;i++){
		AEUIBox* box=root->subBoxes[i];
		(*box->event)(box,event,data);
	}
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
			if(self->image){
				AEUIBoxSignalChildren(self,event,self->image);
				AEImageBlit(data,self->x,self->y,self->image);
			}
			break;
			
		case AEUIBoxEventSDLEvent:
				
			//	if(AEUIBoxTouchesRect(self,)) AEUIBoxActive=self;
			
			AEUIBoxSignalChildren(self,event,data);
			break;
		
		case AEUIBoxEventRelease:
			if(self->image) AEImageDelete(self->image);
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
	return box;
}

int AEUIBoxTouchesRect(AEUIBox* box,int x,int y,int w,int h){
	
	AEVec2i distance={box->x-x,box->y-y};
	if(distance.x<0) distance.x=-distance.x;
	if(distance.y<0) distance.y=-distance.y;
	
	AEVec2i combinedSize={box->w+w,box->h+h};
	
	if(distance.x>combinedSize.x) return 0;
	if(distance.y>combinedSize.y) return 0;
	
	return 1;
}