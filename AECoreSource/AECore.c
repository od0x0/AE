#include "AECore.h"
#include "SOIL.h"
#include <math.h>
#include <string.h>
#include "AECamera.h"

char* AEStringDuplicate(const char* string){
	if(not string) return NULL;
	size_t length=strlen(string);
	char* newstring=malloc(length+1);
	memcpy(newstring, string, length+1);
	return newstring;
}

///////////////////////////////////////////////////
//Texture Stuff
AETexture AETextureLoadWithFlags(const char* filename, AETextureFlag flags){
	//SOIL is EPIC, no denial.
	//Handles EVERYTHING, beautifully too
	AETexture texture = SOIL_load_OGL_texture
		(
			filename,
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			flags | SOIL_FLAG_INVERT_Y
		);
	if(texture==0){
		const char* result=SOIL_last_result();
		char message[strlen(result)+strlen(filename)+strlen(": ")+1];
		sprintf(message, "%s: %s", result, filename);
		AEError(message);
	}
	//AETextureBind(texture);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	return texture;
}

AETexture AETextureLoadFromMemoryWithFlags(void* data,size_t dataSize, AETextureFlag flags){
	AETexture texture = SOIL_load_OGL_texture_from_memory(data, dataSize, SOIL_LOAD_AUTO, 0, flags |  SOIL_FLAG_INVERT_Y);
	if(texture==0) printf("Texture loading from memory failed because %s",SOIL_last_result());
	//AETextureBind(texture);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
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

static uint64_t AEUInt64From8Bytes(uint8_t* bytes,bool bigendian){
	uint64_t integer=0;
	for(int i=0;i<8;i++){
		int byteID=i;
		if(bigendian) byteID=8-i-1;
		integer+=(uint64_t)bytes[byteID]*AEUInt64TwoToThe(8*i);
	}
	return integer;
}

static void AEUInt64To8Bytes(uint64_t integer,uint8_t* bytes,bool bigendian){
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

static uint32_t AEUInt32From4Bytes(uint8_t* bytes,bool bigendian){
	uint32_t integer=0;
	for(int i=0;i<4;i++){
		int byteID=i;
		if(bigendian) byteID=4-i-1;
		integer+=(uint32_t)bytes[byteID]*(uint32_t)AEUInt64TwoToThe(8*i);
	}
	return integer;
}

static void AEUInt32To4Bytes(uint32_t integer,uint8_t* bytes,bool bigendian){
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

uint64_t AEHostU64FromNet(uint64_t netu64){
	return AEUInt64From8Bytes((uint8_t*)&netu64, true);
}

uint64_t AENetU64FromHost(uint64_t hostu64){
	AEUInt64To8Bytes(hostu64, (uint8_t*)&hostu64, true);
	return hostu64;
}

uint32_t AEHostU32FromNet(uint32_t netu32){
	return AEUInt32From4Bytes((uint8_t*)&netu32, true);
}

uint32_t AENetU32FromHost(uint32_t hostu32){
	AEUInt32To4Bytes(hostu32, (uint8_t*)&hostu32, true);
	return hostu32;
}
///////////////////////////////////////////////////
/*
#include "setjmp.h"
struct AEException{
	jmp_buf jumpbuffer;
	char* message;
	const char* functionOfOrigin;
	const char* type;
	bool wasThrown;
	AEException* last;
};

#ifndef GNUC
#define __thread
#endif

static __thread AEException* AEExceptionsRoot=NULL;

AEException* AEExceptionsRootGet(void){
	return AEExceptionsRoot;
}

AEException* AEExceptionsPush(void){
	AEException* exception=calloc(1, sizeof(AEException));
	exception->last=AEExceptionsRoot;
	AEExceptionsRoot=exception;
	if(setjmp(exception->jumpbuffer)) return exception;
	return NULL;
}

void AEExceptionsPop(void){
	if(AEExceptionsRoot->wasThrown){
		const char* type=AEExceptionsRoot->type;
		if(type==NULL) type = "Exception";
		fprintf(stderr,"[%s] %s():%s\n",type,AEExceptionsRoot->functionOfOrigin,AEExceptionsRoot->message);
		AEException* e=AEExceptionsRoot;
		AEExceptionsRoot=e->last;
		free(e->message);
		free(e);
		//abort is very magical, it also flushes all streams, so we are saved the 3 lines of typing
		abort();
	}
	AEException* e=AEExceptionsRoot;
	AEExceptionsRoot=e->last;
	free(e->message);
	free(e);
}

void AEExceptionsThrow_internal(const char* type, char* message, const char* functionOfOrigin){

	if(not AEExceptionsRoot){
		if(type==NULL) type = "Exception";
		fprintf(stderr,"[%s] %s():%s\n",type,functionOfOrigin,message);
		//abort is very magical, it also flushes all streams, so we are saved the 3 lines of typing
		abort();
	}
	
	AEExceptionsRoot->type=type;
	AEExceptionsRoot->wasThrown=true;
	if(message) message=AEStringDuplicate(message);
	AEExceptionsRoot->message=message;
	AEExceptionsRoot->functionOfOrigin=functionOfOrigin;
	longjmp(AEExceptionsRoot->jumpbuffer, 1);
}

AEException* AEExceptionsCatch(const char* type){
	if(not AEExceptionsRoot->wasThrown) return NULL;
	if(type==NULL) return AEExceptionsRoot;
	if(strcmp(type, AEExceptionsRoot->type)==0) return AEExceptionsRoot;
	return NULL;
}

#undef __thread*/

////////////////////////////////////////////////////
//File Stuff


void AEMBufferInit(AEMBuffer* self){
	memset(self, 0, sizeof(AEMBuffer));
}

void AEMBufferDeinit(AEMBuffer* self){
	if (not self) return;
	if(self->file) fclose(self->file);
	free(self->bytes);
	memset(self, 0, sizeof(AEMBuffer));
}
void* AEMBufferBytesGet(AEMBuffer* self, size_t size){
	if(self->file) {
		return NULL;
	}
	self->position+=size;
	if(self->position > self->length){
		self->position=self->length;
		return NULL;
	}
	return (void*) (((char*)self->bytes)+(self->position-size));
}

void AEMBufferRead(AEMBuffer* self, void* data, size_t size){
	if(self->file) {
		fread(data, 1, size, self->file);
		return;
	}
	void* from=AEMBufferBytesGet(self, size);
	if(data and from) memcpy(data, from, size);
}

void AEMBufferWrite(AEMBuffer* self, void* data, size_t size){
	if(self->file) {
		fwrite(data, 1, size, self->file);
		return;
	}
	size_t oldPosition=self->position;
	void* to=AEMBufferBytesGet(self, size);
	if(not to){
		self->position=oldPosition;
		self->length += size;
		self->allocated = self->length + (self->length/10)*2;
		self->bytes=realloc(self->bytes, self->allocated);
		to=AEMBufferBytesGet(self, size);
	}
	if(data and to) memcpy(to, data, size);
}

void AEMBufferSeek(AEMBuffer* self, long offset, int from){
	if(self->file) {
		fseek(self->file, offset, from);
		return;
	}
	switch (from) {
		case SEEK_CUR:
			AEMBufferSetPosition(self, self->position+offset);
			break;
		case SEEK_END:
			AEMBufferSetPosition(self, (self->length-1)+offset);
			break;
		case SEEK_SET:
			AEMBufferSetPosition(self, offset);
			break;
		default:
			AEError("Invalid Argument for 'from'.");
			break;
	}
}
void AEMBufferSetPosition(AEMBuffer* self, size_t position){
	self->position=position;
}

size_t AEMBufferGetPosition(AEMBuffer* self){
	if(self->file) return ftell(self->file);
	return self->position;
}

size_t AEMBufferGetLength(AEMBuffer* self){
	return self->length;
}

////////////////////////////////////////////////////
//View Stuff
AEContext AEContextsActive={
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

void AEContextsSetActive(AEContext* context){
	AEContextsActive=*context;
}

AEContext* AEContextsGetActive(void){
	return &AEContextsActive;
}

void AEContextOpen(AEContext* context,const char* title,int w,int h){
	if(not context) context=AEContextsGetActive();
	
	if(context->open==NULL || context->refresh==NULL || context->pollInput==NULL || context->swapBuffers==NULL || context->close==NULL || context->seconds==NULL) AEError("AEContext function pointers need to all be filled before you can use the engine.");
	
	context->w=w;context->h=h;
	context->open(context, title);
	context->clearedBuffers=GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT;
	AECamera* cam=AECamerasGetActive();
	AECameraSetViewport(cam,context->w,context->h);
	
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
	
	context->fixedUpdateFrameRateMax=60;
	context->fixedUpdateFrameRateMin=15;
}

static void AEDefaultPerframeFunc(AEContext* context, double step){}

void AEContextRun(AEContext* context){
	if(not context) context=AEContextsGetActive();
	//0 is a magical number, simply acts as the default
	if(context->frameUpdate==NULL) context->frameUpdate=AEDefaultPerframeFunc;
	
	double lastFrameTime = context->seconds(context);
	double cyclesLeftOver = 0.0;
	
	double updateInterval = 1.0 / context->fixedUpdateFrameRateMax;
	double maxCyclesPerFrame = context->fixedUpdateFrameRateMax / context->fixedUpdateFrameRateMin;
	
	double now=context->seconds(context);
	double then=now;
	while(context->pollInput(context)){
		//Some code borrowed from sacredsoftware.com (written by Alex Diener, aka ThemsAllTook)
		if(context->fixedUpdate){
			double currentTime=0;
			double updateIterations=0;
	
			currentTime = context->seconds(context);
			updateIterations = ((currentTime - lastFrameTime) + cyclesLeftOver);
	
			if (updateIterations > (maxCyclesPerFrame * updateInterval)) {
				updateIterations = (maxCyclesPerFrame * updateInterval);
			}
	
			while (updateIterations > updateInterval) {
				updateIterations -= updateInterval;
				context->fixedUpdate(context, updateInterval);
			}
	
			cyclesLeftOver = updateIterations;
			lastFrameTime = currentTime;
		}
		if(context->clearedBuffers) glClear(context->clearedBuffers);
		AECameraBind(AECamerasGetActive());
		
		now=context->seconds(context);
		context->frameUpdate(context, (now-then));
		then=now;
		//Sounds....  Poetic
		
		context->swapBuffers(context);
	}
	AEContextClose(context);
}

void AEContextClose(AEContext* context){
	if(not context) context=AEContextsGetActive();
	if(context->close) context->close(context);
}