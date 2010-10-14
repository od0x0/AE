//
//  UIButtonView.m
//  AEED
//
//  Created by Oliver Daids on 9/9/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import "UIButtonView.h"


@implementation AEUIButtonView

-init{
	self=[super init];
	[self lock];
	return self;
}

-(void)render{
	unsigned char oldcolor[4];
	memcpy(oldcolor,color,sizeof(oldcolor));
	if(state){
		color[0]*=0.5;
		color[1]*=0.5;
		color[2]*=0.5;
	}
	[super render];
	memcpy(color,oldcolor,sizeof(oldcolor));
}

-(void)click{
	state=2;
	if(clickCallback and not clickCallback(self,clickCallbackContext)) return;
	[super click];
}

-(void)event:(AEUIViewEvent *)event{
	if(isHidden) return;

	AEUIRect mouseRect=AEUIRectNew(AEUIMouse.x,AEContextActiveGet()->h-AEUIMouse.y,0,0);
	int mouseIsHovering=AEUIRectsCollide(bounds,mouseRect);
	
	if(mouseIsHovering) state=1;
	else state=0;
	
	if(mouseIsHovering && event->type==AEUIViewEventTypeMouseMove && AESDLMouseButton(1)){
		return;//Skip children handling it
	}
	
	[super event: event];
}

-(int)buttonID{
	return buttonID;
}

-(void)setButtonID:(int)newButtonID{
	buttonID=newButtonID;
}

-(AEUIButtonViewClickCallback)clickCallback{
	return clickCallback;
}

-(void)setClickCallback:(AEUIButtonViewClickCallback)newClickCallback withContext:(void*)context{
	clickCallback=newClickCallback;
	clickCallbackContext=context;
}

@end
