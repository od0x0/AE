#pragma once
#include "Core.h"
#include "FastMath.h"
#include <math.h>

static inline AEVec3 AEVec3Lerp(AEVec3 a,AEVec3 b,float t){
	AEVec3 r;
	r.x=AELerp(a.x,b.x,t);
	r.y=AELerp(a.y,b.y,t);
	r.z=AELerp(a.z,b.z,t);
	return r;
}

static inline AEVec3 AEVec3UnlerpForA(AEVec3 r,AEVec3 b,float t){
	AEVec3 a;
	a.x=AEUnlerpForA(r.x,b.x,t);
	a.y=AEUnlerpForA(r.y,b.y,t);
	a.z=AEUnlerpForA(r.z,b.z,t);
	return a;
}

typedef AEVec4 AEQuat;

static inline AEVec3 AEVec3FromCoords(float x,float y,float z){
	AEVec3 v={x,y,z};
	return v;
}
static inline AEVec3 AEVec3FromSingle(float xyz){return AEVec3FromCoords(xyz,xyz,xyz);}
static inline AEVec3 AEVec3Add(AEVec3 v,AEVec3 v2){return AEVec3FromCoords(v.x+v2.x,v.y+v2.y,v.z+v2.z);}
static inline AEVec3 AEVec3Sub(AEVec3 v,AEVec3 v2){return AEVec3FromCoords(v.x-v2.x,v.y-v2.y,v.z-v2.z);}
static inline AEVec3 AEVec3Mul(AEVec3 v,AEVec3 v2){return AEVec3FromCoords(v.x*v2.x,v.y*v2.y,v.z*v2.z);}
static inline AEVec3 AEVec3Div(AEVec3 v,AEVec3 v2){return AEVec3FromCoords(v.x/v2.x,v.y/v2.y,v.z/v2.z);}
static inline float AEVec3Dot(AEVec3 v,AEVec3 v2){return v.x*v2.x+v.y*v2.y+v.z*v2.z;}
static inline AEVec3 AEVec3Cross(AEVec3 v,AEVec3 v2){return AEVec3FromCoords(v.y*v2.z-v2.y*v.z,v.z*v2.x-v2.z*v.x,v.x*v2.y-v2.x*v.y);}
static inline float AEVec3LengthSQ(AEVec3 v){return AEVec3Dot(v,v);}
static inline float AEVec3LengthInv(AEVec3 v){return AESqrtInv(AEVec3LengthSQ(v));}
static inline AEVec3 AEVec3Normalized(AEVec3 v){return AEVec3Mul(AEVec3FromSingle(AEVec3LengthInv(v)),v);}
static inline float AEVec3LengthInvAccurate(AEVec3 v){return 1/sqrtf(AEVec3LengthSQ(v));}
static inline AEVec3 AEVec3NormalizedAccurate(AEVec3 v){return AEVec3Mul(AEVec3FromSingle(AEVec3LengthInvAccurate(v)),v);}
static inline AEVec3 AEVec3Max(AEVec3 a,AEVec3 b){return AEVec3FromCoords(AEMax(a.x,b.x),AEMax(a.y,b.y),AEMax(a.z,b.z));}
static inline AEVec3 AEVec3Min(AEVec3 a,AEVec3 b){return AEVec3FromCoords(AEMin(a.x,b.x),AEMin(a.y,b.y),AEMin(a.z,b.z));}
static char* AEVec3AsString(AEVec3 v){
	static char buffer[512];
	sprintf(buffer,"{%f,%f,%f}",v.x,v.y,v.z);
	return buffer;
}
static inline AEVec3 AEFastAngleTo(AEVec3 v){
	AEVec3 r={AEAtan2(v.y,v.z),AEAtan2(v.z,v.z),AEAtan2(v.y,v.x)};
	return r;
}

