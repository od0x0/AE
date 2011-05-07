#include "AEWindow.h"

typedef struct AEViewVFCData AEViewVFCData;

struct AEView{
	AEVec3 position;
	AEVec3 rotation;
	//float transform[16];
	struct {
		int x, y, w, h;
	}viewport;
	struct {
		float near, far, fov;
	}projection;
	GLbitfield clearedBuffers;
	AEViewCallbackRenderFunc renderFunc;
	AEViewCallbackDeinitFunc deinitFunc;
	void* userdata;
	char* name;
	bool onlyDoPartialUpdate;
	AEWindow* window;
	AEViewVFCData* vfcData;
};

struct AEViewVFCData{
	struct{AEVec3 x,y,z;} axes;
	float tangentOfFov;
	AEVec2 sphereFactor;
};

void AEViewSetVFC(AEView* self, bool on){
	if(not on) {
		free(self->vfcData);
		return;
	}
	if(not self->vfcData) self->vfcData=calloc(1, sizeof(AEViewVFCData));
	AEQuat q=AEQuatFromEuler(self->rotation);
	self->vfcData->axes.z=AEQuatMulVec3(q, AEVec3From3(0, 0, -1));
	self->vfcData->axes.y=AEQuatMulVec3(q, AEVec3From3(0, +1, 0));
	self->vfcData->axes.x=AEVec3Cross(self->vfcData->axes.z, self->vfcData->axes.y);
	self->vfcData->tangentOfFov=tanf(AEPiOver180*(self->projection.fov/2));
	self->vfcData->sphereFactor.y=1.0/cosf(AEPiOver180*self->projection.fov);
	self->vfcData->sphereFactor.x=1.0/cosf(atanf(self->vfcData->tangentOfFov*(self->viewport.w/(float)self->viewport.h)));
}

bool AEViewGetVFC(AEView* self){
	return self->vfcData not_eq NULL;
}

bool AEViewVFCTestSphere(AEView* self, float x, float y, float z, float r){
	if(not self->vfcData) return false;
	//Mostly taken from Lighthouse3d, so the code is pretty horrid
	AEViewVFCData* vfcData=self->vfcData;

	AEVec3 v = AEVec3Sub(AEVec3From3(x, y, z), self->position);
	
	float az = AEVec3Dot(v, vfcData->axes.z);
	if (az > self->projection.far + r || az < self->projection.near-r) return false;

	float ax = AEVec3Dot(v, vfcData->axes.x);
	float zz1 = az * vfcData->tangentOfFov * (self->viewport.w/(float)self->viewport.h);
	float d1 = vfcData->sphereFactor.x * r;
	if (ax > zz1+d1 || ax < -zz1-d1) return false;
	
	float ay = AEVec3Dot(v, vfcData->axes.y);
	float zz2 = az * vfcData->tangentOfFov;
	float d2 = vfcData->sphereFactor.y * r;
	if (ay > zz2+d2 || ay < -zz2-d2) return false;
	
	if (az > self->projection.far - r || az < self->projection.near+r) return true;
	if (ay > zz2-d2 || ay < -zz2+d2) return true;
	if (ax > zz1-d1 || ax < -zz1+d1) return true;
	return true;
}

void AEViewSetRenderFunc(AEView* self, AEViewCallbackRenderFunc func){
	self->renderFunc=func;
}

AEViewCallbackRenderFunc AEViewGetRenderFunc(AEView* self){
	return self->renderFunc;
}

void AEViewSetDeinitFunc(AEView* self, AEViewCallbackDeinitFunc func){
	self->deinitFunc=func;
}

AEViewCallbackDeinitFunc AEViewGetDeinitFunc(AEView* self){
	return self->deinitFunc;
}

void AEViewSetUserdata(AEView* self, void* to){
	self->userdata=to;
}

void* AEViewGetUserdata(AEView* self){
	return self->userdata;
}

AEWindow* AEViewGetWindow(AEView* self){
	return self->window;
}

void AEViewGetPosition(AEView* self, AEVec3* v3){
	*v3=self->position;
}

void AEViewSetPosition(AEView* self, float x, float y, float z){
	self->position=AEVec3From3(x, y, z);
}

void AEViewGetRotation(AEView* self, AEVec3* v3){
	*v3=self->rotation;
}

void AEViewSetRotation(AEView* self, float x, float y, float z){
	self->rotation=AEVec3From3(x, y, z);
	if(AEViewGetVFC(self)) AEViewSetVFC(self, true);//Refresh the VFC state
}

void AEViewSetProjection(AEView* self, float near, float far, float fov){
	if(near not_eq AEInfinity) self->projection.near=near;
	if(far not_eq AEInfinity) self->projection.far=far;
	if(fov not_eq AEInfinity) self->projection.fov=fov;
	self->onlyDoPartialUpdate=false;
	if(AEViewGetVFC(self)) AEViewSetVFC(self, true);
}

