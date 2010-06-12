#pragma once
#include "Core.h"

void AEObjectLuaEvent(AEObject* object,int event,void* data);
void AEObjectLuaAddEvent(AEObject* object,int event,const char* name);