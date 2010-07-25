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
	unsigned char* pixels=SOIL_load_image(filename,&w,&h,&channelCount,SOIL_LOAD_AUTO);
//	printf("Channel Count: %i\n",channelCount);
	AEImage* image=calloc(1,sizeof(AEImage));
	image->w=w;
	image->h=h;
	image->channels=channelCount;
	image->pixels=pixels;
	return image;
}

AEImage* AEImageFromMemory(void* data,size_t dataSize){
	int channelCount,w,h;
	unsigned char* pixels=SOIL_load_image_from_memory(data,dataSize,&w,&h,&channelCount,SOIL_LOAD_AUTO);
//	printf("Channel Count: %i\n",channelCount);
	AEImage* image=calloc(1,sizeof(AEImage));
	image->w=w;
	image->h=h;
	image->channels=channelCount;
	image->pixels=pixels;
	return image;
}

unsigned char* AEImagePixelGet(AEImage* image,int x,int y){
	if(x > image->w || y> image->h || y < 0 || x < 0) return NULL;
	int channels=AEImageChannelsPerPixelGet(image);
	return image->pixels+x*channels+y*image->h*channels;
}

void AEImagePixelSet(AEImage* image,int x,int y,unsigned char* rgba){
	if(x > image->w || y> image->h || y < 0 || x < 0) return;
	int channels=AEImageChannelsPerPixelGet(image);
	unsigned char* pixel = image->pixels+x*channels+y*image->h*channels;
	for(int i=0;i<channels;i++) pixel[i]=rgba[i];
}

int AEImageChannelsPerPixelGet(AEImage* image){
	return image->channels;
}

void AEImageBlit(AEImage* to,int offsetx,int offsety,AEImage* from){
	//Currently only works on rgba images
	if(to && AEImageChannelsPerPixelGet(to)!=4) return;
	if(from && AEImageChannelsPerPixelGet(from)!=4) return;
	
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
			pixelTo=AEImagePixelGet(to,x+offsetx,y+offsety);
			pixelFrom=AEImagePixelGet(from,x,y);
			if(pixelFrom && pixelTo){
				//to=fromAlpha*from+(1-fromAlpha)*to
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

unsigned int AEImageToTexture(AEImage* image){
	/*GLuint texture=0;
	glGenTextures(1,&texture);
	AETextureBind(texture);
	gluBuild2DMipmaps( GL_TEXTURE_2D, 4, image->w,image->h, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
	return (unsigned int)texture;*/
	int channels=AEImageChannelsPerPixelGet(image);
	return SOIL_create_OGL_texture
	(
		image->pixels,
		image->w, image->h, channels,
		SOIL_CREATE_NEW_ID,
		AEStateActiveGet()->textureLoadFlags
	);
}