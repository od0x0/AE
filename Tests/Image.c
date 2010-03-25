#include "../AE/Image.h"
#include "../AE/Core.h"

AEImage* ScreenImage=NULL;
AEImage* Image=NULL;

void perframe(float step){
	
	int xcount=ScreenImage->w/Image->w;
	int ycount=ScreenImage->h/Image->h;
	
	for(int x=0;x<xcount;x++)
		for(int y=0;y<ycount;y++){
			AEImageBlit(ScreenImage,x*Image->w,y*Image->h,Image);
		}
		
	AEImageBlit(NULL,0,0,ScreenImage);
}

int main(int argc,char** argv){
	AEInit("Image Test",512,512);
	Image=AEImageFromFile("Data/Texture.png");
	ScreenImage=AEImageNew(512,512);
	AERefreshViewport(1);
	AEStart(perframe);
	return 1;
}
