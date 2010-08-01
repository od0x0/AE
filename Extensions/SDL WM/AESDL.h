#pragma once
#include "Core.h"
#include "SDL.h"

int AESDLEventFilter(const SDL_Event* event);

AEWM* AESDLNew(char* title,AEState* state);

AEState* AESDLStateGet(AEWM* wm);

void AESDLStateSet(AEWM* wm,AEState* state);
void AESDLSwapBuffers(AEWM* wm);

int AESDLKey(int key);
int AESDLMouseButton(char button);

int AESDLPollInput(AEWM* wm);

void AESDLDelete(AEWM* wm);

double AESDLSecondsGet(AEWM* wm);
void AESDLInit(void);