void AEViewGetProjection(AEView* self, float* near, float* far, float* fov){
	if(near) *near=self->projection.near;
	if(far) *far=self->projection.far;
	if(fov) *fov=self->projection.fov;
}

void AEViewSetViewport(AEView* self, int x, int y, int w, int h){
	self->viewport.x=x;
	self->viewport.y=y;
	if(w > 0) self->viewport.w=w;
	if(h > 0) self->viewport.h=h;
	self->onlyDoPartialUpdate=false;
	if(AEViewGetVFC(self)) AEViewSetVFC(self, true);
}

void AEViewGetViewport(AEView* self, int* x, int* y, int* w, int* h){
	if(x) *x=self->viewport.x;
	if(y) *y=self->viewport.y;
	if(w) *w=self->viewport.w;
	if(h) *h=self->viewport.h;
}

static void AEViewRender(AEView* self){
	if(self->onlyDoPartialUpdate==false){
		self->onlyDoPartialUpdate=true;
		
		int x=self->viewport.x;
		int y=self->viewport.y;
		int w=self->viewport.w;
		int h=self->viewport.h;
		float fov=self->projection.fov;
		float far=self->projection.far;
		float near=self->projection.near;
		
		glViewport(x,y,w,h);
		glMatrixMode(GL_PROJECTION);
			if(fov) {
				float m[16];
				AEMatrixFromPerspective(m, fov, (float)w/(float)h, near, far);
				glLoadMatrixf(m);
			}
			else {
				glLoadIdentity();
				#ifdef AEiOS
				glOrthof(x,w,y,h,near,far);
				#else
				glOrtho(x,w,y,h,near,far);
				#endif
			}
		glMatrixMode(GL_MODELVIEW);
	}
	//float m[16];
	glLoadIdentity();
	
	glRotatef(-self->rotation.x,1,0,0);
	glRotatef(-self->rotation.y,0,1,0);
	//glMatrixMode(GL_PROJECTION);
	glRotatef(-self->rotation.z,0,0,1);
	//glMatrixMode(GL_MODELVIEW);
	glTranslatef(-self->position.x,-self->position.y,-self->position.z);
	self->renderFunc(self);
	//glMatrixMode(GL_PROJECTION);
	//glRotatef(self->rotation.z,0,0,1);
	//glMatrixMode(GL_MODELVIEW);
}

struct AEWindow{
	unsigned int w, h;
	void* userdata;
	AEWindowFormat format;
	AEWindowContextCallbacks contextCallbacks;
	AEArray(AEView*) views;
	GLbitfield clearedBuffers;
	AEWindowCallbackFixedUpdateFunc fixedUpdate;
	double fixedUpdateFrameRateMax;
	double fixedUpdateFrameRateMin;
	AEWindowCallbackFrameUpdateFunc frameUpdate;
};

AEWindow* AEWindowNew(void){
	AEWindow* self=calloc(1, sizeof(AEWindow));
	AEArrayInit(& self->views);
	self->clearedBuffers=GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT;
	static const AEWindowFormat windowFormat={
		.colorBits={8,8,8,8},
		.stencilBits=8,
		.depthBits=8,
		.inFullscreen=false,
		.multisample=0
	};
	self->format=windowFormat;
	self->fixedUpdateFrameRateMax=60;
	self->fixedUpdateFrameRateMin=15;
	return self;
}

void AEWindowDelete(AEWindow* self){
	if(not self) return;
	size_t viewCount=AEArrayLength(& self->views);
	AEView** views=AEArrayAsCArray(& self->views);
	for (size_t i=0; i<viewCount; i++) {
		AEView* view=views[i];
		if(view->deinitFunc) view->deinitFunc(view);
	}
	AEArrayDeinit(& self->views);
	free(self);
}

AEView* AEWindowCreateView(AEWindow* self, const char* name){
	AEView* view=calloc(1, sizeof(AEView));
	AEViewSetProjection(view, 1, 3000, 60);
	AEViewSetViewport(view, 0, 0, self->w, self->h);
	view->window=self;
	view->name=AECloneString(name);
	AEArrayAdd(& self->views, view);
	return view;
}

void AEWindowDestroyView(AEWindow* self, AEView* view){
	if(view->deinitFunc) view->deinitFunc(view);
	AEArrayRemoveBytes(& self->views, & view);
	AEViewSetVFC(view, false);
	free(view->name);
	free(view);
}

AEView* AEWindowLookupView(AEWindow* self, const char* name){
	size_t viewCount=AEArrayLength(& self->views);
	AEView** views=AEArrayAsCArray(& self->views);
	for (size_t i=0; i<viewCount; i++) {
		AEView* view=views[i];
		if(strcmp(view->name, name)==0) return view;
	}
	return NULL;
}

void AEWindowGetFormat(AEWindow* self, AEWindowFormat* format){
	*format=self->format;
}

void AEWindowSetFormat(AEWindow* self, const AEWindowFormat* format){
	self->format=*format;
}

