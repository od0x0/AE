#include "A2RATNodeMaterial.h"

A2RATNodeType* A2RATNodeTypeMaterial(void);

void A2RATNodeMaterialTextureListSet(A2RATNode* self, AEList* textureList){
	AEList* oldTextureList=A2RATNodeMaterialTextureListGet(self);
	for (size_t i=0; i<AEListLength(oldTextureList); i++) {
		A2RATNodeDelete(AEListAsArray(oldTextureList,A2RATNode*)[i]);
	}
	AEListDelete(oldTextureList);
	
	textureList=AEListRetain(textureList);
	
	for (size_t i=0; i<AEListLength(textureList); i++) {
		A2RATNodeRetain(AEListAsArray(oldTextureList,A2RATNode*)[i]);
	}
	
	A2RATNodeSlotSet(self, "texturelist", textureList);
}

AEList* A2RATNodeMaterialTextureListGet(A2RATNode* self){
	return A2RATNodeSlotGet(self, "texturelist");
}