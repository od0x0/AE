#include "../AE/Image.h"
#include "../AE/Core.h"
#include <SDL/SDL.h>

AEImage* ScreenImage=NULL;
AEImage* Image=NULL;

void perframe(float step){
	AEImageBlit(NULL,0,0,Image);
	//glRasterPos3i(0,0,-1);
	//glDrawPixels(Image->w,Image->h,GL_RGBA,GL_UNSIGNED_BYTE,Image->pixels);
}

int main(int argc,char** argv){
	AEInit("Image Test",128,128);
	Image=AEImageFromFile("Data/Texture.png");
	//printf()
	AERefreshViewport(1);
	AEStart(perframe);
	return 1;
}
