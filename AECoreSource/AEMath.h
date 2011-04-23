#pragma once
#include <math.h>
#include <stdlib.h>

#define AEEpsilon 0.000001
#define AEPi 3.14159265f
#define AEInfinity INFINITY

#define AEMathInline static inline

typedef struct{float x,y;}AEVec2;
typedef struct{float x,y,z;}AEVec3;
typedef struct{float x,y,z,w;}AEVec4;
AEMathInline AEVec4 AEVec3AsVec4(AEVec3 v3){
	AEVec4 v4={v3.x,v3.y,v3.z,0};
	return v4;
}
AEMathInline AEVec3 AEVec4AsVec3(AEVec4 v4){
	AEVec3 v3={v3.x,v3.y,v3.z};
	return v3;
}
AEMathInline AEVec3 AEVec2AsVec3(AEVec2 v2){
	AEVec3 v3={v3.x,v3.y,0};
	return v3;
}
AEMathInline AEVec2 AEVec3AsVec2(AEVec3 v3){
	AEVec2 v2={v3.x,v3.y};
	return v2;
}
typedef AEVec4 AEQuat;
typedef AEVec4 AEPlane;
typedef struct{int x,y;}AEVec2i;
typedef struct{int x,y,z;}AEVec3i;
typedef struct{int x,y,z,w;}AEVec4i;

typedef struct{float r,g,b;}AERGB;
typedef struct{float r,g,b,a;}AERGBA;
typedef struct{unsigned char r,g,b;}AERGBub;
typedef struct{unsigned char r,g,b,a;}AERGBAub;

/* Notes:
	The main focus for now is simple code.
	Small structures are passed by value so the compiler has an easier time optimizing.
	A part of this was extracted from tutorials and such because I have better things to do than write math code.
*/

AEMathInline float AELerp(float a,float b,float t){return a+t*(b-a);}
AEMathInline float AEExtrapolateForA(float r,float b,float t){return b-(b-r)/t;}
AEMathInline float AEExtrapolateForB(float r,float a,float t){return -((a-r)/t)-a;}

/*#ifdef __i386__

#include <xmmintrin.h>
AEMathInline float AESqrtInv(float f){
	__m128 x = _mm_load_ss( &f );
	x = _mm_rsqrt_ss( x ); // x = _mm_rsqrt_ss( x, x ) on some compilers
	
	float result;
	_mm_store_ss( &result, x );
	return result;
}	

#elif __ppc__

#include <ppc_intrinsics.h>

AEMathInline float AESqrtInv(float x){
		return __frsqrtes(x);
}

#else
*/
AEMathInline float AESqrtInv(float x){
	return 1/sqrtf(x);
}

//#endif

#define AEPiOver180 (AEPi/180.0f)
#define AE180OverPi (180.0f/AEPi)
#define AESign(x) (((x)>0)*2-1)
//#define AEAbs(x) (AESign(x)*x)
#define AEAbs fabsf
#define AESqrt sqrtf

AEMathInline float AEMax(float a,float b){
	if(a>b) return a;
	else return b;
}

AEMathInline float AEMin(float a,float b){
	if(a<b) return a;
	else return b;
}

AEMathInline float AEClamp(float value, float min, float max){
	if(value < min) value=min;
	if(value > max) value=max;
	return value;
}

AEMathInline float AERandom(void){
	static unsigned int seed=0;
	seed=rand();
	srand(seed);
	return seed/(float)RAND_MAX;
}

AEMathInline float AERandomBetween(float min,float max){
	return (max-min)*AERandom()+min;
}

//////////////////////////////////////////////////////////////////
//3 Component Vector Math
//////////////////////////////////////////////////////////////////

AEMathInline AEVec3 AEVec3Lerp(AEVec3 a,AEVec3 b,float t){
	AEVec3 r;
	r.x=AELerp(a.x,b.x,t);
	r.y=AELerp(a.y,b.y,t);
	r.z=AELerp(a.z,b.z,t);
	return r;
}

AEMathInline AEVec3 AEVec3ExtrapolateForA(AEVec3 r,AEVec3 b,float t){
	AEVec3 a;
	a.x=AEExtrapolateForA(r.x,b.x,t);
	a.y=AEExtrapolateForA(r.y,b.y,t);
	a.z=AEExtrapolateForA(r.z,b.z,t);
	return a;
}

