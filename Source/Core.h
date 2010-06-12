/*
Writen by Oliver Daids, 2009.

Ambition Engine Core.
*/

#pragma once
#include "HeaderStart.h"
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>


typedef struct{float x,y;}AEVec2;
typedef struct{float x,y,z;}AEVec3;
typedef struct{float x,y,z,w;}AEVec4;
typedef struct{int x,y;}AEVec2i;
typedef struct{int x,y,z;}AEVec3i;
typedef struct{int x,y,z,w;}AEVec4i;



void AEInit(char* title,int w,int h);
void AEStart(void (*func)(float));
void AEQuit(void);

void AEGet(int option,void* pointerToValue);
void AESet(int option,void* pointerToValue);

int AEKey(int key);
int AEMouseButton(char button);
void AEPollInput(void);

unsigned int AETextureLoad(const char* filename);
inline void AETextureBind(unsigned int texture){glBindTexture(GL_TEXTURE_2D,(GLuint)texture);}
void AETextureDelete(unsigned int texture);

unsigned int AELinearSearch_internal(void* value,void* array,int length,int size);
#define AELinearSearch(val,array,len) AELinearSearch_internal(val,array,len,sizeof(*val))

#include "HeaderEnd.h"