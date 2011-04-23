#pragma once
#include "AECore.h"
#include "AEArray.h"

typedef struct AEWindow AEWindow;
typedef void (*AEWindowContextCallbackOpenFunc)(AEWindow* self, const char* title);
typedef void (*AEWindowContextCallbackRefreshFunc)(AEWindow* self);
typedef int (*AEWindowContextCallbackPollInputFunc)(AEWindow* self);
typedef void (*AEWindowContextCallbackSwapBuffersFunc)(AEWindow* self);
typedef void (*AEWindowContextCallbackCloseFunc)(AEWindow* self);
typedef double (*AEWindowContextCallbackSecondsFunc)(AEWindow* self);
typedef void (*AEWindowCallbackFixedUpdateFunc)(AEWindow* self, double secondsSinceLastCall);
typedef void (*AEWindowCallbackFrameUpdateFunc)(AEWindow* self, double secondsSinceLastCall);

typedef struct AEView AEView;
typedef void (*AEViewCallbackRenderFunc)(AEView* self);
typedef void (*AEViewCallbackDeinitFunc)(AEView* self);

void AEViewSetRenderFunc(AEView* self, AEViewCallbackRenderFunc func);
AEViewCallbackRenderFunc AEViewGetRenderFunc(AEView* self);

void AEViewSetDeinitFunc(AEView* self, AEViewCallbackDeinitFunc func);
AEViewCallbackDeinitFunc AEViewGetDeinitFunc(AEView* self);

void AEViewSetUserdata(AEView* self, void* to);
void* AEViewGetUserdata(AEView* self);

AEWindow* AEViewGetWindow(AEView* self);

void AEViewGetPosition(AEView* self, AEVec3* v3);
void AEViewSetPosition(AEView* self, float x, float y, float z);
void AEViewGetRotation(AEView* self, AEVec3* v3);
void AEViewSetRotation(AEView* self, float x, float y, float z);

void AEViewSetProjection(AEView* self, float near, float far, float fov);
void AEViewGetProjection(AEView* self, float* near, float* far, float* fov);
void AEViewSetViewport(AEView* self, int x, int y, int w, int h);
void AEViewGetViewport(AEView* self, int* x, int* y, int* w, int* h);

void AEViewSetVFC(AEView* self, bool on);
bool AEViewGetVFC(AEView* self);
bool AEViewVFCTestSphere(AEView* self, float x, float y, float z, float r);

typedef struct {
	AERGBAub colorBits;
	unsigned char stencilBits, depthBits;
	unsigned char inFullscreen, multisample;
}AEWindowFormat;

typedef struct {
	AEWindowContextCallbackOpenFunc open;
	AEWindowContextCallbackRefreshFunc refresh;
	AEWindowContextCallbackPollInputFunc pollInput;
	AEWindowContextCallbackSwapBuffersFunc swapBuffers;
	AEWindowContextCallbackCloseFunc close;
	AEWindowContextCallbackSecondsFunc seconds;
}AEWindowContextCallbacks;

AEWindow* AEWindowNew(void);
void AEWindowDelete(AEWindow* self);

AEView* AEWindowCreateView(AEWindow* self, const char* name);
void AEWindowDestroyView(AEWindow* self, AEView* view);
AEView* AEWindowLookupView(AEWindow* self, const char* name);


void AEWindowGetFormat(AEWindow* self, AEWindowFormat* format);
void AEWindowSetFormat(AEWindow* self, const AEWindowFormat* format);

void AEWindowSetFixedUpdateFunc(AEWindow* self, AEWindowCallbackFixedUpdateFunc func, double minFrameRate, double maxFrameRate);
AEWindowCallbackFixedUpdateFunc AEWindowGetFixedUpdateFunc(AEWindow* self, double* minFrameRate, double* maxFrameRate);

void AEWindowSetFrameUpdateFunc(AEWindow* self, AEWindowCallbackFrameUpdateFunc func);
AEWindowCallbackFrameUpdateFunc AEWindowGetFrameUpdateFunc(AEWindow* self);

void AEWindowGetContextCallbacks(AEWindow* self, AEWindowContextCallbacks* callbacks);
void AEWindowSetContextCallbacks(AEWindow* self, const AEWindowContextCallbacks* callbacks);

void AEWindowGetSize(AEWindow* self, int* w, int* h);

void AEWindowSetClearedBuffers(AEWindow* self, GLbitfield to);
GLbitfield AEWindowGetClearedBuffers(AEWindow* self);

void AEWindowSetUserdata(AEWindow* self, void* to);
void* AEWindowGetUserdata(AEWindow* self);

void AEWindowOpen(AEWindow* self, const char* title, int w, int h);
void AEWindowRun(AEWindow* self);
void AEWindowClose(AEWindow* self);

