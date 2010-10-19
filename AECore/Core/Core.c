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
	if(texid==0){
		const char* result=SOIL_last_result();
		char message[strlen(result)+strlen(filename)+strlen(": ")+1];
		sprintf(message, "%s: %s", result, filename);
		AEError(message);
	}
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
	if(message) message=strdup(message);
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
	free(self->bytes);
	memset(self, 0, sizeof(AEMBuffer));
}
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
	if(data and from) memcpy(data, from, size);
}

void AEMBufferWrite(AEMBuffer* self, void* data, size_t size){
	size_t oldPosition=self->position;
	void* to=AEMBufferBytesGet(self, size);
	if(not to){
		self->position=oldPosition;
		self->length += size;
		self->allocated = self->length * 1.2;
		self->bytes=realloc(self->bytes, self->allocated);
		to=AEMBufferBytesGet(self, size);
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

void AEContextInit(AEContext* context,char* title,int w,int h){
	if(not context) context=AEContextActiveGet();
	
	if(context->init==NULL || context->refresh==NULL || context->pollinput==NULL || context->swapbuffers==NULL || context->deinit==NULL || context->secondsget==NULL) AEError("AEContext function pointers need to all be filled before you can use the engine.");
	
	context->w=w;context->h=h;
	context->init(context,title, context->initarg);
	
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
	
	context->fixedUpdateFrameRateMax=60;
	context->fixedUpdateFrameRateMin=15;
}

static void AEDefaultPerframeFunc(AEContext* context, double step, void* arg){}

void AEContextRun(AEContext* context){
	if(not context) context=AEContextActiveGet();
	//0 is a magical number, simply acts as the default
	if(context->frameupdate==NULL) context->frameupdate=AEDefaultPerframeFunc;
	
	double lastFrameTime = context->secondsget(context, context->secondsgetarg);
	double cyclesLeftOver = 0.0;
	
	double updateInterval = 1.0 / context->fixedUpdateFrameRateMax;
	double maxCyclesPerFrame = context->fixedUpdateFrameRateMax / context->fixedUpdateFrameRateMin;
	
	double now=context->secondsget(context, context->secondsgetarg);
	double then=now;
	while(context->pollinput(context, context->pollinputarg)){
		//Some code borrowed from sacredsoftware.com (code written by Alex Diener, aka ThemsAllTook)
		if(context->fixedupdate){
			double currentTime=0;
			double updateIterations=0;
	
			currentTime = context->secondsget(context, context->secondsgetarg);
			updateIterations = ((currentTime - lastFrameTime) + cyclesLeftOver);
	
			if (updateIterations > (maxCyclesPerFrame * updateInterval)) {
				updateIterations = (maxCyclesPerFrame * updateInterval);
			}
	
			while (updateIterations > updateInterval) {
				updateIterations -= updateInterval;
				context->fixedupdate(context, updateInterval, context->fixedupdatearg);
			}
	
			cyclesLeftOver = updateIterations;
			lastFrameTime = currentTime;
		}
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); //This is default
		//glClear(GL_DEPTH_BUFFER_BIT); // Use this if you always use a skybox
		AECameraBind(AECameraActiveGet());
		
		now=context->secondsget(context, context->secondsgetarg);
		context->frameupdate(context, (now-then),context->frameupdatearg);
		then=now;
		//Sounds....  Poetic
		
		context->swapbuffers(context, context->swapbuffersarg);
	}
	AEContextDeinit(context);
}

/*
#define MAXIMUM_FRAME_RATE 120
#define MINIMUM_FRAME_RATE 15
#define UPDATE_INTERVAL (1.0 / MAXIMUM_FRAME_RATE)
#define MAX_CYCLES_PER_FRAME (MAXIMUM_FRAME_RATE / MINIMUM_FRAME_RATE)

void runGame() {
  static double lastFrameTime = 0.0;
  static double cyclesLeftOver = 0.0;
  double currentTime;
  double updateIterations;
  
  currentTime = GetCurrentTime();
  updateIterations = ((currentTime - lastFrameTime) + cyclesLeftOver);
  
  if (updateIterations > (MAX_CYCLES_PER_FRAME * UPDATE_INTERVAL)) {
    updateIterations = (MAX_CYCLES_PER_FRAME * UPDATE_INTERVAL);
  }
  
  while (updateIterations > UPDATE_INTERVAL) {
    updateIterations -= UPDATE_INTERVAL;
    
    updateGame();
  }
  
  cyclesLeftOver = updateIterations;
  lastFrameTime = currentTime;
  
  drawScene();
}
*/

void AEContextDeinit(AEContext* context){
	if(not context) context=AEContextActiveGet();
	if(context->deinit) context->deinit(context, context->deinitarg);
}

void AEContextCallbackSet(AEContext* context, int funcname, void* func, void* arg){
	if(not context) context=AEContextActiveGet();
	switch (funcname) {
		case AEContextCallbackInit:
			context->init=func;
			context->initarg=arg;
			break;
		case AEContextCallbackRefresh:
			context->refresh=func;
			context->refresharg=arg;
			break;
		case AEContextCallbackPollInput:
			context->pollinput=func;
			context->pollinputarg=arg;
			break;
		case AEContextCallbackSwapBuffers:
			context->swapbuffers=func;
			context->swapbuffersarg=arg;
			break;
		case AEContextCallbackDeinit:
			context->deinit=func;
			context->deinitarg=arg;
			break;
		case AEContextCallbackSecondsGet:
			context->secondsget=func;
			context->secondsgetarg=arg;
			break;
		case AEContextCallbackFixedUpdate:
			context->fixedupdate=func;
			context->fixedupdatearg=arg;
			break;
		case AEContextCallbackFrameUpdate:
			context->frameupdate=func;
			context->frameupdatearg=arg;
			break;
		default:
			AEError("Unknown callback set.");
			break;
	}
}

void* AEContextCallbackGet(AEContext* context, int funcname,void** arg){
	if(not context) context=AEContextActiveGet();
	switch (funcname) {
		case AEContextCallbackInit:
			if(arg) *arg=context->initarg;
			return context->init;
		case AEContextCallbackRefresh:
			if(arg) *arg=context->refresharg;
			return context->refresh;
		case AEContextCallbackPollInput:
			if(arg) *arg=context->pollinputarg;
			return context->pollinput;
		case AEContextCallbackSwapBuffers:
			if(arg) *arg=context->swapbuffersarg;
			return context->swapbuffers;
		case AEContextCallbackDeinit:
			if(arg) *arg=context->deinitarg;
			return context->deinit;
		case AEContextCallbackSecondsGet:
			if(arg) *arg=context->secondsgetarg;
			return context->secondsget;
		case AEContextCallbackFixedUpdate:
			if(arg) *arg=context->fixedupdatearg;
			return context->fixedupdate;
		case AEContextCallbackFrameUpdate:
			if(arg) *arg=context->frameupdatearg;
			return context->frameupdate;
		default:
			AEError("Unknown callback set.");
			break;
	}
	return NULL;
}