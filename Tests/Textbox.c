#include "../AE/UI.h"
#include "../AE/Core.h"

AEFont* Font=NULL;
AEUITextbox* Textbox=NULL;

void perframe(float step){
	glLoadIdentity();
	glTranslatef(0,0,-2);
	AEUIBoxSignal(Textbox,AEUIBoxEventRender,NULL);
}

int main(int argc,char** argv){
	AEInit("Text Test",512,512);
	AEFontInit();
	
	Font=AEFontNew("Data/ARIAL.TTF",20);
	
	Textbox=AEUIBoxAddChild(NULL,AEUITextboxEvent,NULL,0,0,800,500);
	Textbox->text=strdup("Hello world.-----|");
	Textbox->max=strlen(Textbox->text);
	Textbox->caret=Textbox->max-1;
	Textbox->font=Font;
	
	SDL_SetEventFilter(AEUIBoxEventFilter);
	
	AERefreshViewport(1);
	glClearColor(1,1,1,1);
	glTranslatef(0,0,-2);
	AEStart(perframe);
	return 1;
}
