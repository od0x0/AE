#pragma once
#include "AECore.h"
#include "TextBuffer.h"
#include "AEUIFont.h"
#include "AETable.h"
#include "AEArray.h"

typedef struct{
	int x,y,w,h;
}AEUIRect;

typedef struct{
	unsigned int type;
	union{
		
	}data;
}AEUIEvent;

typedef struct AEUI AEUI;
typedef struct AEUIView AEUIView;
typedef void (*AEUIViewDeinitFunc)(AEUIView* self);
typedef void (*AEUIViewDrawFunc)(AEUIView* self);
typedef void (*AEUIViewEventFunc)(AEUIView* self, const AEUIEvent* event);

AEUIView* AEUIViewNew(AEUI* ui);
AEUIView* AEUIViewRetain(AEUIView* self);
void AEUIViewDelete(AEUIView* self);
void AEUIViewRender(AEUIView* self);
void AEUIViewEvent(AEUIView* self, const AEUIEvent* event);
void AEUIViewNameSet(AEUIView* self, char* name);
char* AEUIViewNameGet(AEUIView* self);
void AEUIViewUISet(AEUIView* self, AEUI* ui);
AEUI* AEUIViewUIGet(AEUIView* self);
void AEUIViewParentSet(AEUIView* self, AEUIView* parent);
AEUIView* AEUIViewParentGet(AEUIView* self);
void AEUIViewChildrenAdd(AEUIView* self, AEUIView* child);
void AEUIViewChildrenRemove(AEUIView* self, AEUIView* child);
AEUIView* AEUIViewChildrenGet(AEUIView* self, size_t i);
void AEUIViewChildrenSet(AEUIView* self, size_t i, AEUIView* child);
size_t AEUIViewChildrenCount(AEUIView* self, size_t i);
void AEUIViewBoundsSet(AEUIView* self, AEUIRect* bounds);
void AEUIViewBoundsGet(AEUIView* self, AEUIRect* bounds);
void AEUIViewDrawFuncSet(AEUIView* self, AEUIViewDrawFunc func);
AEUIViewDrawFunc AEUIViewDrawFuncGet(AEUIView* self);
void AEUIViewEventFuncSet(AEUIView* self, AEUIViewEventFunc func);
AEUIViewEventFunc AEUIViewEventFuncGet(AEUIView* self);
void AEUIViewDeinitFuncSet(AEUIView* self, AEUIViewDeinitFunc func);
AEUIViewDeinitFunc AEUIViewDeinitFuncGet(AEUIView* self);
void AEUIViewUserdataSet(AEUIView* self, void* userdata);
void* AEUIViewUserdataGet(AEUIView* self);

AEUI* AEUINew(char* fontpath, int size);
AEUI* AEUIRetain(AEUI* self);
void AEUIDelete(AEUI* self);

void AEUIWindowBoundsGet(AEUI* self, AEUIRect* bounds);
void AEUIWindowBoundsSet(AEUI* self, AEUIRect* bounds);
AEUIView* AEUIRootViewGet(AEUI* self);
void AEUIRootViewSet(AEUI* self, AEUIView* root);
AEUIFont* AEUIFontGet(AEUI* self);

#define AEUIMouseButtonLeft (1)
#define AEUIMouseButtonMiddle (2)
#define AEUIMouseButtonRight (3)
#define AEUIMouseButtonUpWheel (4)
#define AEUIMouseButtonDownWheel (5)