/*
Writen by Oliver Daids, 2009.

Ambition Engine Core.
*/

#pragma once

//Add known systems to make it easier.
#ifdef __APPLE__
	#include <TargetConditionals.h>
	#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
		#define AEiOS
		#include <OpenGLES/ES1/gl.h>
		#include <OpenGLES/ES1/glext.h>
	#else
		#include <OpenGL/gl.h>
		#include <OpenGL/glu.h>
	#endif
#elif defined(__WIN32__)
#include "GL/gl.h"
#include "GL/glu.h"
#else
//Most portable option, just add a header search path
#include "gl.h"
#include "glu.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
//If your compiler doesn't have this, then that sucks for you. *cough* MSVC *cough*
#include <stdint.h>
#ifndef __cplusplus
#include <stdbool.h>
#include <iso646.h>
#endif

//Like, stdbool & iso646 zomgftwbbq

#include <math.h>
#include <string.h>

static char* AECloneString(const char* string){
	if(not string) return NULL;
	const size_t length=strlen(string)+1;
	return (char*)memcpy(malloc(length+1), string, length+1);
}