AEMathInline AEVec3 AEVec3From3(float x,float y,float z){
	AEVec3 v={x,y,z};
	return v;
}
AEMathInline AEVec3 AEVec3From1(float xyz){return AEVec3From3(xyz,xyz,xyz);}
AEMathInline AEVec3 AEVec3ScaleTo360Range(AEVec3 v){
	v.x=fmodf(v.x, 360);
	v.y=fmodf(v.y, 360);
	v.z=fmodf(v.z, 360);
	return v;
}
AEMathInline AEVec3 AEVec3Add(AEVec3 v,AEVec3 v2){return AEVec3From3(v.x+v2.x,v.y+v2.y,v.z+v2.z);}
AEMathInline AEVec3 AEVec3Sub(AEVec3 v,AEVec3 v2){return AEVec3From3(v.x-v2.x,v.y-v2.y,v.z-v2.z);}
AEMathInline AEVec3 AEVec3Mul(AEVec3 v,AEVec3 v2){return AEVec3From3(v.x*v2.x,v.y*v2.y,v.z*v2.z);}
AEMathInline AEVec3 AEVec3Div(AEVec3 v,AEVec3 v2){return AEVec3From3(v.x/v2.x,v.y/v2.y,v.z/v2.z);}
AEMathInline AEVec3 AEVec3Neg(AEVec3 v){return AEVec3From3(-v.x,-v.y,-v.z);}
AEMathInline float AEVec3Dot(AEVec3 v,AEVec3 v2){return v.x*v2.x+v.y*v2.y+v.z*v2.z;}
AEMathInline AEVec3 AEVec3Cross(AEVec3 v,AEVec3 v2){return AEVec3From3(v.y*v2.z-v2.y*v.z,v.z*v2.x-v2.z*v.x,v.x*v2.y-v2.x*v.y);}
AEMathInline float AEVec3LengthSQ(AEVec3 v){return AEVec3Dot(v,v);}
AEMathInline float AEVec3LengthInv(AEVec3 v){return AESqrtInv(AEVec3LengthSQ(v));}
AEMathInline float AEVec3Length(AEVec3 v){return AESqrt(AEVec3Dot(v,v));}
AEMathInline AEVec3 AEVec3Normalized(AEVec3 v){return AEVec3Mul(AEVec3From1(AEVec3LengthInv(v)),v);}
AEMathInline AEVec3 AEVec3Max(AEVec3 a,AEVec3 b){return AEVec3From3(AEMax(a.x,b.x),AEMax(a.y,b.y),AEMax(a.z,b.z));}
AEMathInline AEVec3 AEVec3Min(AEVec3 a,AEVec3 b){return AEVec3From3(AEMin(a.x,b.x),AEMin(a.y,b.y),AEMin(a.z,b.z));}
AEMathInline AEVec3 AEVec3Clamp(AEVec3 a, AEVec3 b, AEVec3 c){
	return AEVec3From3(AEClamp(a.x, b.x, c.x), AEClamp(a.y, b.y, c.y), AEClamp(a.z, b.z, c.z));
}
AEMathInline int AEVec3IsBetween(AEVec3 hit,AEVec3 min,AEVec3 max){
	if(hit.x < min.x || max.x < hit.x) return 0;
	if(hit.y < min.y || max.y < hit.y) return 0;
	if(hit.z < min.z || max.z < hit.z) return 0;
	return 1;
}
AEMathInline float AEVec3DistanceBetween(AEVec3 v, AEVec3 v2){
	AEVec3 difference=AEVec3Sub(v, v2);
	return sqrtf(AEVec3Dot(difference, difference));
}
AEMathInline AEVec3 AEVec3Round(AEVec3 v){return AEVec3From3(roundf(v.x), roundf(v.y), roundf(v.z));}
AEMathInline AEVec3 AEVec3Abs(AEVec3 v){return AEVec3From3(fabsf(v.x), fabsf(v.y), fabsf(v.z));}
AEMathInline AEVec3 AEVec3RandomBetween(AEVec3 min,AEVec3 max){
	return AEVec3From3(AERandomBetween(min.x,max.x),AERandomBetween(min.y,max.y),AERandomBetween(min.z,max.z));
}
AEMathInline AEVec3 AEVec3AngleTo(AEVec3 v){
	AEVec3 r={AE180OverPi*atan2f(v.y,v.z),AE180OverPi*atan2f(v.z,v.z),AE180OverPi*atan2f(v.y,v.x)};
	return r;
}

