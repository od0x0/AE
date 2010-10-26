#pragma once

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
#include "AEMath.h"
#include "AEArray.h"