void AEWindowSetFixedUpdateFunc(AEWindow* self, AEWindowCallbackFixedUpdateFunc func, double minFrameRate, double maxFrameRate){
	self->fixedUpdate=func;
	self->fixedUpdateFrameRateMin=minFrameRate;
	self->fixedUpdateFrameRateMax=maxFrameRate;
}

AEWindowCallbackFixedUpdateFunc AEWindowGetFixedUpdateFunc(AEWindow* self, double* minFrameRate, double* maxFrameRate){
	if(minFrameRate) *minFrameRate=self->fixedUpdateFrameRateMin;
	if(maxFrameRate) *maxFrameRate=self->fixedUpdateFrameRateMax;
	return self->fixedUpdate;
}

void AEWindowSetFrameUpdateFunc(AEWindow* self, AEWindowCallbackFrameUpdateFunc func){
	self->frameUpdate=func;
}

AEWindowCallbackFrameUpdateFunc AEWindowGetFrameUpdateFunc(AEWindow* self){
	return self->frameUpdate;
}

void AEWindowGetContextCallbacks(AEWindow* self, AEWindowContextCallbacks* callbacks){
	*callbacks=self->contextCallbacks;
}

void AEWindowSetContextCallbacks(AEWindow* self, const AEWindowContextCallbacks* callbacks){
	self->contextCallbacks=*callbacks;
}

void AEWindowGetSize(AEWindow* self, int* w, int* h){
	if(w) *w=self->w;
	if(h) *h=self->h;
}

void AEWindowSetClearedBuffers(AEWindow* self, GLbitfield to){
	self->clearedBuffers=to;
}

GLbitfield AEWindowGetClearedBuffers(AEWindow* self){
	return self->clearedBuffers;
}

void AEWindowSetUserdata(AEWindow* self, void* to){
	self->userdata=to;
}

void* AEWindowGetUserdata(AEWindow* self){
	return self->userdata;
}

void AEWindowOpen(AEWindow* self, const char* title, int w, int h){
	self->w=w;
	self->h=h;
	self->contextCallbacks.open(self, title);
	
	//self->clearedBuffers=GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT;
	/*AECamera* cam=self->camera;
	AECameraSetViewport(cam,0,0,self->w,self->h);
	
	//Because it also affects the modelview matrix unfortunately
	glPushMatrix();
	AECameraBind(cam);
	glPopMatrix();*/
	
	glClearColor(0,0,0,1);
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glEnable(GL_DEPTH_TEST);
	glEnable( GL_TEXTURE_2D );
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	
	
	/*
	size_t viewCount=AEArrayLength(& self->views);
	AEView** views=AEArrayAsCArray(& self->views);
	for (size_t i=0; i<viewCount; i++) {
		
	}*/
}

void AEWindowRun(AEWindow* self){
	double lastFrameTime = self->contextCallbacks.seconds(self);
	double cyclesLeftOver = 0.0;
	
	double updateInterval = 1.0 / self->fixedUpdateFrameRateMax;
	double maxCyclesPerFrame = self->fixedUpdateFrameRateMax / self->fixedUpdateFrameRateMin;
	
	double now=self->contextCallbacks.seconds(self);
	double then=now;
	AEView* lastViewRendered=NULL;
	while(self->contextCallbacks.pollInput(self)){
		//Some code borrowed from sacredsoftware.com (written by Alex Diener, aka ThemsAllTook)
		if(self->fixedUpdate){
			double currentTime=0;
			double updateIterations=0;
	
			currentTime = self->contextCallbacks.seconds(self);
			updateIterations = ((currentTime - lastFrameTime) + cyclesLeftOver);
	
			if (updateIterations > (maxCyclesPerFrame * updateInterval)) {
				updateIterations = (maxCyclesPerFrame * updateInterval);
			}
	
			while (updateIterations > updateInterval) {
				updateIterations -= updateInterval;
				self->fixedUpdate(self, updateInterval);
			}
	
			cyclesLeftOver = updateIterations;
			lastFrameTime = currentTime;
		}
		
		if(self->frameUpdate){
			now=self->contextCallbacks.seconds(self);
			self->frameUpdate(self, (now-then));
			then=now;
		}
		//Sounds....  Poetic
		
		if(self->clearedBuffers) glClear(self->clearedBuffers);
		
		size_t viewCount=AEArrayLength(& self->views);
		AEView** views=AEArrayAsCArray(& self->views);
		for (size_t i=0; i<viewCount; i++) {
			AEView* view=views[i];
			if(view->renderFunc){
				if(lastViewRendered not_eq view and lastViewRendered) lastViewRendered->onlyDoPartialUpdate=false;
				AEViewRender(view);
				lastViewRendered=view;
			}
		}
		
		self->contextCallbacks.swapBuffers(self);
	}
}

void AEWindowClose(AEWindow* self){
	if(self->contextCallbacks.close) self->contextCallbacks.close(self);
}
