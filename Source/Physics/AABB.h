#include "FastMath3D.h"

typedef struct{
	union{
		float x,y,z;
		float array[3];
		AEVec3 vec3;
	}center;
	union{
		float x,y,z;
		float array[3];
		AEVec3 vec3;
	}size;
}AEAABB;

inline int AEAABBCollide(AEAABB* b,AEAABB* b2){
	for(int i=0;i<3;i++){
		float d=fabsf(b->center.array[i]-b2->center.array[i]);
		float size=b->size.array[i]+b2->size.array[i];
		if(d>size) return 0;
	}
	return 1;
}

inline void AEAABBFromTriangles(AEAABB* b,AEVec3* vs,unsigned int count){
	AEVec3 min={0,0,0};
	AEVec3 max=min;
	
	for(int i=0;i<count;i++){
		max=AEVec3Max(max,vs[i]);
		min=AEVec3Min(min,vs[i]);
	}
	
	b->size.vec3=AEVec3Sub(max,min);
	b->center.vec3=AEVec3Add(min,AEVec3Mul(b->size.vec3,AEVec3FromSingle(0.5)));
}