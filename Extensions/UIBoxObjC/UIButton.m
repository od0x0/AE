#import "UIButton.h"

@implementation AEUIButton
-init{
	self=[super init];
	if(self){
		self->borderColor.a=1;
		self->highlightBorderColor.r=0;
		self->highlightBorderColor.g=(float)0x90/0xff;
		self->highlightBorderColor.b=(float)0xd0/0xff;
		self->highlightBorderColor.a=(float)0xff/0xff;
		//[self insert:"" At:0];
	}
	return self;
}
-render{
	[super render];
	AETextureBind(0);
	glColor4f(0.5, 0.5, 0.5, 0.5);
	if(self->on) AEUIBoxRectDraw(self->rectangle);
	glLineWidth(2);
	
	
	
	AEUIBoxRect rect=self->rectangle;
	AERGBA bordercolor=self->borderColor;
	unsigned char bordercolorbytes[4]={0xff,0xff,0xff,0xff};
	bordercolorbytes[0]*=bordercolor.r;
	bordercolorbytes[1]*=bordercolor.g;
	bordercolorbytes[2]*=bordercolor.b;
	bordercolorbytes[3]*=bordercolor.a;
	glColor4ubv(bordercolorbytes);
	AEUIDrawRectOutline(rect);
	
	if(self->on){
		bordercolor=self->highlightBorderColor;
		rect.x+=5;
		rect.y+=5;
		rect.w-=5;
		rect.h-=5;
		unsigned char bordercolorbytes[4]={0xff,0xff,0xff,0xff};
		bordercolorbytes[0]*=bordercolor.r;
		bordercolorbytes[1]*=bordercolor.g;
		bordercolorbytes[2]*=bordercolor.b;
		bordercolorbytes[3]*=bordercolor.a;
		glColor4ubv(bordercolorbytes);
		float centerx=rect.x+rect.w/4;
		float centery=rect.y+rect.h/4;
		
		rect.x=-rect.w/4+centerx;
		rect.y=-rect.h/4+centery;
		rect.w=rect.w/4*3;
		rect.h=rect.h/4*3;
		AEUIDrawRectOutline(rect);
	}
	
	glColor3f(1,1,1);
	glLineWidth(1);
	
	return self;
}
-click{
	[super click];
	if(self->callback) self->callback(self);
	self->on=YES;
	return self;
}
-cursorUp:(int)button{
	if(self->on) self->on=NO;
	return self;
}
@end