//////////////////////////////////////////////////////////////////
//Quaternion Math
//////////////////////////////////////////////////////////////////

AEMathInline AEQuat AEQuatFromCoords(float x,float y,float z,float w){
	AEQuat q={x,y,z,w};
	return q;
}
AEMathInline AEQuat AEQuatIdentity(void){return AEQuatFromCoords(0,0,0,1);}
AEMathInline AEQuat AEQuatFromVec3(AEVec3 v){return AEQuatFromCoords(v.x,v.y,v.z,0);}
AEMathInline void AEQuatComputeW(AEQuat* q){
	AEQuat r=*q;
	float t = 1.0f - r.x*r.x - r.y*r.y - r.z*r.z;
	if(t<0.0f) r.w=0.0f;
	else r.w=-sqrtf(t);
	*q=r;
}
AEMathInline AEQuat AEQuatMul(AEQuat q,AEQuat q2){
	AEQuat r;
	r.x=(q.w*q2.x) + (q.x*q2.w) + (q.y*q2.z) - (q.z*q2.y);
	r.y=(q.w*q2.y) - (q.x*q2.z) + (q.y*q2.w) + (q.z*q2.x);
	r.z=(q.w*q2.z) + (q.x*q2.y) - (q.y*q2.x) + (q.z*q2.w);
	r.w=(q.w*q2.w) - (q.x*q2.x) - (q.y*q2.y) - (q.z*q2.z);
	return r;
}
AEMathInline AEQuat AEQuatNormalized(AEQuat q){
	float magnitude = AESqrtInv(((q.x * q.x) +(q.y * q.y) +(q.z * q.z) +(q.w * q.w)));
	q.x *= magnitude;
	q.y *= magnitude;
	q.z *= magnitude;
	q.w *= magnitude;
	return q;
}
AEMathInline AEQuat AEQuatInverse(AEQuat q){
	float length = 1.0f / ((q.x * q.x) +(q.y * q.y) +(q.z * q.z) +(q.w * q.w));
	q.x *= -length;
	q.y *= -length;
	q.z *= -length;
	q.w *= length;
	return q;
}
AEMathInline AEQuat AEQuatConjungate(AEQuat q){return AEQuatFromCoords(-q.x,-q.y,-q.z,q.w);}
AEMathInline AEVec3 AEQuatMulVec3(AEQuat q,AEVec3 v){
	AEQuat vq={v.x,v.y,v.z,0};
	vq=AEQuatMul(q,AEQuatMul(vq,AEQuatInverse(q)));
	v.x=vq.x;
	v.y=vq.y;
	v.z=vq.z;
	return v;
}
AEMathInline void AEQuatRotate(AEQuat* q,float angle,float x,float y,float z){
	AEQuat rotation={0,0,0,1};
	angle *= AEPiOver180*0.5f;
	float sinAngle = sinf(angle);
	rotation.x = (x * sinAngle);
	rotation.y = (y * sinAngle);
	rotation.z = (z * sinAngle);
	rotation.w = cosf(angle);
	*q=AEQuatMul(*q,rotation);
}
AEMathInline void AEQuatRotateX(AEQuat* q,float angle){
	AEQuat rotation={0,0,0,1};
	angle *= AEPiOver180*0.5f;
	rotation.x = sinf(angle);
	rotation.w = cosf(angle);
	*q=AEQuatMul(*q,rotation);
}
AEMathInline void AEQuatRotateY(AEQuat* q,float angle){
	AEQuat rotation={0,0,0,1};
	angle *= AEPiOver180*0.5f;
	rotation.y = sinf(angle);
	rotation.w = cosf(angle);
	*q=AEQuatMul(*q,rotation);
}
AEMathInline void AEQuatRotateZ(AEQuat* q,float angle){
	AEQuat rotation={0,0,0,1};
	angle *= AEPiOver180*0.5f;
	rotation.z = sinf(angle);
	rotation.w = cosf(angle);
	*q=AEQuatMul(*q,rotation);
}
AEMathInline AEQuat AEQuatFromEuler(AEVec3 v){
	AEQuat x,y,z={0,0,0,1};
	v=AEVec3ScaleTo360Range(v);
	x=y=z;
	AEQuatRotateX(&x,v.x);
	AEQuatRotateY(&y,v.y);
	AEQuatRotateZ(&z,v.z);
	return AEQuatMul(z,AEQuatMul(y,x));
}

