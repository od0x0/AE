#include "../AE/Core.h"
#include "../AE/Text.h"
#include "../AE/UI.h"



void AELuaUIRun(char* file);

AEFont* Font=NULL;

void perframe(float step){
	//
	//glTranslatef(0,0,1);
	//AEText("Hello from C",Font,400,250);
	//AEUIRenderMode(0);
}

int main(int argc,char** argv){
	puts(argv[0]);
	AEInit("Window",800,500);
	glClearColor(1,1,1,1);
	//Code here
	//AEFontInit();
	//Font=AEFontNew("Data/ARIAL.TTF",100);
	//AEUIRenderMode(1);
	glTranslatef(0,0,1);
	AELuaUIRun("LuaUI.lua");
	//AEUIRenderMode(0);
	//AEStart(perframe);
	return 1;
}
