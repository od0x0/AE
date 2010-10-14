#include "../Core.h"
#include "../SOIL/SOIL.h"
#include <math.h>
#include <string.h>
#include "../Camera.h"

///////////////////////////////////////////////////
//Texture Stuff
AETexture AETextureLoadWithFlags(const char* filename, unsigned int flags){
	//SOIL is EPIC, no denial.
	//Handles EVERYTHING, beautifully too
	AETexture texid = SOIL_load_OGL_texture
		(
			filename,
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			flags | SOIL_FLAG_TEXTURE_REPEATS |  SOIL_FLAG_INVERT_Y
		);
	if(texid==0) printf("Texture loading of %s failed because %s",filename,SOIL_last_result());
	AETextureBind(texid);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	return texid;
}

AETexture AETextureLoadFromMemoryWithFlags(void* data,size_t dataSize, unsigned int flags){
	AETexture texture = SOIL_load_OGL_texture_from_memory(data, dataSize, SOIL_LOAD_AUTO, 0, flags | SOIL_FLAG_TEXTURE_REPEATS |  SOIL_FLAG_INVERT_Y);
	if(texture==0) printf("Texture loading from memory failed because %s",SOIL_last_result());
	AETextureBind(texture);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	return texture;
}

void AETextureDelete(AETexture texture){
	glDeleteTextures(1,&texture);
}
/////////////////////////////////////////////////////
//Utility



static uint64_t AEUInt64TwoToThe(uint64_t exponent){
	uint64_t x=1;
	for(uint64_t i=0;i<exponent;i++) x*=2;
	return x;
}

uint64_t AEUInt64From8Bytes(uint8_t* bytes,bool bigendian){
	uint64_t integer=0;
	for(int i=0;i<8;i++){
		int byteID=i;
		if(bigendian) byteID=8-i-1;
		integer+=(uint64_t)bytes[byteID]*AEUInt64TwoToThe(8*i);
	}
	return integer;
}

void AEUInt64To8Bytes(uint64_t integer,uint8_t* bytes,bool bigendian){
	for(int i=0;i<8;i++) bytes[i]=0;
	uint64_t quotient=integer;
	for(int i=0;i<8;i++){
		int byteID=i;
		if(bigendian) byteID=8-i-1;
		if(quotient==0) break;
		bytes[byteID]=quotient%256;
		quotient/=256;
	}
}

uint32_t AEUInt32From4Bytes(uint8_t* bytes,bool bigendian){
	uint32_t integer=0;
	for(int i=0;i<4;i++){
		int byteID=i;
		if(bigendian) byteID=4-i-1;
		integer+=(uint32_t)bytes[byteID]*AEUInt64TwoToThe(8*i);
	}
	return integer;
}

void AEUInt32To4Bytes(uint32_t integer,uint8_t* bytes,bool bigendian){
	for(int i=0;i<4;i++) bytes[i]=0;
	uint32_t quotient=integer;
	for(int i=0;i<4;i++){
		int byteID=i;
		if(bigendian) byteID=4-i-1;
		if(quotient==0) break;
		bytes[byteID]=quotient%256;
		quotient/=256;
	}
}


void AEError_internal(char* message,const char* function){
	printf("ERROR in %s():\n\t%s\n",function,message);
	fflush(stdout);
	abort();
}
////////////////////////////////////////////////////
//File Stuff
/*
void AEFileInit(AEFile* self){
	memset(self, 0, sizeof(AEFile));
}

void AEFileDeinit(AEFile* self){
	self->close(self);
	memset(self, 0, sizeof(AEFile));
}

void AEFileRead(AEFile* self, void* to, size_t size){
	self->read(self, to, size);
}

void AEFileWrite(AEFile* self, void* from, size_t size){
	self->write(self, from, size);
}

void AEFilePositionSet(AEFile* self, uint64_t position){
	self->positionSet(self, position);
}

uint64_t AEFilePositionGet(AEFile* self){
	return self->positionGet(self);
}

uint64_t AEFileLengthGet(AEFile* self){
	return self->lengthGet(self);
}

bool AEFileIsAtEnd(AEFile* self){
	return self->eof(self);
}

typedef struct{
	uint64_t size;
	uint64_t position;
	void* memory;
}AEFile_Data_Memory;

void AEFile_MemoryRead(AEFile* self, void* to, size_t size){
	AEFile_Data_Memory* data=self->data;
	if((data->position+size) > data->size){
		data->position=data->size;
		return;
	}
	memcpy(to, data->memory+data->position, size);
}

void AEFile_MemoryWrite(AEFile* self, void* from, size_t size){
	AEFile_Data_Memory* data=self->data;
	if((data->position+size) > data->size){
		data->position=data->size;
		return;
	}
	memcpy(data->memory+data->position, from, size);
}

void AEFile_MemoryClose(AEFile* self){
	AEFile_Data_Memory* data=self->data;
	free(data);
	self->data=NULL;
}

void AEFile_MemoryPositionSet(AEFile* self, uint64_t position){
	AEFile_Data_Memory* data=self->data;
	data->position=position;
}

uint64_t AEFile_MemoryPositionGet(AEFile* self){
	AEFile_Data_Memory* data=self->data;
	return data->position;
}

uint64_t AEFile_MemoryLengthGet(AEFile* self){
	AEFile_Data_Memory* data=self->data;
	return data->size;
}

bool AEFile_MemoryEof(AEFile* self){
	AEFile_Data_Memory* data=self->data;
	return data->position==data->size;
}

void AEFileInitWithMemory(AEFile* self, void* memory, size_t size){
	AEFileInit(self);
	AEFile_Data_Memory* data=calloc(1,sizeof(AEFile_Data_Memory));
	data->memory=memory;
	data->size=size;
	self->data=data;
	
	self->read=AEFile_MemoryRead;
	self->write=AEFile_MemoryWrite;
	self->close=AEFile_MemoryClose;
	self->positionGet=AEFile_MemoryPositionGet;
	self->positionSet=AEFile_MemoryPositionSet;
	self->lengthGet=AEFile_MemoryLengthGet;
	self->eof=AEFile_MemoryEof;
}*/