AEMathInline AEQuat AEQuatSlerp(AEQuat start, AEQuat end, float alpha) {
	const float SLERP_TO_LERP_SWITCH_THRESHOLD=0.01f;
	
	float difference = ((start.x * end.x) + (start.y * end.y) + (start.z * end.z) + (start.w * end.w));
	float startWeight, endWeight;
	
	if((1.0f - fabs(difference)) > SLERP_TO_LERP_SWITCH_THRESHOLD) {
		float theta = acosf(fabsf(difference));
		float oneOverSinTheta = (1.0f / sinf(theta));
		startWeight = (sinf(theta * (1.0f - alpha)) * oneOverSinTheta);
		endWeight = (sinf(theta * alpha) * oneOverSinTheta);
		if (difference < 0.0f) startWeight = -startWeight;
	}
	else{
		startWeight = (1.0f - alpha);
		endWeight = alpha;
	}
	
	AEQuat result;
	
	result.x = (start.x * startWeight) + (end.x * endWeight);
	result.y = (start.y * startWeight) + (end.y * endWeight);
	result.z = (start.z * startWeight) + (end.z * endWeight);
	result.w = (start.w * startWeight) + (end.w * endWeight);
	return AEQuatNormalized(result);
}

AEMathInline AEQuat AEQuatBetween(AEVec3 v1,AEVec3 v2){
	AEQuat q;
	AEVec3 axis = AEVec3Cross(v1,v2);
	q.x = axis.x;
	q.y = axis.y;
	q.z = axis.z;
	//if (0/*vectors are unit length*/) q.w = 1 + AEVec3Dot(v1,v2);
	//else
		q.w = AESqrtInv(AEVec3LengthSQ(v1) * AEVec3LengthSQ(v2)) + AEVec3Dot(v1,v2);
	
	q=AEQuatNormalized(q);
	return q;
}

/*//From http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/
AEMathInline AEVec3 AEQuatToEuler(AEQuat q){
	AEVec3 result={0,0,0};
	float factor=1/(AEPiOver180*0.5f);
	float test = q.x*q.y + q.z*q.w;
	if (test > 0.499) { // singularity at north pole
		result.x = 2 * atan2f(q.x,q.w)*factor;
		result.y = (float)M_PI*0.5f*factor;
		result.z = 0;
	}
	else if (test < -0.499) { // singularity at south pole
		result.x = -2.0f * atan2f(q.x,q.w)*factor;
		result.y = (float)- (float)M_PI*0.5f*factor;
		result.z = 0;
	}
	else{
		float sqx = q.x*q.x;
		float sqy = q.y*q.y;
		float sqz = q.z*q.z;
		result.x = (float)atan2f(2.0f*q.y*q.w-2.0f*q.x*q.z , 1 - 2*sqy - 2*sqz)*factor;
		result.y = (float)asinf(2.0f*test)*factor;
		result.z = (float)atan2f(2.0f*q.x*q.w-2.0f*q.y*q.z , 1.0f - 2.0f*sqx - 2.0f*sqz)*factor;
	}
	return result;
}

AEMathInline AEVec4 AEQuatToAxisAngle(AEQuat q){
	float sinAngle;
	AEVec4 axisangle;
	q=AEQuatNormalized(q);
	sinAngle = sqrtf(1.0f - (q.w * q.w));
	if (fabsf(sinAngle) < 0.0005f) sinAngle = 1.0f;
	
	axisangle.x = (q.x / sinAngle);
	axisangle.y = (q.y / sinAngle);
	axisangle.z = (q.z / sinAngle);

	axisangle.w = (acosf(q.w) * 2.0f);
	
	return axisangle;
}*/
/*AEVec4 axisangle={0,0,0,0};
	if (q.w > 1) q=AEQuatNormalized(q); // if w>1 acos and sqrt will produce errors, this cant happen if quaternion is normalised
	axisangle.w = 2 * acos(q.w);
	double s = sqrt(1-q.w*q.w); // assuming quaternion normalised then w is less than 1, so term always positive.
	if (s < 0.001) { // test to avoid divide by zero, s is always positive due to sqrt
		// if s close to zero then direction of axis not important
		axisangle.x = q.x; // if it is important that axis is normalised then replace with x=1; y=z=0;
		axisangle.y = q.y;
		axisangle.z = q.z;
	}
	else {
		axisangle.x = q.x / s; // normalise axis
		axisangle.y = q.y / s;
		axisangle.z = q.z / s;
	}*/
