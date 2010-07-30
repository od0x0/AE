#include "UIView.h"


@implementation AEUIView
/*{
	AEUIRect bounds;
	AEUIView* parent;
	AEUIArray* subviews;
	AEFont* font;
	unsigned char color[4];
	unsigned char bordercolor[4];
	unsigned int backgroundTexture;
}*/
-init{
	self=[super init];
	if(self){
		[self focus];
	}
	return self;
}
-(void)setBounds:(AEUIRect)newbounds{
	bounds=newbounds;
}

-(AEUIRect)bounds{
	return bounds;
}

-setParent:(id)newparent{
	[parent removeSubview:self];
	parent=newparent;
	if(font==NULL) [self setFont:[parent font]];
	[parent addSubview:self];
	return self;
}

-parent{
	return parent;
}

-(void)setFont:(AEFont*)newfont{
	font=newfont;
}

-(AEFont*)font{
	return font;
}

-(void)setColorR:(float)r G:(float)g B:(float)b A:(float)a{
	color[0]=255*r;
	color[1]=255*g;
	color[2]=255*b;
	color[3]=255*a;
}

-(AERGBA)color{
	AERGBA result={color[0]/255.0f,color[1]/255.0f,color[2]/255.0f,color[3]/255.0f};
	return result;
}

-(void)setBorderColorR:(float)r G:(float)g B:(float)b A:(float)a{
	borderColor[0]=255*r;
	borderColor[1]=255*g;
	borderColor[2]=255*b;
	borderColor[3]=255*a;
}

-(AERGBA)borderColor{
	AERGBA result={borderColor[0]/255.0f,borderColor[1]/255.0f,borderColor[2]/255.0f,borderColor[3]/255.0f};
	return result;
}

-(void)setBackground:(AEImage*)newimage{
	AETextureDelete(backgroundTexture);
	backgroundTexture=AEImageToTexture(newimage);
}

-(AEImage*)background{
	return NULL;
}

-(void)addSubview:(id)child{
	if(subviews==nil) subviews=[[AEUIArray alloc] init];
	[subviews add:child];
}

-(void)removeSubview:(id)child{
	if(subviews==nil) return;
	[subviews remove:child];
}

-(AEUIArray*)subviews{
	return subviews;
}

-(void)event:(AEUIViewEvent*)event{
	AEUIRect mouseRect=AEUIRectNew(AEStateActiveGet()->mouse.x,AEStateActiveGet()->h-AEStateActiveGet()->mouse.y,0,0);
	int mouseIsHovering=AEUIRectsCollide(bounds,mouseRect);

	if(event->type==AEUIViewEventTypeMouseButton && event->data.mousebutton.wasPressed){
		/*float mouseY=AEStateActiveGet()->h-AEStateActiveGet()->mouse.y;
		char conditions=0;
		conditions+= bounds.x < AEStateActiveGet()->mouse.x;
		conditions+= bounds.y < mouseY;
		conditions+= (bounds.x+bounds.w) > AEStateActiveGet()->mouse.x;
		conditions+= (bounds.y+bounds.h) > mouseY;*/
		char conditions=4*mouseIsHovering;
		if(conditions==4){
			[self click];
		}
		else [self loseFocus];
	}
	else if(event->type==AEUIViewEventTypeMove){
		bounds.x+=event->data.move.x;
		bounds.y+=event->data.move.y;
	}
	//Drag handling
	else if(mouseIsHovering && event->type==AEUIViewEventTypeMouseMove && AEMouseButton(1)){
		AEUIViewEvent moveevent;
		moveevent.type=AEUIViewEventTypeMove;
		moveevent.data.move.x=event->data.mousemove.x;
		moveevent.data.move.y=event->data.mousemove.y;
		[self event:&moveevent];
		return;//Skip children handling it
	}
	
	size_t length=[subviews length];
	for(size_t i=0;i<length;i++)
		[[subviews at:i] event:event];
}

-(void)focus{
	[parent focus];
	AEUIArray* siblings=[parent subviews];
	if(siblings){
		id top=[siblings at:[siblings length]-1];
		[siblings swap:top with:self];
	}
	[AEUIViewActive loseFocus];
	AEUIViewActive=self;
}

-(void)loseFocus{
	if(self==AEUIViewActive) AEUIViewActive=nil;
}

-(BOOL)hasFocus{
	return AEUIViewActive==self;
}

-(void)render{
	glColor4ubv(self->color);
	AETextureBind(self->backgroundTexture);
	AEUIRectDraw(self->bounds);
	glColor4ubv(self->borderColor);
	AETextureBind(0);
	AEUIRectDrawOutline(self->bounds);
	glColor3f(1, 1, 1);

	size_t length=[subviews length];
	for(size_t i=0;i<length;i++)
		[[subviews at:i] render];
}

-(void)dealloc{
	[self loseFocus];
	[subviews release];
	AETextureDelete(backgroundTexture);
	[super dealloc];
}

-(void)click{
	[self focus];
}

@end

AEUIView* AEUIViewActive;
