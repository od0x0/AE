#pragma once
#include "Core.h"
#include "FastMath.h"
#include <math.h>

inline AEVec3 AEVec3Lerp(AEVec3 a,AEVec3 b,float t){
	AEVec3 r;
	r.x=AELerp(a.x,b.x,t);
	r.y=AELerp(a.y,b.y,t);
	r.z=AELerp(a.z,b.z,t);
	return r;
}

inline AEVec3 AEVec3UnlerpForA(AEVec3 r,AEVec3 b,float t){
	AEVec3 a;
	a.x=AEUnlerpForA(r.x,b.x,t);
	a.y=AEUnlerpForA(r.y,b.y,t);
	a.z=AEUnlerpForA(r.z,b.z,t);
	return a;
}

typedef AEVec4 AEQuat;

inline AEVec3 AEVec3FromCoords(float x,float y,float z){
	AEVec3 v={x,y,z};
	return v;
}
inline AEVec3 AEVec3FromSingle(float xyz){return AEVec3FromCoords(xyz,xyz,xyz);}
inline AEVec3 AEVec3Add(AEVec3 v,AEVec3 v2){return AEVec3FromCoords(v.x+v2.x,v.y+v2.y,v.z+v2.z);}
inline AEVec3 AEVec3Sub(AEVec3 v,AEVec3 v2){return AEVec3FromCoords(v.x-v2.x,v.y-v2.y,v.z-v2.z);}
inline AEVec3 AEVec3Mul(AEVec3 v,AEVec3 v2){return AEVec3FromCoords(v.x*v2.x,v.y*v2.y,v.z*v2.z);}
inline AEVec3 AEVec3Div(AEVec3 v,AEVec3 v2){return AEVec3FromCoords(v.x/v2.x,v.y/v2.y,v.z/v2.z);}
inline float AEVec3Dot(AEVec3 v,AEVec3 v2){return v.x*v2.x+v.y*v2.y+v.z*v2.z;}
inline AEVec3 AEVec3Cross(AEVec3 v,AEVec3 v2){return AEVec3FromCoords(v.y*v2.z-v2.y*v.z,v.z*v2.x-v2.z*v.x,v.x*v2.y-v2.x*v.y);}
inline float AEVec3LengthSQ(AEVec3 v){return AEVec3Dot(v,v);}
inline float AEVec3LengthInv(AEVec3 v){return AESqrtInv(AEVec3LengthSQ(v));}
inline AEVec3 AEVec3Normalized(AEVec3 v){return AEVec3Mul(AEVec3FromSingle(AEVec3LengthInv(v)),v);}
inline float AEVec3LengthInvAccurate(AEVec3 v){return 1/sqrtf(AEVec3LengthSQ(v));}
inline AEVec3 AEVec3NormalizedAccurate(AEVec3 v){return AEVec3Mul(AEVec3FromSingle(AEVec3LengthInvAccurate(v)),v);}
static char* AEVec3AsString(AEVec3 v){
	static char buffer[512];
	sprintf(buffer,"{%f,%f,%f}",v.x,v.y,v.z);
	return buffer;
}
inline AEVec3 AEFastAngleTo(AEVec3 v){
	AEVec3 r={AEAtan2(v.y,v.z),AEAtan2(v.z,v.z),AEAtan2(v.y,v.x)};
	return r;
}

inline AEQuat AEQuatFromCoords(float x,float y,float z,float w){
	AEQuat q={x,y,z,w};
	return q;
}
inline AEQuat AEQuatIdentity(void){return AEQuatFromCoords(0,0,0,1);}
inline AEQuat AEQuatFromVec3(AEVec3 v){return AEQuatFromCoords(v.x,v.y,v.z,0);}
inline AEQuat AEQuatMul(AEQuat q,AEQuat q2){
	AEQuat r;
	r.x=(q.w*q2.x) + (q.x*q2.w) + (q.y*q2.z) - (q.z*q2.y);
	r.y=(q.w*q2.y) - (q.x*q2.z) + (q.y*q2.w) + (q.z*q2.x);
	r.z=(q.w*q2.z) + (q.x*q2.y) - (q.y*q2.x) + (q.z*q2.w);
	r.w=(q.w*q2.w) - (q.x*q2.x) - (q.y*q2.y) - (q.z*q2.z);
	return r;
}
inline AEQuat AEQuatNormalizedApprox(AEQuat q){
	float magnitude = AESqrtInv(((q.x * q.x) +(q.y * q.y) +(q.z * q.z) +(q.w * q.w)));
	q.x *= magnitude;
	q.y *= magnitude;
	q.z *= magnitude;
	q.w *= magnitude;
	return q;
}
inline AEQuat AEQuatNormalized(AEQuat q){
	float magnitude = 1/sqrt(((q.x * q.x) +(q.y * q.y) +(q.z * q.z) +(q.w * q.w)));
	q.x *= magnitude;
	q.y *= magnitude;
	q.z *= magnitude;
	q.w *= magnitude;
	return q;
}
inline AEQuat AEQuatInverse(AEQuat q){
	float length = 1.0f / ((q.x * q.x) +(q.y * q.y) +(q.z * q.z) +(q.w * q.w));
	q.x *= -length;
	q.y *= -length;
	q.z *= -length;
	q.w *= length;
	return q;
}
inline AEQuat AEQuatConjungate(AEQuat q){return AEQuatFromCoords(-q.x,-q.y,-q.z,q.w);}
inline AEVec3 AEQuatMulVec3(AEQuat q,AEVec3 v){
	AEQuat vq={v.x,v.y,v.z,0};
	vq=AEQuatMul(q,AEQuatMul(vq,AEQuatInverse(q)));
	v.x=vq.x;
	v.y=vq.y;
	v.z=vq.z;
	return v;
}
inline void AEQuatRotate(AEQuat* q,float angle,float x,float y,float z){
	AEQuat rotation={0,0,0,1};
	angle *= AEPiOver180*0.5f;
	float sinAngle = AESin(angle);
	rotation.x = (x * sinAngle);
	rotation.y = (y * sinAngle);
	rotation.z = (z * sinAngle);
	rotation.w = AECos(angle);
	*q=AEQuatMul(*q,rotation);
}
inline void AEQuatRotateX(AEQuat* q,float angle){
	AEQuat rotation={0,0,0,1};
	angle *= AEPiOver180*0.5f;
	rotation.x = AESin(angle);
	rotation.w = AECos(angle);
	*q=AEQuatMul(*q,rotation);
}
inline void AEQuatRotateY(AEQuat* q,float angle){
	AEQuat rotation={0,0,0,1};
	angle *= AEPiOver180*0.5f;
	rotation.y = AESin(angle);
	rotation.w = AECos(angle);
	*q=AEQuatMul(*q,rotation);
}
inline void AEQuatRotateZ(AEQuat* q,float angle){
	AEQuat rotation={0,0,0,1};
	angle *= AEPiOver180*0.5f;
	rotation.z = AESin(angle);
	rotation.w = AECos(angle);
	*q=AEQuatMul(*q,rotation);
}
inline AEQuat AEQuatFrom3dRotation(AEVec3 v){
	AEQuat x,y,z;
	AEQuatRotateX(&x,v.x);
	AEQuatRotateY(&y,v.y);
	AEQuatRotateZ(&z,v.z);
	return AEQuatMul(z,AEQuatMul(y,x));
}