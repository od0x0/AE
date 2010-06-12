#include "../Image.h"

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    static Uint32 rmask = 0xff000000;
    static Uint32 gmask = 0x00ff0000;
    static Uint32 bmask = 0x0000ff00;
    static Uint32 amask = 0x000000ff;
#else
    static Uint32 rmask = 0x000000ff;
    static Uint32 gmask = 0x0000ff00;
    static Uint32 bmask = 0x00ff0000;
    static Uint32 amask = 0xff000000;
#endif

AEImage* AEImageNew(int w,int h){
	AEImage* image=calloc(1,sizeof(AEImage));
	//image->surface=SDL_CreateRGBSurface(SDL_SWSURFACE,w,h,32,rmask, gmask, bmask, amask);
	//image->pixels=image->surface->pixels;
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
	//image->surface=SDL_CreateRGBSurfaceFrom(pixels,w,h,32,w*4,rmask, gmask, bmask, amask);
	//image->pixels=image->surface->pixels;
	image->pixels=pixels;
	return image;
}

unsigned char* AEImagePixel(AEImage* image,int x,int y){
	if(x > image->w || y> image->h || y < 0 || x < 0) return NULL;
	return image->pixels+x*4+y*image->h*4;
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
	//SDL_Rect fromRect={offsetx,offsety,from->w,from->h};
	//SDL_BlitSurface(from->surface,&fromRect,to->surface,NULL);
	//SDL_BlitSurface(from->surface,NULL,to->surface,NULL);
	unsigned char *pixelTo=NULL,*pixelFrom=NULL;
	for(int x=0;x < from->w;x++)
		for(int y=0;y < from->h;y++){
			pixelTo=AEImagePixel(to,x+offsetx,y+offsety);//to,x+offsetx,y+offsety);
			//unsigned char WHITE[4]={255,255,255,255};
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
	//SDL_FreeSurface(image->surface);
	free(image->pixels);
	free(image);
}

unsigned int AEImageToTexture(AEImage* image){
	GLuint texture=0;
	glGenTextures(1,&texture);
	AETextureBind(texture);
	gluBuild2DMipmaps( GL_TEXTURE_2D, 4, image->w,image->h, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
	return (unsigned int)texture;
}