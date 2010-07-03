#include "../AE/Core.h"
#include "../AE/WM.h"
int main(int argc,char** argv){
	AEInit("Window",800,500);
	//Code here
	AEWM* extrawm=AEWMOpen("Hello world",320,240);
	AEStart(NULL);
	return 1;
}
