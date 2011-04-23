#include "AEImage.h"

void AEImageInitWithSize(AEImage* image, int w,int h){
	memset(image, 0, sizeof(AEImage));
	image->pixels=calloc(w*h,4);
	image->w=w;
	image->h=h;
	image->channels=4;
}

void AEImageInitFromFile(AEImage* image, const char* filename){
	if(filename==NULL) return;
	int channelCount,w,h;
	unsigned char* pixels=SOIL_load_image(filename,&w,&h,&channelCount,SOIL_LOAD_AUTO);
	if(pixels==NULL){
		printf("Loading of %s failed because %s",filename,SOIL_last_result());
		AEError("");
		//return NULL;
	}
//	printf("Channel Count: %i\n",channelCount);
	memset(image, 0, sizeof(AEImage));
	image->w=w;
	image->h=h;
	image->channels=channelCount;
	image->pixels=pixels;
}

void AEImageInitFromMemory(AEImage* image, void* data,size_t dataSize){
	int channelCount,w,h;
	unsigned char* pixels=SOIL_load_image_from_memory(data,dataSize,&w,&h,&channelCount,SOIL_LOAD_AUTO);
//	printf("Channel Count: %i\n",channelCount);
	memset(image, 0, sizeof(AEImage));
	image->w=w;
	image->h=h;
	image->channels=channelCount;
	image->pixels=pixels;
}

void AEImageGetPixel(AEImage* image,int x,int y,unsigned char* rgba){
	if(x > image->w || y> image->h || y < 0 || x < 0) return;
	int channels=image->channels;
	memcpy(rgba,image->pixels+x*channels+y*image->w*channels,4);
}

void AEImageSetPixel(AEImage* image,int x,int y,unsigned char* rgba){
	if(x > image->w || y> image->h || y < 0 || x < 0) return;
	int channels=image->channels;
	unsigned char* pixel = image->pixels+x*channels+y*image->w*channels;
	for(int i=0;i<channels;i++) pixel[i]=rgba[i];
}

void AEImageBlit(AEImage* to,int offsetx,int offsety,AEImage* from){
	//Currently only works on rgba images
	if(to && to->channels!=4) return;
	if(from && from->channels!=4) return;
	
	if(to==NULL){
		//Blit to screen, slow right now; Just for testing
		if(from==NULL) return;
		glRasterPos3i(offsetx,offsety,-1);
		glDrawPixels(from->w,from->h,GL_RGBA,GL_UNSIGNED_BYTE,from->pixels);
		return;
	}
	else if(from==NULL){
		//Clear to white
		memset(to->pixels,255,to->w*to->h*to->channels);
		return;
	}
	unsigned char pixelTo[4],pixelFrom[4];
	for(int x=0;x < from->w;x++)
		for(int y=0;y < from->h;y++){
			AEImageGetPixel(to,x+offsetx,y+offsety,pixelTo);
			AEImageGetPixel(from,x,y,pixelFrom);
			if(pixelFrom && pixelTo){
				//to=fromAlpha*from+(1-fromAlpha)*to
				float srcAlpha=pixelFrom[3]/255.0f;
				pixelTo[0]=pixelFrom[0]*srcAlpha+pixelTo[0]*(1-srcAlpha);
				pixelTo[1]=pixelFrom[1]*srcAlpha+pixelTo[1]*(1-srcAlpha);
				pixelTo[2]=pixelFrom[2]*srcAlpha+pixelTo[2]*(1-srcAlpha);
				pixelTo[3]=pixelFrom[3]*srcAlpha+pixelTo[3]*(1-srcAlpha);
			}
			AEImageSetPixel(to,x+offsetx,y+offsety,pixelTo);
		}
}

void AEImageDeinit(AEImage* image){
	free(image->pixels);
}

AETexture AEImageToTextureWithFlags(AEImage* image, AETextureFlag textureLoadFlags){
	return SOIL_create_OGL_texture
	(
		image->pixels,
		image->w, image->h, image->channels,
		SOIL_CREATE_NEW_ID,
		textureLoadFlags | SOIL_FLAG_INVERT_Y
	);
}
