#include "../Light.h"

AEList* AELights=NULL;

void AELightsSort(void){}

void AELightsToBoundLights(char maxActive,float x,float y,float z,float radius){
	AELight* lights=AEListAsArray(AELights,AELight);
	AEBoundLightCount=0;
	for(unsigned int i=0;i<AEListLength(AELights) && AEBoundLightCount<maxActive;i++){
		if(lights[i].on==0) continue;
		AEVec3 difference={lights[i].x-x,lights[i].y-y,lights[i].z-z};
		float sqrDist=difference.x*difference.x+difference.y*difference.y+difference.z*difference.z;
		if(sqrDist>((radius+lights[i].radius)*(radius+lights[i].radius))) continue;
		if(AECameraVFCheckSphere(AECameraActiveGet(),lights[i].x,lights[i].y,lights[i].z,lights[i].radius)==0) continue;
		
		AEBoundLights[AEBoundLightCount++]=lights[i];
	}
	if(AEBoundLightCount>1) AEBoundLightCount--;
}

void AELightsInit(void){
	AELights=AEListNew(AELight);
}

void AELightsQuit(void){
	AEListDelete(AELights);
}

AELight AEBoundLights[8];
char AEBoundLightCount=0;

void AEBoundLightsBind(void){}