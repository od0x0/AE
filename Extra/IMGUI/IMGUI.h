#pragma once
#include "Core.h"

//An experiment in immediate mode gui, scales down very well, and very low memory/performace footprint

typedef struct{
	float x, y, w, h;
}AEIMGUIRect;

static inline AEIMGUIRect AEIMGUIRectNew(float x,float y,float w,float h){
	AEIMGUIRect rect={x,y,w,h};
	return rect;
}

typedef struct AEIMGUIEvent{
	int type;
	union{
		struct{float x,y;}mouseMove;
		struct{int button;}mouseClick;
		struct{int key;}keyDown;
		struct{int key;}keyUp;
	}data;
	struct AEIMGUIEvent* lastEvent;
}AEIMGUIEvent;

typedef struct{
	size_t activeItemID;
	unsigned int currentDepth;
	AETexture backgroundTexture;
	AEIMGUIRect bounds;
	AERGBA normalColor, hoverColor, clickColor;
	AEIMGUIEvent* lastChain;
}AEIMGUI;

#define AEIMGUIEventMouseMove 1
#define AEIMGUIEventMouseClick 2
#define AEIMGUIEventKeyDown 3
#define AEIMGUIEventKeyUp 4

void AEIMGUIWindowSizeSet(AEIMGUI* gui,float w,float h);

void AEIMGUIPositionSet(AEIMGUI* gui,float x,float y);

void AEIMGUISizeSet(AEIMGUI* gui,float w,float h);

bool AEIMGUIButtonSubmit(AEIMGUI* gui, size_t itemID, char* text);
void AEIMGUILabelSubmit(AEIMGUI* gui, size_t itemID, char* text);
bool AEIMGUIImageSubmit(AEIMGUI* gui);

char* AEIMGUITextGet(AEIMGUI* gui);
char* AEIMGUITextSet(AEIMGUI* gui);