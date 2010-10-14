#include "UIView.h"

AEVec2i AEUIMouse={0,0};
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
	size_t length=[subviews length];
	for(size_t i=0; i<length; i++){
		AEUIView* child=[subviews at: i];
		
		float wfactor=bounds.w/(float)newbounds.w;
		float hfactor=bounds.h/(float)newbounds.h;
		
		float xbias=bounds.x-newbounds.x;
		float ybias=bounds.y-newbounds.y;
		
		AEUIRect childsNewBounds=[child bounds];
		childsNewBounds.x+=xbias;
		childsNewBounds.y+=ybias;
		childsNewBounds.w*=wfactor;
		childsNewBounds.h*=hfactor;
		[child setBounds: childsNewBounds];
	}
	bounds=newbounds;
}

-(AEUIRect)bounds{
	return bounds;
}

-(AEUIView*)setParent:(id)newparent{
	[parent removeSubview:self];
	parent=newparent;
	if(font==NULL) [self setFont:[parent font]];
	[parent addSubview:self];
	return self;
}

-(AEUIView*)parent{
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
	backgroundTexture=AEImageToTextureWithFlags(newimage,AETextureLoadFlagInvertY);
}

-(AEImage*)background{
	return NULL;
}

-(void)addSubview:(id)child{
	if(subviews==nil) subviews=[[AEUIArray alloc] init];
	[subviews add: [child retain]];
}

-(void)removeSubview:(id)child{
	if(subviews==nil) return;
	[subviews remove: child];
	[child release];
}

-(AEUIArray*)subviews{
	return subviews;
}

-(void)event:(AEUIViewEvent*)event{
	if(isHidden) return;

	AEUIRect mouseRect=AEUIRectNew(AEUIMouse.x,AEContextActiveGet()->h-AEUIMouse.y,0,0);
	int mouseIsHovering=AEUIRectsCollide(bounds,mouseRect);

	if(event->type==AEUIViewEventTypeMouseButton && event->data.mousebutton.wasPressed){
		/*float mouseY=AEStateActiveGet()->h-AEStateActiveGet()->mouse.y;
		char conditions=0;
		conditions+= bounds.x < AEStateActiveGet()->mouse.x;
		conditions+= bounds.y < mouseY;
		conditions+= (bounds.x+bounds.w) > AEStateActiveGet()->mouse.x;
		conditions+= (bounds.y+bounds.h) > mouseY;*/
		if(mouseIsHovering){
			//if(event->data.mousebutton.button==1) 
			[self click];
		}
		else [self loseFocus];
	}
	//Drag handling
	else if(mouseIsHovering && event->type==AEUIViewEventTypeMouseMove && AESDLMouseButton(1)){
		[self moveX: event->data.mousemove.x Y:event->data.mousemove.y];
		return;//Skip children handling it
	}
	
	size_t length=[subviews length];
	for(size_t i=0;i<length;i++)
		[[subviews at:i] event:event];
}

-(void)moveX:(int)x Y:(int)y{
	bounds.x+=x;
	bounds.y+=y;
}

-(void)focus{
	//[self unhide];
	[AEUIViewActive loseFocus];
	[parent focus];
	AEUIArray* siblings=[parent subviews];
	if(siblings){
		id top=[siblings at:[siblings length]-1];
		[siblings swap:top with:self];
	}
	AEUIViewActive=self;
}

-(void)loseFocus{
	if(self==AEUIViewActive) AEUIViewActive=nil;
}

-(BOOL)hasFocus{
	return (not isHidden) and AEUIViewActive==self;
}

-(void)render{
	if(isHidden) return;

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

-(void)update:(float)stepInSeconds{
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
	if(isHidden) return;
	[self focus];
}

-(void)hide{
	isHidden=YES;
	//[self loseFocus];
}

-(void)unhide{
	isHidden=NO;
}

-(BOOL)isHidden{
	return isHidden;
}

@end

AEUIView* AEUIViewActive;
