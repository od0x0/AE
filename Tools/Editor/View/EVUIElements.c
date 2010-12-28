#include "EVUIElements.h"

static inline void PumpArc(int x, int y, int radius, int startAngle, int endAngle){
	for(size_t i=startAngle;i<endAngle;i+=10) glVertex2f(x + sin(i*AEPiOver180) * radius, y + cos(i*AEPiOver180) * radius);
}

void EVUIDrawRect(EVUI* self, EVUIRect* bounds, EVUIRenderStyle* style){
	float cornerRadius = 0;

	if(style){
		cornerRadius=style->cornerRadius;
		glColor4f(style->fillColor.r, style->fillColor.g, style->fillColor.b, style->fillColor.a);
		AETextureBind(style->texture);
	}
	
	//To Do: Add Texturing and support for atlases!
	
	/*if(cornerRadius==0){
		glBegin(GL_QUADS);
		glVertex2f(bounds->x, bounds->y+bounds->h);
		glVertex2f(bounds->x, bounds->y);
		glVertex2f(bounds->x+bounds->w, bounds->y);
		glVertex2f(bounds->x+bounds->w, bounds->y+bounds->h);
		glEnd();
		return;
	}*/
	
	glFrontFace(GL_CW);
	glBegin(GL_POLYGON);
	PumpArc(bounds->x+bounds->w-cornerRadius, bounds->y+bounds->h-cornerRadius, cornerRadius, 0, 90);
	PumpArc(bounds->x+bounds->w-cornerRadius, bounds->y+cornerRadius, cornerRadius, 90, 180);
	PumpArc(bounds->x+cornerRadius, bounds->y+cornerRadius, cornerRadius, 180, 270);
	PumpArc(bounds->x+cornerRadius, bounds->y+bounds->h-cornerRadius, cornerRadius, 270, 360);
	glEnd();
	if(style){
		glColor4f(style->edgeColor.r, style->edgeColor.g, style->edgeColor.b, style->edgeColor.a);
		AETextureBind(0);
		glLineWidth(style->edgeWidth);
	}
	glBegin(GL_LINE_LOOP);
	PumpArc(bounds->x+bounds->w-cornerRadius, bounds->y+bounds->h-cornerRadius, cornerRadius, 0, 90);
	PumpArc(bounds->x+bounds->w-cornerRadius, bounds->y+cornerRadius, cornerRadius, 90, 180);
	PumpArc(bounds->x+cornerRadius, bounds->y+cornerRadius, cornerRadius, 180, 270);
	PumpArc(bounds->x+cornerRadius, bounds->y+bounds->h-cornerRadius, cornerRadius, 270, 360);
	glEnd();
	glFrontFace(GL_CCW);
}

void EVUIDrawLine(EVUI* self, AEVec2* points, size_t pointCount, EVUIRenderStyle* style){
	if(style){
		glColor4f(style->fillColor.r, style->fillColor.g, style->fillColor.b, style->fillColor.a);
		AETextureBind(style->texture);
		glLineWidth(style->edgeWidth);
	}
	glBegin(GL_LINES);
		if(pointCount > 1) for (size_t i=0; i<(pointCount-1); i++) {
			AEVec2 p=points[i+0];
			AEVec2 p2=points[i+1];
			glVertex2f(p.x, p.y);
			glVertex2f(p2.x, p2.y);
		}
	glEnd();
}

void EVUIDrawBezier(EVUI* self, AEVec2* controlPoints, size_t controlPointCount, size_t subdivisionCount, EVUIRenderStyle* style){
	AEVec2 points[subdivisionCount];
	
	for(size_t i=0; i<subdivisionCount; i++){
		points[i]=AEVec2sEvaluateBezierCurve(controlPoints, controlPointCount, i/(float)(subdivisionCount-1));
	}
	EVUIDrawLine(self, points, subdivisionCount, style);
}

void EVUIDrawText(EVUI* self, EVUIRect* bounds, const char* text, EVUIRenderStyle* style){
	if(style==NULL) return;
	AEVec2 center={
		bounds->x+bounds->w/2,
		bounds->y+bounds->h/2
	};
	switch (style->fontStyle.alignment) {
		case AEFontAlignLeft:
			center.x=bounds->x;
			center.y=(bounds->y+bounds->h);//-([font lineHeight]);
			break;
		case AEFontAlignCenter:
			center.x=bounds->x+bounds->w/2;
			center.y=bounds->y+bounds->h/2+(AEFontLineHeightGet(style->font)/4)*3;
			break;
		default:
			AEError("Unsupported alignment.");
			break;
	}
	glPushAttrib(GL_SCISSOR_BIT);
	glEnable(GL_SCISSOR_TEST);
	glScissor(bounds->x, bounds->y, bounds->w, bounds->h);
	if(style->font) AEFontRenderText(style->font, & style->fontStyle, center.x, center.y, 0, 0, text, 0);
	glPopAttrib();
}

