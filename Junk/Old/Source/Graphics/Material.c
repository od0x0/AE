#include "../Material.h"

AEList* AEMaterials=NULL;

unsigned int AEMaterialLoad(char* path){
	if(path==NULL){
		if(AEMaterials==NULL) AEMaterials=AEListNew(AEMaterial);
		AEMaterial mat={{0,0},0,NULL};
		AEListAdd(AEMaterials,AEMaterial,mat);
		return AEListLength(AEMaterials);//OpenGL Texture style
	}
	return 0;
}

void AEMaterialBind(unsigned int material,unsigned int additionalTexture){
	if(material==0){
		//Unbind everything
		AEShaderBind(NULL,0,NULL);
		return;
	}
	AEMaterial* mat=AEListAsArray(AEMaterials,AEMaterial);
	material--;
	unsigned int textures[3];
	textures[0]=mat[material].textures[0];
	textures[1]=mat[material].textures[1];
	unsigned int count=mat[material].count;
	if(additionalTexture){
		textures[2]=additionalTexture;
		count++;
	}
	AEShaderBind(mat[material].shader,count,textures);
}

void AEMaterialDelete(unsigned int material){
	if(material==0) return;
	material--;
	AEShaderDelete(AEListAsArray(AEMaterials,AEMaterial)[material].shader);
	AETextureDelete(AEListAsArray(AEMaterials,AEMaterial)[material].textures[0]);
	AETextureDelete(AEListAsArray(AEMaterials,AEMaterial)[material].textures[1]);
}
