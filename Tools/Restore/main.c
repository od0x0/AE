#include "SDL.h"
#include "AE.h"
#include "ECOperationList.h"
#include "AETextBuffer.h"
#include "EVUIElements.h"
AEVec3 AEVec3EvaluateQuadraticBezierCurve(AEVec3 p, AEVec3 p2, AEVec3 p3, float t){
	return AEVec3Add(AEVec3Mul(p3, AEVec3FromSingle(t*t)), AEVec3Add(AEVec3Mul(p2, AEVec3FromSingle(2*(1-t)*t)), AEVec3Mul(p, AEVec3FromSingle((1-t)*(1-t)))));
}



/*static float bc(float n, float k){
	if(k == -1) return 1;
	return ((n-k) / (k+1)) * bc(n, k-1);
}

static float bc2(float n, float k){
	if(k > n-k) k = n - k;
	float c = 1;
	for(size_t i=0; i<k; i++){
		c *= n-i;
		c /= i+1;
	}
	return c;
}

AEVec3 AEVec3EvaluateBezierCurve(AEVec3* points, size_t count, float t){
	AEVec3 r={0,0,0};
	for (size_t i=0; i<count; i++) {
		size_t k = i;
		if(k > count-k) k = count - k;
		float c = 1;
		for(size_t j=0; j<k; j++){
			c *= count-j;
			c /= j+1;
		}
		//float 
		c = bc2(count, i);
		const float factor = c * powf(1-t, count-i) * powf(t, i);
		const AEVec3 p = AEVec3Mul(points[i], AEVec3FromSingle(factor));
		r = AEVec3Add(r, p);
	}
	return r;
}*/

/*AEVec3 AEVec3EvaluateBezierCurve(AEVec3* points, size_t count, float t){
	if(count == 0) return AEVec3FromSingle(0);
	return AEVec3Add(
		AEVec3Mul(AEVec3FromSingle(1-t), AEVec3EvaluateBezierCurve(points, count-1, t)),
		AEVec3Mul(AEVec3FromSingle(t), AEVec3EvaluateBezierCurve(points+1, count-1, t))
	);
}*/

int AESDLKey(int);
int AESDLMouseButton(char);
EVUI* UI;
AEFont* Font;

static void UIViewEnterUIDrawMode(void){
	glColor3f(1, 1, 1);
	glPushMatrix();
	glDisable(GL_DEPTH_TEST);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0,AEContextActiveGet()->w,0,AEContextActiveGet()->h,0,1000);
	glMatrixMode(GL_MODELVIEW);
	AETextureBind(0);
}

static void UIViewExitUIDrawMode(void){
	AETextureBind(0);
	glMatrixMode(GL_PROJECTION);
		glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glPopMatrix();
	glColor3f(1, 1, 1);
}