bool EVUIDoButton(EVUI* self, EVUIRect* bounds, const char* text, EVUIRenderStyle* upStyle, EVUIRenderStyle* downStyle){
	AEVec2 mouse;
	EVUIMouseGet(self, & mouse);
	EVUIRect mouseRect={mouse.x, mouse.y, 0, 0};
	
	bool isClicked=EVUIMouseButtonIsDown(self, EVUIMouseButtonLeft) and EVUIRectIntersects(*bounds, mouseRect) and (self->mouseClipping ? EVUIRectIntersects(self->mouseClipBounds, mouseRect): true);
	
	EVUIRenderStyle style;
	if(isClicked) style=*downStyle;
	else style=*upStyle;
	style.fontStyle.alignment=0;
	
	EVUIDrawRect(self, bounds, & style);
	EVUIDrawText(self, bounds, text, & style);
	
	return isClicked and EVUIMouseButtonWasJustPressed(self, EVUIMouseButtonLeft);
}

bool EVUIDoTextBox(EVUI* self, EVUIRect* bounds, const char* text, EVUIRenderStyle* style, EVUIRenderStyle* activeStyle, size_t ID){
	if(EVUIDoButton(self, bounds, NULL, style, activeStyle)){
		self->textbox.activeID=ID;
		AETextBufferDelete(self->textbox.textBuffer);
		self->textbox.textBuffer=AETextBufferNew();
		AETextBufferInsert(self->textbox.textBuffer, 0, text);
		self->textbox.caret=AETextBufferLengthGet(self->textbox.textBuffer);
	}
	else if(EVUIMouseButtonWasJustPressed(self, EVUIMouseButtonLeft)) EVUIPushCharacter(self, '\n');
	
	if(ID not_eq self->textbox.activeID){
		bounds->y+=2;
		bounds->x+=style->textboxHorizontalShift;
		EVUIDrawText(self, bounds, text, style);
		bounds->x-=style->textboxHorizontalShift;
		bounds->y-=2;
		return false;
	}
	else{
		style=activeStyle;
		const char* rendertext = AETextBufferTextGet(self->textbox.textBuffer);
		AEVec2 center={
			bounds->x+bounds->w/2,
			bounds->y+bounds->h/2
		};
		center.x=bounds->x+style->textboxHorizontalShift;
		float textLength=AEFontMeasureTextLength(style->font, rendertext, self->textbox.caret);
		if(self->textbox.caret)///Quit bug fix for bad wrapping on a caret of 0
		if(center.x+textLength > bounds->x+bounds->w){
			center.x-=textLength-bounds->w/4*3;
		}
		center.y=(bounds->y+bounds->h);//-([font lineHeight]);
		glPushAttrib(GL_SCISSOR_BIT);
		glEnable(GL_SCISSOR_TEST);
		glScissor(bounds->x, bounds->y-bounds->h, bounds->w, 2*bounds->h);
		AEFontRenderText(style->font, & style->fontStyle, center.x, center.y+style->textboxVerticalShift, 0, 0, rendertext, self->textbox.caret);
		glPopAttrib();
	}
	bool wasChanged = self->textbox.wasEditted;
	self->textbox.wasEditted=false;
	if(wasChanged){
		self->textbox.activeID=0;
		self->textbox.caret=0;
	}
	return wasChanged;
}

bool EVUIDoSlider(EVUI* self, EVUIRect* bgBounds, EVUIRect* knobBounds, EVUIRenderStyle* bgStyle, EVUIRenderStyle* knobStyle, AEVec2* value){
	bool hasBeenChanged=EVUIDoButton(self, bgBounds, NULL, bgStyle, bgStyle);
	AEVec2 mouse;
	EVUIMouseGet(self, & mouse);
	EVUIRect mouseRect={mouse.x, mouse.y, 0, 0};
	hasBeenChanged = EVUIMouseButtonIsDown(self, EVUIMouseButtonLeft) and EVUIRectIntersects(*bgBounds, mouseRect) and (self->mouseClipping ? EVUIRectIntersects(self->mouseClipBounds, mouseRect): true);
	EVUIDrawRect(self, knobBounds, knobStyle);
	if(hasBeenChanged and value){
		EVUIMouseGet(self, value);
		value->x=AEClamp((value->x-(float)bgBounds->x)/bgBounds->w, 0, 1);
		value->y=AEClamp((value->y-(float)bgBounds->y)/bgBounds->h, 0, 1);
	}
	return hasBeenChanged;
}