void* AEMBufferBytesGet(AEMBuffer* self, size_t size){
	self->position+=size;
	if(self->position > self->length){
		self->position=self->length;
		return NULL;
	}
	return self->bytes+(self->position-size);
}

void AEMBufferRead(AEMBuffer* self, void* data, size_t size){
	void* from=AEMBufferBytesGet(self, size);
	if(data) memcpy(data, from, size);
}

void AEMBufferWrite(AEMBuffer* self, void* data, size_t size){
	size_t oldPosition=self->position;
	void* to=AEMBufferBytesGet(self, size);
	if(not to){
		self->position=oldPosition;
		self->length += size;
		self->allocated = self->length * 1.2;
		self->bytes=realloc(self->bytes, self->allocated);
	}
	if(data and to) memcpy(to, data, size);
}

void AEMBufferPositionSet(AEMBuffer* self, size_t position){
	self->position=position;
}

size_t AEMBufferPositionGet(AEMBuffer* self){
	return self->position;
}

size_t AEMBufferLengthGet(AEMBuffer* self){
	return self->length;
}

////////////////////////////////////////////////////
//View Stuff
AEContext AEContextActive={
	.w=800,
	.h=500,
	.r=8,
	.g=8,
	.b=8,
	.a=8,
	.stencil=8,
	.depth=8,
	.inFullscreen=0
};

void AEContextActiveSet(AEContext* context){
	AEContextActive=*context;
}

AEContext* AEContextActiveGet(void){
	return &AEContextActive;
}

void AEInit(AEContext* context,char* title,int w,int h){
	if(not context) context=AEContextActiveGet();
	
	if(context->init==NULL || context->refresh==NULL || context->pollinput==NULL || context->swapbuffers==NULL || context->deinit==NULL || context->seconds==NULL) AEError("AEContext function pointers need to all be filled before you can use the engine.");
	
	context->w=w;context->h=h;
	context->init(context,title);
	
	AECamera* cam=AECameraActiveGet();
	AECameraViewportSet(cam,context->w,context->h);
	
	//Because it also affects the modelview matrix unfortunately
	glPushMatrix();
	AECameraBind(cam);
	glPopMatrix();
	
	glClearColor(0,0,0,1);
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable(GL_DEPTH_TEST);
	glEnable( GL_TEXTURE_2D );
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

static void AEDefaultPerframeFunc(float step){}

void AEStart(AEContext* context,void (*perframe)(float)){
	//0 is a magical number, simply acts as the default
	if(perframe==NULL) perframe=AEDefaultPerframeFunc;
	
	if(not context) context=AEContextActiveGet();
	
	float now=context->seconds(context);
	float then=now;
	while(context->pollinput(context)){
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //This is default
		//glClear(GL_DEPTH_BUFFER_BIT); // Use this if you always use a skybox
		AECameraBind(AECameraActiveGet());
		
		now=context->seconds(context);
		(*perframe)((now-then));
		then=now;
		//Sounds....  Poetic
		
		context->swapbuffers(context);
	}
	context->deinit(context);
}

void AEQuit(AEContext* context){
	if(not context) context=AEContextActiveGet();
	context->deinit(context);
}