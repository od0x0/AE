#include "AEImage.h"
/*
#ifndef AEiOS
#include "SOIL.h"
#endif

//typedef GLuint GLuint;
typedef GLuint AETextureFlag;

#define AETextureFlagForcePowerOfTwo SOIL_FLAG_POWER_OF_TWO
#define AETextureFlagMipMap SOIL_FLAG_MIPMAPS
#define AETextureFlagRepeat SOIL_FLAG_TEXTURE_REPEATS
#define AETextureFlagPremultiplyAlpha SOIL_FLAG_MULTIPLY_ALPHA
#define AETextureFlagCompressDXT SOIL_FLAG_COMPRESS_TO_DXT
#define AETextureFlagLoadDXT SOIL_FLAG_DDS_LOAD_DIRECT
#define AETextureFlagNTSC SOIL_FLAG_NTSC_SAFE_RGB
#define AETextureFlagDefault (AETextureFlagCompressDXT|AETextureFlagMipMap|AETextureFlagRepeat)

GLuint AETextureLoadWithFlags(const char* filename, AETextureFlag flags);
#define AETextureLoad(filename) AETextureLoadWithFlags(filename,AETextureFlagDefault)
GLuint AETextureLoadFromMemoryWithFlags(void* data,size_t dataSize, AETextureFlag flags);
static inline void AETextureBind(GLuint texture){glBindTexture(GL_TEXTURE_2D, texture);}
void AETextureDelete(GLuint texture);

static inline GLuint AETextureLoad_internal(GLuint texture, AETextureFlag flags){
	if(texture==0) return 0;
	if(flags & AETextureFlagRepeat) return texture;
	AETextureBind(texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	return texture;
}

GLuint AETextureLoadWithFlags(const char* filename, AETextureFlag flags){
	//SOIL is EPIC, no denial.
	//Handles EVERYTHING, beautifully too
	#ifndef AEiOS
	GLuint texture = SOIL_load_OGL_texture
		(
			filename,
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			flags | SOIL_FLAG_INVERT_Y
		);
	#else
	GLuint texture=0;
	#endif
	return AETextureLoad_internal(texture, flags);
}

GLuint AETextureLoadFromMemoryWithFlags(void* data, size_t dataSize, AETextureFlag flags){
	#ifndef AEiOS
	GLuint texture = SOIL_load_OGL_texture_from_memory(data, dataSize, SOIL_LOAD_AUTO, 0, flags |  SOIL_FLAG_INVERT_Y);
	#else
	GLuint texture=0;
	#endif
	return AETextureLoad_internal(texture, flags);
}

void AETextureDelete(GLuint texture){
	glDeleteTextures(1,&texture);
}

GLuint AEImageToTextureWithFlags(AEImage* image, AETextureFlag textureLoadFlags){
	#ifndef AEiOS
	return SOIL_create_OGL_texture
	(
		image->pixels,
		image->w, image->h, image->channels,
		SOIL_CREATE_NEW_ID,
		textureLoadFlags | SOIL_FLAG_INVERT_Y
	);
	#else
	return 0;
	#endif
}

*/

void AEImageInitWithSize(AEImage* image, int w,int h,int channels){
	memset(image, 0, sizeof(AEImage));
	image->pixels=calloc(w*h,channels);
	image->w=w;
	image->h=h;
	image->channels=channels;
}
/*
void AEImageInitFromFile(AEImage* image, const char* filename){
	if(filename==NULL) return;
	int channelCount,w,h;
	#ifdef AEiOS
	unsigned char* pixels=NULL;
	#else
	//unsigned char* pixels=SOIL_load_image(filename,&w,&h,&channelCount,SOIL_LOAD_AUTO);
	#endif
	memset(image, 0, sizeof(AEImage));
	image->w=w;
	image->h=h;
	image->channels=channelCount;
	image->pixels=pixels;
}

void AEImageInitFromMemory(AEImage* image, void* data,size_t dataSize){
	int channelCount,w,h;
	#ifdef AEiOS
	unsigned char* pixels=NULL;
	#else
	unsigned char* pixels=SOIL_load_image_from_memory(data,dataSize,&w,&h,&channelCount,SOIL_LOAD_AUTO);
	#endif
//	printf("Channel Count: %i\n",channelCount);
	memset(image, 0, sizeof(AEImage));
	image->w=w;
	image->h=h;
	image->channels=channelCount;
	image->pixels=pixels;
}
*/
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
		//glRasterPos3i(offsetx,offsety,-1);
		//glDrawPixels(from->w,from->h,GL_RGBA,GL_UNSIGNED_BYTE,from->pixels);
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