static inline AEQuat AEQuatFromCoords(float x,float y,float z,float w){
	AEQuat q={x,y,z,w};
	return q;
}
static inline AEQuat AEQuatIdentity(void){return AEQuatFromCoords(0,0,0,1);}
static inline AEQuat AEQuatFromVec3(AEVec3 v){return AEQuatFromCoords(v.x,v.y,v.z,0);}
static inline AEQuat AEQuatMul(AEQuat q,AEQuat q2){
	AEQuat r;
	r.x=(q.w*q2.x) + (q.x*q2.w) + (q.y*q2.z) - (q.z*q2.y);
	r.y=(q.w*q2.y) - (q.x*q2.z) + (q.y*q2.w) + (q.z*q2.x);
	r.z=(q.w*q2.z) + (q.x*q2.y) - (q.y*q2.x) + (q.z*q2.w);
	r.w=(q.w*q2.w) - (q.x*q2.x) - (q.y*q2.y) - (q.z*q2.z);
	return r;
}
static inline AEQuat AEQuatNormalizedApprox(AEQuat q){
	float magnitude = AESqrtInv(((q.x * q.x) +(q.y * q.y) +(q.z * q.z) +(q.w * q.w)));
	q.x *= magnitude;
	q.y *= magnitude;
	q.z *= magnitude;
	q.w *= magnitude;
	return q;
}
static inline AEQuat AEQuatNormalized(AEQuat q){
	float magnitude = 1/sqrt(((q.x * q.x) +(q.y * q.y) +(q.z * q.z) +(q.w * q.w)));
	q.x *= magnitude;
	q.y *= magnitude;
	q.z *= magnitude;
	q.w *= magnitude;
	return q;
}
static inline AEQuat AEQuatInverse(AEQuat q){
	float length = 1.0f / ((q.x * q.x) +(q.y * q.y) +(q.z * q.z) +(q.w * q.w));
	q.x *= -length;
	q.y *= -length;
	q.z *= -length;
	q.w *= length;
	return q;
}
static inline AEQuat AEQuatConjungate(AEQuat q){return AEQuatFromCoords(-q.x,-q.y,-q.z,q.w);}
static inline AEVec3 AEQuatMulVec3(AEQuat q,AEVec3 v){
	AEQuat vq={v.x,v.y,v.z,0};
	vq=AEQuatMul(q,AEQuatMul(vq,AEQuatInverse(q)));
	v.x=vq.x;
	v.y=vq.y;
	v.z=vq.z;
	return v;
}
static inline void AEQuatRotate(AEQuat* q,float angle,float x,float y,float z){
	AEQuat rotation={0,0,0,1};
	angle *= AEPiOver180*0.5f;
	float sinAngle = AESin(angle);
	rotation.x = (x * sinAngle);
	rotation.y = (y * sinAngle);
	rotation.z = (z * sinAngle);
	rotation.w = AECos(angle);
	*q=AEQuatMul(*q,rotation);
}
static inline void AEQuatRotateX(AEQuat* q,float angle){
	AEQuat rotation={0,0,0,1};
	angle *= AEPiOver180*0.5f;
	rotation.x = AESin(angle);
	rotation.w = AECos(angle);
	*q=AEQuatMul(*q,rotation);
}
static inline void AEQuatRotateY(AEQuat* q,float angle){
	AEQuat rotation={0,0,0,1};
	angle *= AEPiOver180*0.5f;
	rotation.y = AESin(angle);
	rotation.w = AECos(angle);
	*q=AEQuatMul(*q,rotation);
}
static inline void AEQuatRotateZ(AEQuat* q,float angle){
	AEQuat rotation={0,0,0,1};
	angle *= AEPiOver180*0.5f;
	rotation.z = AESin(angle);
	rotation.w = AECos(angle);
	*q=AEQuatMul(*q,rotation);
}
static inline AEQuat AEQuatFromEuler(AEVec3 v){
	AEQuat x,y,z={0,0,0,1};
	x=y=z;
	AEQuatRotateX(&x,v.x);
	AEQuatRotateY(&y,v.y);
	AEQuatRotateZ(&z,v.z);
	return AEQuatMul(z,AEQuatMul(y,x));
}

//From http://www.euclideanspace.com/maths/geometry/rotations/conversions/quaternionToEuler/
static inline AEVec3 AEQuatToEuler(AEQuat q){
	AEVec3 result={0,0,0};
	float factor=1/(AEPiOver180*0.5);
	double test = q.x*q.y + q.z*q.w;
	if (test > 0.499) { // singularity at north pole
		result.x = 2 * atan2f(q.x,q.w)*factor;
		result.y = M_PI*0.5*factor;
		result.z = 0;
	}
	else if (test < -0.499) { // singularity at south pole
		result.x = -2 * atan2f(q.x,q.w)*factor;
		result.y = - M_PI*0.5*factor;
		result.z = 0;
	}
	else{
		double sqx = q.x*q.x;
		double sqy = q.y*q.y;
		double sqz = q.z*q.z;
		result.x = atan2f(2*q.y*q.w-2*q.x*q.z , 1 - 2*sqy - 2*sqz)*factor;
		result.y = asinf(2*test)*factor;
		result.z = atan2f(2*q.x*q.w-2*q.y*q.z , 1 - 2*sqx - 2*sqz)*factor;
	}
	return result;
}

static inline AEVec3 AEMatrix3x3ToEuler(float* m3x3){
    // Assuming the angles are in radians.
	float factor=1/(AEPiOver180*0.5);
	AEVec3 result={0,0,0};
	if (m3x3[1] > 0.998) { // singularity at north pole
		result.x = atan2f(m3x3[6],m3x3[8])*factor;
		result.y = M_PI*0.5*factor;
		result.z = 0;
	}
	else if (m3x3[1] < -0.998) { // singularity at south pole
		result.x = atan2f(m3x3[6],m3x3[8])*factor;
		result.y = -M_PI*0.5*factor;
		result.z = 0;
	}
	else{
		result.x = atan2f(-m3x3[2],m3x3[0])*factor;
		result.z = atan2f(-m3x3[7],m3x3[4])*factor;
		result.y = asinf(m3x3[1])*factor;
	}
	return result;
}

static inline void AEQuatToMatrix4x4(AEQuat q,float* m4x4){
	m4x4[0]  = (1.0f - (2.0f * ((q.y * q.y) + (q.z * q.z))));
	m4x4[1]  =         (2.0f * ((q.x * q.y) + (q.z * q.w)));
	m4x4[2]  =         (2.0f * ((q.x * q.z) - (q.y * q.w)));
	m4x4[3]  = 0.0f;
	m4x4[4]  =         (2.0f * ((q.x * q.y) - (q.z * q.w)));
	m4x4[5]  = (1.0f - (2.0f * ((q.x * q.x) + (q.z * q.z))));
	m4x4[6]  =         (2.0f * ((q.y * q.z) + (q.x * q.w)));
	m4x4[7]  = 0.0f;
	m4x4[8]  =         (2.0f * ((q.x * q.z) + (q.y * q.w)));
	m4x4[9]  =         (2.0f * ((q.y * q.z) - (q.x * q.w)));
	m4x4[10] = (1.0f - (2.0f * ((q.x * q.x) + (q.y * q.y))));
	m4x4[11] = 0.0f;
	m4x4[12] = 0.0f;
	m4x4[13] = 0.0f;
	m4x4[14] = 0.0f;
	m4x4[15] = 1.0f;
}