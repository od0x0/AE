#pragma once
#include "AE.h"
#include "SDL.h"
/*
int AESDLEventFilter(const SDL_Event* event);

void AESDLOpen(AEContext* context,const char* title);
void AESDLSwapBuffers(AEContext* context);
void AESDLRefresh(AEContext* context);
int AESDLKey(int key);
int AESDLMouseButton(char button);
int AESDLPollInput(AEContext* context);
void AESDLClose(AEContext* context);
double AESDLSecondsGet(AEContext* context);
void AESDLRefreshContext(void);

void AESDLSetupContext(AEContext* context);
*/
int AESDLEventFilter(const SDL_Event* event);

void AESDLOpen(AEWindow* window,const char* title);
void AESDLSwapBuffers(AEWindow* window);
void AESDLRefresh(AEWindow* window);
int AESDLKey(int key);
int AESDLMouseButton(char button);
int AESDLPollInput(AEWindow* window);
void AESDLClose(AEWindow* window);
double AESDLGetSeconds(AEWindow* window);

const AEWindowContextCallbacks* AESDLWindowContextCallbacks(void);