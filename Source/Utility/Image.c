#include "../Image.h"

AEImage* AEImageNew(int w,int h){
	AEImage* image=calloc(1,sizeof(AEImage));
	image->pixels=calloc(w*h,4);
	image->w=w;
	image->h=h;
	return image;
}

AEImage* AEImageFromFile(char* filename){
	int channelCount,w,h;
	unsigned char* pixels=SOIL_load_image(filename,&w,&h,&channelCount,SOIL_LOAD_RGBA);
	AEImage* image=calloc(1,sizeof(AEImage));
	image->w=w;
	image->h=h;
	image->pixels=pixels;
	return image;
}

unsigned char* AEImagePixel(AEImage* image,int x,int y){
	if(x > image->w || y> image->h || y < 0 || x < 0) return NULL;
	return image->pixels+x*4+y*image->w;
}

void AEImageBlit(AEImage* to,int offsetx,int offsety,AEImage* from){
	if(to==NULL){
		//Blit to screen, slow right now; Just for testing
		if(from==NULL) return;
		glRasterPos3i(offsetx,offsety,-1);
		glDrawPixels(from->w,from->h,GL_RGBA,GL_UNSIGNED_BYTE,from->pixels);
		return;
	}
	else if(from==NULL){
		//Clear to white
		memset(to->pixels,255,to->w*to->h*4);
		return;
	}
	unsigned char *pixelTo=NULL,*pixelFrom=NULL;
	for(int x=0;x < from->w;x++)
		for(int y=0;y < from->h;y++){
			pixelTo=AEImagePixel(to,x+offsetx,y+offsety);
			pixelFrom=AEImagePixel(from,x,y);
			if(pixelFrom && pixelTo){
				float srcAlpha=pixelFrom[3]/255.0f;
				pixelTo[0]=pixelFrom[0]*srcAlpha+pixelTo[0]*(1-srcAlpha);
				pixelTo[1]=pixelFrom[1]*srcAlpha+pixelTo[1]*(1-srcAlpha);
				pixelTo[2]=pixelFrom[2]*srcAlpha+pixelTo[2]*(1-srcAlpha);
				pixelTo[3]=pixelFrom[3]*srcAlpha+pixelTo[3]*(1-srcAlpha);
			}
		}
}

void AEImageDelete(AEImage* image){
	free(image->pixels);
	free(image);
}

