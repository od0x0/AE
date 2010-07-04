#pragma once
#include "Core.h"
#include <math.h>

static inline float AELerp(float a,float b,float t){return a+t*(b-a);}
static inline float AEUnlerpForA(float r,float b,float t){return b-(b-r)/t;}
static inline float AEUnlerpForB(float r,float a,float t){return -((a-r)/t)-a;}

#ifdef __i386__

#include <xmmintrin.h>
static inline float AESqrtInv(float f){
	__m128 x = _mm_load_ss( &f );
	x = _mm_rsqrt_ss( x ); // x = _mm_rsqrt_ss( x, x ) on some compilers
	
	float result;
	_mm_store_ss( &result, x );
	return result;
}	

#elif __ppc__

#include <ppc_intrinsics.h>

static inline float AESqrtInv(float x){
    return __frsqrtes(x);
}

#else

static inline AESqrtInv(float x){
	return 1/sqrtf(x);
}

#endif

/*
 static inline float AEFastSqrtInv(float x){
 float xhalf = 0.5f*x;
 int i = *(int*)&x;
 i = 0x5f375a86 - (i>>1);
 x = *(float*)&i;
 x = x*(1.5f - xhalf*x*x);
 return x;
 }
*/

/*static float AESinTable[360];
static float AECosTable[360];*/
#define AEPiOver180 (3.14159265/180)
#define AESign(x) (((x)>0)*2-1)
#define AEAbs(x) (AESign(x)*x)

#define AESin sinf
#define AECos cosf
#define AEAtan2 atan2f

/*inline float AEFastSin(float x){
	int index=((int)x)%360;
	if(index<0)
		return AESinTable[360-index];
	else 
		return AESinTable[index];
}

inline float AEFastCos(float x){
	int index=((int)x)%360;
	if(index<0)
		return AECosTable[360-index];
	else 
		return AECosTable[index];
}*/

inline float AEMax(float a,float b){
	if(a>b) return a;
	else return b;
}

inline float AEMin(float a,float b){
	if(a<b) return a;
	else return b;
}

//From the dspguru site (forgot the link)
inline float AEFastAtan2(float y, float x){
	float coeff_1 = 3.14159265/4;
	float coeff_2 = 3*coeff_1;
	float abs_y = AEAbs(y)+1e-10; // kludge to prevent 0/0 condition
	float angle;
	if (x>=0){	
		float r = (x - abs_y) / (x + abs_y);
		angle = coeff_1 - coeff_1 * r;
	}
	else{
		float r = (x + abs_y) / (abs_y - x);
		angle = coeff_2 - coeff_1 * r;
	}
	if (y < 0)
		return(-angle); // negate if in quad III or IV
	else
		return(angle);
}

static inline void AEMathSetup(void){
	/*for(int i=0;i<360;i++){
		AESinTable[i]=sinf(AEPiOver180*i);
		AECosTable[i]=cosf(AEPiOver180*i);
	}*/
}