//////////////////////////////////////////////////////////////////
//Basic Matrix Math (Only what is needed that the other functions don't supply already)
//////////////////////////////////////////////////////////////////

/*AEMathInline AEVec3 AEMatrix3x3ToEuler(float* m3x3){
		// Assuming the angles are in radians.
	float factor=1/(AEPiOver180*0.5f);
	AEVec3 result={0,0,0};
	if (m3x3[1] > 0.998f) { // singularity at north pole
		result.x = atan2f(m3x3[6],m3x3[8])*factor;
		result.y = (float)M_PI*0.5f*factor;
		result.z = 0;
	}
	else if (m3x3[1] < -0.998) { // singularity at south pole
		result.x = atan2f(m3x3[6],m3x3[8])*factor;
		result.y = -(float)M_PI*0.5f*factor;
		result.z = 0;
	}
	else{
		result.x = atan2f(-m3x3[2],m3x3[0])*factor;
		result.z = atan2f(-m3x3[7],m3x3[4])*factor;
		result.y = asinf(m3x3[1])*factor;
	}
	return result;
}
*/

AEMathInline void AEMatrixFromQuat(float* m4x4, AEQuat q){
	m4x4[0]	= (1.0f - (2.0f * ((q.y * q.y) + (q.z * q.z))));
	m4x4[1]	=				 (2.0f * ((q.x * q.y) + (q.z * q.w)));
	m4x4[2]	=				 (2.0f * ((q.x * q.z) - (q.y * q.w)));
	m4x4[3]	= 0.0f;
	m4x4[4]	=				 (2.0f * ((q.x * q.y) - (q.z * q.w)));
	m4x4[5]	= (1.0f - (2.0f * ((q.x * q.x) + (q.z * q.z))));
	m4x4[6]	=				 (2.0f * ((q.y * q.z) + (q.x * q.w)));
	m4x4[7]	= 0.0f;
	m4x4[8]	=				 (2.0f * ((q.x * q.z) + (q.y * q.w)));
	m4x4[9]	=				 (2.0f * ((q.y * q.z) - (q.x * q.w)));
	m4x4[10] = (1.0f - (2.0f * ((q.x * q.x) + (q.y * q.y))));
	m4x4[11] = 0.0f;
	m4x4[12] = 0.0f;
	m4x4[13] = 0.0f;
	m4x4[14] = 0.0f;
	m4x4[15] = 1.0f;
}

AEMathInline void AEMatrixLoadIdentity(float* m){
	m[0]=1.0f;
	m[1]=0.0f;
	m[2]=0.0f;
	m[3]=0.0f;
	
	m[4]=0.0f;
	m[5]=1.0f;
	m[6]=0.0f;
	m[7]=0.0f;
	
	m[8]=0.0f;
	m[9]=0.0f;
	m[10]=1.0f;
	m[11]=0.0f;
	
	m[12]=0.0f;
	m[13]=0.0f;
	m[14]=0.0f;
	m[15]=1.0f;
}

AEMathInline void AEMatrixMul(float* r, float* a, float* b){
	r[0]=a[0]*b[0]+a[4]*b[1]+a[8]*b[2]+a[12]*b[3];
	r[1]=a[1]*b[0]+a[5]*b[1]+a[9]*b[2]+a[13]*b[3];
	r[2]=a[2]*b[0]+a[6]*b[1]+a[10]*b[2]+a[14]*b[3];
	r[3]=a[3]*b[0]+a[7]*b[1]+a[11]*b[2]+a[15]*b[3];
	r[4]=a[0]*b[4]+a[4]*b[5]+a[8]*b[6]+a[12]*b[7];
	r[5]=a[1]*b[4]+a[5]*b[5]+a[9]*b[6]+a[13]*b[7];
	r[6]=a[2]*b[4]+a[6]*b[5]+a[10]*b[6]+a[14]*b[7];
	r[7]=a[3]*b[4]+a[7]*b[5]+a[11]*b[6]+a[15]*b[7];
	r[8]=a[0]*b[8]+a[4]*b[9]+a[8]*b[10]+a[12]*b[11];
	r[9]=a[1]*b[8]+a[5]*b[9]+a[9]*b[10]+a[13]*b[11];
	r[10]=a[2]*b[8]+a[6]*b[9]+a[10]*b[10]+a[14]*b[11];
	r[11]=a[3]*b[8]+a[7]*b[9]+a[11]*b[10]+a[15]*b[11];
	r[12]=a[0]*b[12]+a[4]*b[13]+a[8]*b[14]+a[12]*b[15];
	r[13]=a[1]*b[12]+a[5]*b[13]+a[9]*b[14]+a[13]*b[15];
	r[14]=a[2]*b[12]+a[6]*b[13]+a[10]*b[14]+a[14]*b[15];
	r[15]=a[3]*b[12]+a[7]*b[13]+a[11]*b[14]+a[15]*b[15];
}

