#include "../Core.h"
#include "../SOIL/SOIL.h"
#include <math.h>
#include <string.h>

//C has no template functions, so we have to do this ugly thing

unsigned int AELinearSearch_internal(void* value,void* array,int length,int size){
	if(array==NULL || value==NULL) return 0;
	for(unsigned int i=0;i<length;i++)
		if(memcmp(((char*)array+(i*size)),value,size)==0) return i+1;
	
	return 0;
}

#define AELinearSearch(val,array,len) AELinearSearch_internal(val,array,len,sizeof(*val))

void AEMoveXZ(void* object,float x,float z){
	AECameraType* o=(AECameraType*)object;//As long as we don't use the fov/near/far, we should be fine
	const float piover180=M_PI/180;
	o->x += sinf(o->rotation.y*piover180) * z;
	o->z += cosf(o->rotation.y*piover180) * z;
	o->x += sinf((o->rotation.y+90)*piover180) * x;
	o->z += cosf((o->rotation.y+90)*piover180) * x;
}

AEVec3 AEAngleTo(float x,float y,float z){
	const float piunder180=180/M_PI;
	AEVec3 angle= {atanf(y/z)*piunder180,atanf(z/x)*piunder180,atanf(y/x)*piunder180};
	return angle;
}