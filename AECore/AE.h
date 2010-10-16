#pragma once

#include "List.h"

//Add known systems
#ifdef __APPLE_CC__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
//Most portable option, just add a header search path
#include "gl.h"
#include "glu.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#ifndef __cplusplus
#include <stdbool.h>
#include <iso646.h>
#endif
//Like, stdbool & iso646 zomgftwbbq

#include <math.h>
#include <string.h>

typedef struct{float x,y;}AEVec2;
typedef struct{float x,y,z;}AEVec3;
typedef struct{float x,y,z,w;}AEVec4;
typedef AEVec4 AEQuat;
typedef struct{int x,y;}AEVec2i;
typedef struct{int x,y,z;}AEVec3i;
typedef struct{int x,y,z,w;}AEVec4i;

typedef struct{float r,g,b;}AERGB;
typedef struct{float r,g,b,a;}AERGBA;
typedef struct{unsigned char r,g,b;}AERGBub;
typedef struct{unsigned char r,g,b,a;}AERGBAub;

#include "Camera.h"