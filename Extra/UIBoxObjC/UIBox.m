#include "UIBox.h"

@implementation AEUIBox
-init{
	self=[super init];
	if(self){
		self->rectangle=AEUIBoxRectNew(0,0,0,0);
		self->children=NULL;
		self->parent=nil;
		[self focus];
	}
	return self;
}

-setRect:(AEUIBoxRect)rect{
	//Should really do scaling here, but whatever
	self->rectangle=rect;
	return self;
}

-(AEUIBoxRect)rect{
	return self->rectangle;
}

-addChild:(AEUIBox*)child{
	if(self->children==NULL) self->children=AEListNew(AEUIBox*);
	AEListAdd(self->children,AEUIBox*,child);
	child->parent=self;
	return self;
}

-removeChild:(AEUIBox*)child{
	return self;
}

-(AEList*)childList{
	return self->children;
}

-raiseChild:(AEUIBox*)child{
	if(self->children==NULL) return self;
	unsigned int b=0;
	
	AEUIBox** subBoxes=AEListAsArray(self->children,AEUIBox*);
	unsigned int subBoxCount=AEListLength(self->children);
	
	for(b=0;b<subBoxCount;b++){
		if(subBoxes[b]==child) break;
	}
	AEUIBox* top=subBoxes[subBoxCount-1];
	subBoxes[subBoxCount-1]=child;
	subBoxes[b]=top;
	return self;
}

-raise{
	[self->parent raise];
	[self->parent raiseChild: self];
	return self;
}

-focus{
	[self raise];
	AEUIBoxActive=self;
	return self;
}

-(BOOL)hasFocus{

	if(AEUIBoxActive==self) return YES;
	
	if(self->children){
		AEUIBox** subBoxes=AEListAsArray(self->children,AEUIBox*);
		unsigned int subBoxCount=AEListLength(self->children);
	
		for(size_t b=0;b<subBoxCount;b++){
			if(subBoxes[b]==AEUIBoxActive) return YES;
		}
	}
	return NO;
}

-render{
	glColor4f(self->color.r, self->color.g, self->color.b, self->color.a);
	//AETextureBind(self->texture);
	AEUIBoxRectDraw(self->rectangle);
	glColor3f(1, 1, 1);
	if(self->children){
		AEUIBox** subBoxes=AEListAsArray(self->children,AEUIBox*);
		unsigned int subBoxCount=AEListLength(self->children);
	
		for(size_t b=0;b<subBoxCount;b++){
			AEUIBox* box=subBoxes[b];
			[box render];
		}
	}
	return self;
}

-cursorDown:(int)button{
	float mouseY=AEStateActiveGet()->h-AEStateActiveGet()->mouse.y;
	char conditions=0;
	conditions+= self->rectangle.x < AEStateActiveGet()->mouse.x;
	conditions+= self->rectangle.y < mouseY;
	conditions+= (self->rectangle.x+self->rectangle.w) > AEStateActiveGet()->mouse.x;
	conditions+= (self->rectangle.y+self->rectangle.h) > mouseY;
	if(conditions==4){
		[self click];
	}
	if(self->children){
		AEUIBox** subBoxes=AEListAsArray(self->children,AEUIBox*);
		unsigned int subBoxCount=AEListLength(self->children);
	
		for(size_t b=0;b<subBoxCount;b++){
			AEUIBox* box=subBoxes[b];
			[box cursorDown:button];
		}
	}
	return self;
}

-cursorUp:(int)button{
	if(self->children){
		AEUIBox** subBoxes=AEListAsArray(self->children,AEUIBox*);
		unsigned int subBoxCount=AEListLength(self->children);
	
		for(size_t b=0;b<subBoxCount;b++){
			AEUIBox* box=subBoxes[b];
			[box cursorUp:button];
		}
	}
	return self;
}

-cursorMoveX:(int)x Y:(int)y{
	if(self->children){
		AEUIBox** subBoxes=AEListAsArray(self->children,AEUIBox*);
		unsigned int subBoxCount=AEListLength(self->children);
	
		for(size_t b=0;b<subBoxCount;b++){
			AEUIBox* box=subBoxes[b];
			[box cursorMoveX:x Y:y];
		}
	}
	return self;
}

-keyDown:(int)key{
	if(self->children){
		AEUIBox** subBoxes=AEListAsArray(self->children,AEUIBox*);
		unsigned int subBoxCount=AEListLength(self->children);
	
		for(size_t b=0;b<subBoxCount;b++){
			AEUIBox* box=subBoxes[b];
			[box keyDown:key];
		}
	}
	return self;
}

-keyUp:(int)key{
	if(self->children){
		AEUIBox** subBoxes=AEListAsArray(self->children,AEUIBox*);
		unsigned int subBoxCount=AEListLength(self->children);
	
		for(size_t b=0;b<subBoxCount;b++){
			AEUIBox* box=subBoxes[b];
			[box keyUp:key];
		}
	}
	return self;
}

-moveX:(int)x Y:(int)y{
	self->rectangle.x+=x;
	self->rectangle.y+=y;
	
	AEUIBox** subBoxes=AEListAsArray(self->children,AEUIBox*);
	unsigned int subBoxCount=AEListLength(self->children);
	
	for(size_t b=0;b<subBoxCount;b++){
		AEUIBox* box=subBoxes[b];
		[box moveX:x Y:y];
	}
	return self;
}

-click{	
	[self focus];
	return self;
}

-(void)dealloc{
	if(self->children){
		AEUIBox** subBoxes=AEListAsArray(self->children,AEUIBox*);
		unsigned int subBoxCount=AEListLength(self->children);
	
		for(size_t b=0;b<subBoxCount;b++){
			AEUIBox* box=subBoxes[b];
			[box release];
		}
	
		AEListDelete(self->children);
	}
	
	[self->parent release];
	
	[super dealloc];
}

@end

AEUIBox* AEUIBoxActive;