AEMathInline AEVec3 AEMatrixMulVec3(float* m4x4, AEVec3 v) {
	AEVec3 result;
	
	result.x = ((m4x4[0] * v.x) + (m4x4[4] * v.y) + (m4x4[8]	* v.z) + m4x4[12]);
	result.y = ((m4x4[1] * v.x) + (m4x4[5] * v.y) + (m4x4[9]	* v.z) + m4x4[13]);
	result.z = ((m4x4[2] * v.x) + (m4x4[6] * v.y) + (m4x4[10] * v.z) + m4x4[14]);
	return result;
}

//////////////////////////////////////////////////////////////////
//Plane/Ray Utility Functions
//////////////////////////////////////////////////////////////////

AEMathInline AEVec3 AERayAtTime(AEVec3 s, AEVec3 d, float t){
	return AEVec3Add(s, AEVec3Mul(d, AEVec3From1(t)));
}

AEMathInline float AEPlaneRayIntersectionTime(AEPlane p, AEVec3 s, AEVec3 d){
	return -(p.x*s.x + p.y*s.y + p.z*s.z + p.w) / (p.x*d.x + p.y*d.y + p.z*d.z);
}

AEMathInline float AEPlanePointSignedDistance(AEPlane plane, AEVec3 point){
	return AEVec3Dot(AEVec4AsVec3(plane), point)+plane.w;
}

AEMathInline float AEPlaneLineSegmentIntersectionPercent(AEPlane plane, AEVec3 start, AEVec3 end){
	const float planeDistanceFromStart = AEPlanePointSignedDistance(plane, start);
	//const float planeDistanceFromEnd = AEPlanePointSignedDistance(plane, end);

	const AEVec3 difference = AEVec3Sub(end, start);

	//const float t = 
	return AEAbs(planeDistanceFromStart) / AEVec3Dot(AEVec4AsVec3(plane), difference);

	//return AERayAtTime(start, difference, t);//Notice that we do not normalize!
}

//////////////////////////////////////////////////////////////////
//Bezier Curve Evaluation
//////////////////////////////////////////////////////////////////

AEMathInline float AEBinomialCoefficient(float n, float k){
	if(k > n-k) k = n - k;
	float c = 1;
	for(size_t i=0; i<k; i++){
		c *= n-i;
		c /= i+1;
	}
	return c;
}

AEMathInline AEVec3 AEVec3sEvaluateBezierCurve(AEVec3* points, size_t count, float t){
	AEVec3 r={0,0,0};
	for (size_t i=0; i<count; i++) {
		const float c = AEBinomialCoefficient(count-1, i);
		const float factor = c * powf(1.0f-t, (count-1)-i) * powf(t, i);
		r.x+=points[i].x*factor;
		r.y+=points[i].y*factor;
		r.z+=points[i].z*factor;
	}
	return r;
}

AEMathInline AEVec2 AEVec2sEvaluateBezierCurve(AEVec2* points, size_t count, float t){
	AEVec2 r={0,0};
	for (size_t i=0; i<count; i++) {
		const float c = AEBinomialCoefficient(count-1, i);
		const float factor = c * powf(1.0f-t, (count-1)-i) * powf(t, i);
		r.x+=points[i].x*factor;
		r.y+=points[i].y*factor;
	}
	return r;
}

//////////////////////////////////////////////////////////////////
//Other
//////////////////////////////////////////////////////////////////

AEMathInline float AETriangleCalculateArea(AEVec3 u, AEVec3 v, AEVec3 w){
	return 0.5f*AEAbs(AEVec3Dot(AEVec3Sub(v, u), AEVec3Sub(w, u)));
}
