#include "../Image.h"

AEImage* AEImageNew(int w,int h){
	AEImage* image=calloc(1,sizeof(AEImage));
	image->pixels=calloc(w*h,4);
	image->w=w;
	image->h=h;
	AEImageTextureFlagsSet(image,AETextureLoadFlagDXTCompression|AETextureLoadFlagMipmaps);
	return AEImageRetain(image);
}

AEImage* AEImageRetain(AEImage* image){
	image->refcount++;
	return image;
}

AEImage* AEImageClone(AEImage* image){
	//Not yet implemented
	return NULL;//AEImageRetain(image);
}

void AEImageSerializeToFILE(AEImage* image, FILE* file){
	
}

void AEImageUnserializeFromFILE(AEImage* image, FILE* file){
	free(image->pixels);
	free(image->metastring);
	
	uint64_t version;
	fread(&version, 1, sizeof(uint64_t), file);
	version=AEHostU64FromNet(version);
	if(version not_eq 1) AEError("Invalid version (must be 1).");
	uint64_t dataSize;
	fread(&dataSize, 1, sizeof(uint64_t), file);
	dataSize=AEHostU64FromNet(dataSize);
	
	void* data=malloc(dataSize);
	fread(data, 1, dataSize, file);
	
	int channelCount,w,h;
	unsigned char* pixels=SOIL_load_image_from_memory(data,dataSize,&w,&h,&channelCount,SOIL_LOAD_AUTO);
	free(data);
	
	image->w=w;
	image->h=h;
	image->channels=channelCount;
	image->pixels=pixels;
	
	uint64_t flags;
	fread(&flags, 1, sizeof(uint64_t), file);
	flags=AEHostU64FromNet(flags);
	AEImageTextureFlagsSet(image,flags);
}

void AEImageSerializeToMBuffer(AEImage* image,AEMBuffer* mbuffer){

}

void AEImageUnserializeFromMBuffer(AEImage* image,AEMBuffer* mbuffer){
	free(image->pixels);
	free(image->metastring);
	
	uint64_t version;
	//fread(&version, 1, sizeof(uint64_t), file);
	AEMBufferRead(mbuffer, &version, sizeof(uint64_t));
	version=AEHostU64FromNet(version);
	if(version not_eq 1) AEError("Invalid version (must be 1).");
	uint64_t dataSize;
	//fread(&dataSize, 1, sizeof(uint64_t), file);
	AEMBufferRead(mbuffer, &dataSize, sizeof(uint64_t));
	dataSize=AEHostU64FromNet(dataSize);
	
	//void* data=malloc(dataSize);
	//fread(data, 1, dataSize, file);
	void* data=AEMBufferBytesGet(mbuffer, dataSize);
	int channelCount,w,h;
	unsigned char* pixels=SOIL_load_image_from_memory(data,dataSize,&w,&h,&channelCount,SOIL_LOAD_AUTO);
	//free(data);
	
	image->w=w;
	image->h=h;
	image->channels=channelCount;
	image->pixels=pixels;
	
	uint64_t flags;
	//fread(&flags, 1, sizeof(uint64_t), file);
	AEMBufferRead(mbuffer, &flags, sizeof(uint64_t));
	flags=AEHostU64FromNet(flags);
	AEImageTextureFlagsSet(image,flags);
}

AEImage* AEImageLoad(char* filename){
	int channelCount,w,h;
	unsigned char* pixels=SOIL_load_image(filename,&w,&h,&channelCount,SOIL_LOAD_AUTO);
	if(pixels==NULL){
		printf("Texture loading of %s failed because %s",filename,SOIL_last_result());
		AEError("");
		//return NULL;
	}
//	printf("Channel Count: %i\n",channelCount);
	AEImage* image=calloc(1,sizeof(AEImage));
	image->w=w;
	image->h=h;
	image->channels=channelCount;
	image->pixels=pixels;
	AEImageTextureFlagsSet(image,AETextureLoadFlagDXTCompression|AETextureLoadFlagMipmaps);
	return AEImageRetain(image);
}

AEImage* AEImageLoadFromMemory(void* data,size_t dataSize){
	int channelCount,w,h;
	unsigned char* pixels=SOIL_load_image_from_memory(data,dataSize,&w,&h,&channelCount,SOIL_LOAD_AUTO);
//	printf("Channel Count: %i\n",channelCount);
	AEImage* image=calloc(1,sizeof(AEImage));
	image->w=w;
	image->h=h;
	image->channels=channelCount;
	image->pixels=pixels;
	AEImageTextureFlagsSet(image,AETextureLoadFlagDXTCompression|AETextureLoadFlagMipmaps);
	return AEImageRetain(image);
}

void AEImagePixelGet(AEImage* image,int x,int y,unsigned char* rgba){
	if(x > image->w || y> image->h || y < 0 || x < 0) return;
	int channels=AEImageChannelsPerPixelGet(image);
	memcpy(rgba,image->pixels+x*channels+y*image->h*channels,4);
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
	unsigned char pixelTo[4],pixelFrom[4];
	for(int x=0;x < from->w;x++)
		for(int y=0;y < from->h;y++){
			AEImagePixelGet(to,x+offsetx,y+offsety,pixelTo);
			AEImagePixelGet(from,x,y,pixelFrom);
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
	image->refcount--;
	if(image->refcount > 0) return;
	free(image->pixels);
	free(image->metastring);
	free(image);
}

AETexture AEImageToTextureWithFlags(AEImage* image,unsigned int flags){
	int channels=AEImageChannelsPerPixelGet(image);
	AETexture texture=SOIL_create_OGL_texture
	(
		image->pixels,
		image->w, image->h, channels,
		SOIL_CREATE_NEW_ID,
		image->flags | flags | SOIL_FLAG_TEXTURE_REPEATS | SOIL_FLAG_INVERT_Y
	);
	AETextureBind(texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	return texture;
}

char* AEImageMetaStringGet(AEImage* image){
	return image->metastring;
}

void AEImageMetaStringSet(AEImage* image,char* string){
	free(image->metastring);
	if(string) string=strdup(string);
	image->metastring=string;
}

unsigned int AEImageTextureFlagsGet(AEImage* image){
	return image->flags;
}

void AEImageTextureFlagsSet(AEImage* image,unsigned int flags){
	image->flags=flags;
}