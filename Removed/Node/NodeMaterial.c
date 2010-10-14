#include "NodeInternal.h"

void AENodeMaterialDelete(AENode* node){}

void AENodeMaterialRender(AENode* node){}

/*void AENodeMaterialStep(AENode* node,float stepInSeconds){
	if(AEShadersEnabled==false) return;
	
	AENodeMaterial* material=(AENodeMaterial*)node;
	
	//material->time+=stepInSeconds;
}*/

void AENodeMaterialShaderSet(AENode* node,char* vshadertext,char* fshadertext){}

void AENodeMaterialTextureSet(AENode* node,char* name,AEImage* image){}