static void OnFrameUpdate(AEContext* context, double seconds, void* userdata){
	double difference=(1.0/60.0)-seconds;
	
	int x,y;
	SDL_GetMouseState(& x, & y);
	y=AEContextActiveGet()->h-y;
	EVUIMouseSet(UI, x, y);
	EVUIMouseButtonSet(UI, EVUIMouseButtonLeft, AESDLMouseButton(SDL_BUTTON_LEFT));
	EVUIMouseButtonSet(UI, EVUIMouseButtonRight, AESDLMouseButton(SDL_BUTTON_RIGHT));
	EVUIMouseButtonSet(UI, EVUIMouseButtonMiddle, AESDLMouseButton(SDL_BUTTON_MIDDLE));
	//EVUIMouseButtonSet(UI, EVUIMouseButtonUpWheel, AESDLMouseButton(SDL_BUTTON_WHEELUP));
	//EVUIMouseButtonSet(UI, EVUIMouseButtonDownWheel, AESDLMouseButton(SDL_BUTTON_WHEELDOWN));
	EVUIMouseButtonSet(UI, EVUIMouseButtonUpWheel, AESDLKey(SDLK_UP));
	EVUIMouseButtonSet(UI, EVUIMouseButtonDownWheel, AESDLKey(SDLK_DOWN));
	
	SDL_Event event;
	while (SDL_PollEvent(& event)) {
		switch (event.type) {
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
					case SDLK_BACKSPACE: EVUIPushBackspace(UI); break;
					case SDLK_LEFT: EVUIMoveCaret(UI, -1); break;
					case SDLK_RIGHT: EVUIMoveCaret(UI, 1); break;
					default: EVUIPushCharacter(UI, event.key.keysym.unicode); break;
				}
				break;
			default: break;
		}
	}
	
	glColor3f(1, 1, 1);
	
	EVUIRenderStyle style;
	EVUIRenderStyleInit(& style);
	style.fillColor.a=0;
	style.cornerRadius=2;
	style.font=Font;
	style.fontStyle.textColor.r=style.fontStyle.textColor.g=style.fontStyle.textColor.b=0;
	
	EVUIRenderStyle downStyle=style;
	downStyle.fillColor.r=0.5;
	downStyle.fillColor.g=0;
	downStyle.fillColor.b=0;
	downStyle.fillColor.a=1;
	downStyle.fontStyle.textColor.r=downStyle.fontStyle.textColor.g=downStyle.fontStyle.textColor.b=1;
	
	EVUIBeginRender(UI);
	
	#define itemCount 8
	size_t visibleItemCount=4;
	//if(visibleItemCount > itemCount) visibleItemCount=itemCount;
	static int itemOffset=0;
	const struct{const char* name;} items[itemCount]={
		{"Undo"},
		{"Redo"},
		{"Fail To Do"},
		{"Try To Do"},
		{"Do something"},
		{"Do this"},
		{"Do that"},
		{"Do now"}
	};
	if(itemOffset < 0) itemOffset=0;
	if(itemOffset >= itemCount-1) itemOffset=itemCount-1;
	if(visibleItemCount > itemCount-itemOffset) itemOffset=0;
	
	EVUIRenderStyle backgroundStyle=style;
	backgroundStyle.edgeColor.a=1;
	backgroundStyle.fillColor.r=backgroundStyle.fillColor.g=backgroundStyle.fillColor.b=1;
	backgroundStyle.fillColor.a=1;
	backgroundStyle.edgeWidth=2;
	EVUIRect rect=EVUIRectNew(95, 122-22*visibleItemCount-10, 115, 22*visibleItemCount+35);
	EVUIDrawRect(UI, &rect, &backgroundStyle);
	
	EVUIRenderStyle textboxstyle=style;
	textboxstyle.edgeColor.r=0.5;
	textboxstyle.edgeColor.g=0;
	textboxstyle.edgeColor.b=0;
	textboxstyle.edgeColor.a=1;
	textboxstyle.edgeWidth=2;
	EVUIRect textboxrect=EVUIRectNew(rect.x+5, rect.y+rect.h-22, rect.w-10, 18);
	textboxstyle.fillColor.a=0;
	textboxstyle.fontStyle.textColor.r=textboxstyle.fontStyle.textColor.g=textboxstyle.fontStyle.textColor.b=0;
	static char* text = NULL;
	if(text==NULL) text = AEStringDuplicate("Cool stuff bro");
	if(EVUIDoTextBox(UI, &textboxrect, text, &textboxstyle, &textboxstyle, 1)){
		free(text);
		text=AEStringDuplicate(EVUITextboxTextGet(UI));
	}
	
	if(visibleItemCount < itemCount){
		float height=22*(visibleItemCount+2);
		height *= itemOffset/(long double)itemCount;
		EVUIRect rect=EVUIRectNew(205, 100-height, 5, 22);
		//rect.y-=22*(visibleItemCount/(long double)itemCount);
		EVUIDrawRect(UI, &rect, &downStyle);
	}
	for(size_t i=itemOffset; i<(visibleItemCount+itemOffset); i++){
		EVUIRect rect=EVUIRectNew(100, 100-22*(i-itemOffset), 100, 20);
		if(EVUIDoButton(UI, &rect, items[i].name, & style, & downStyle)) printf("Button #%i, \"%s\" is clicked.\n", (int)i, items[i].name);
	}
	if(EVUIMouseButtonWasJustPressed(UI, EVUIMouseButtonDownWheel)) itemOffset++;
	if(EVUIMouseButtonWasJustPressed(UI, EVUIMouseButtonUpWheel)) itemOffset--;
	EVUIEndRender(UI);
	
	if(difference>0) SDL_Delay(1000*difference);
}

int main(int argc, char** argv){
	AEContext* context=AEContextActiveGet();
	AEContextOpen(context, "Ambition Engine Editor", 800, 500);
	AECameraPositionSet(AECameraActiveGet(), 0, 0, 3);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH, GL_NICEST);
	AETextBufferEdit* edit=NULL;
	edit = AETextBufferEditNew(edit, ", Mah Boi.", 0, 0);
	AETextBufferEdit* edit2=AETextBufferEditNew(edit, "Hello", 0, 0);
	edit=AETextBufferEditNew(edit, "Wai, Hai", 0, 0);
	
	SDL_EnableUNICODE(1);
	UI=EVUINew();
	Font=AEFontLoadFromTTF("/Users/Oliver/Downloads/Geo-Regular.ttf", 18);
	
	AETextBuffer* textbuffer=AETextBufferNew();
	AETextBufferInsert(textbuffer, 0, "BLAH");
	AETextBufferBackspace(textbuffer, 0, AETextBufferLengthGet(textbuffer));
	AETextBufferApplyEditChain(textbuffer, edit /*AETextBufferEditIntersection(edit, edit2)*/);
	puts(AETextBufferTextGet(textbuffer));
	glClearColor(1, 1, 1, 1);
	context->frameUpdate=OnFrameUpdate;
	AEContextRun(context);
	return 0;
}
