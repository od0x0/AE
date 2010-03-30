#include "../AE/Text.h"
#include "../AE/Core.h"

AEFont* Font=NULL;

void perframe(float step){
	glLoadIdentity();
	glTranslatef(0,0,-2);
	AETextMultiline("Hello World.\nThis is a line of text\nMultiple lines of text are drawn with:\nAETextMultiline(string,font,x,y)",Font,0,512-20);
}

int main(int argc,char** argv){
	AEInit("Text Test",512,512);
	AEFontInit();
	Font=AEFontNew("Data/ARIAL.TTF",20);
	AERefreshViewport(1);
	glClearColor(1,1,1,1);
	glTranslatef(0,0,-2);
	AEStart(perframe);
	return 1;
}
