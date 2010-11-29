#pragma once
#include "EUMemory.h"
#include <stdlib.h>
#include <stdio.h>
#include <setjmp.h>
#include <stdbool.h>
#include <stdarg.h>

#define EUFileOpeningException "EUFileOpeningException"

typedef struct EUExceptionCatcher EUExceptionCatcher;
struct EUExceptionCatcher{
	EUExceptionCatcher* parent;
	jmp_buf jumpBuffer;
	char* type;
	void* userdata;
	char* message;
};

void EUExceptionCatcherInitWithParent(EUExceptionCatcher* self, EUExceptionCatcher* parent);
bool EUExceptionCatcherTry(EUExceptionCatcher* self);
bool EUExceptionCatcherCaughtExceptionIsOfType(EUExceptionCatcher* self, char* type);
void EUExceptionCatcherThrow(EUExceptionCatcher* self, char* type, void* userdata, const char* format, ...);
void EUExceptionCatcherDeinit(EUExceptionCatcher* self);