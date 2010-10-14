#include "NodeInternal.h"

void AENodeMaterialDelete(AENode* node){
	AENodeMaterial* material=(AENodeMaterial*)node;
	for(int i=0;i<material->texUnitCount;i++) AETextureDelete(material->textures[i]);
	free(material->textures);
	free(material->textureUniforms);
	glDeleteShader(material->fshader);
	glDeleteShader(material->vshader);
	glDeleteShader(material->program);
}

static unsigned int AEShaderTextureUnitCount=0;

unsigned int AEShadersEnabled=0;

void AENodeMaterialRender(AENode* node){
	AENodeMaterial* material=(AENodeMaterial*)node;
	if(material==NULL){
		if(AEShadersEnabled==0) for(unsigned char i=AEShaderTextureUnitCount;i--;){
			glActiveTexture(GL_TEXTURE0+i);
			AETextureBind(0);
		}
		AEShaderTextureUnitCount=0;
		if(AEShadersEnabled) glUseProgram(0);
		return;
	}
	
	if(AEShadersEnabled) glUseProgram(material->program);
	//It's designed this way so that the last unit accessed is 0, so I have one less state to change
	for(unsigned char i = material->texUnitCount; i--; /*Intentionally nothing here*/){
		glActiveTexture(GL_TEXTURE0+i);
		AETextureBind(material->textures[i]);
		if(AEShadersEnabled && material->textureUniforms) glUniform1i(material->textureUniforms[i],i);
	}
	
	AEShaderTextureUnitCount=material->texUnitCount;
}

void AENodeMaterialStep(AENode* node,float stepInSeconds){

}

void AENodeMaterialShaderSet(AENode* node,char* vshadertext,char* fshadertext){
	if(AEShadersEnabled==0) return;
	
	AENodeMaterial* material=(AENodeMaterial*)node;
	
	if(vshadertext){
		if(material->vshader){
			glDeleteShader(material->vshader);
			material->vshader=0;
		}
		material->vshader=glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(material->vshader, 1, (const GLchar**)&vshadertext,NULL);
		glCompileShader(material->vshader);
	}
	
	if(fshadertext){
		if(material->vshader){
			glDeleteShader(material->fshader);
			material->fshader=0;
		}
		material->fshader=glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(material->fshader, 1, (const GLchar**)&fshadertext,NULL);
		glCompileShader(material->fshader);
	}
	
	if(vshadertext==NULL && fshadertext==NULL){
		if(material->program){
			glDeleteProgram(material->program);
			material->program=0;
		}
		material->program=glCreateProgram();
		if(material->fshader) glAttachShader(material->program,material->fshader);
		if(material->vshader) glAttachShader(material->program,material->vshader);
		glLinkProgram(material->program);
	}
	
	free(material->textureUniforms);
	material->textureUniforms=NULL;
	
	if(material->texUnitCount)
		material->textureUniforms=calloc(1,material->texUnitCount*sizeof(unsigned int));
	for(unsigned char i=material->texUnitCount;i--;){
		char uniformName[256]={'t','e','x','t','u','r','e',i+'0','\0'};
		material->textureUniforms[i]=glGetUniformLocation(material->program,uniformName);
	}
}

void AENodeMaterialTextureSet(AENode* node,char* name,AEImage* image){
	AENodeMaterial* material=(AENodeMaterial*)node;
	material->texUnitCount++;
	material->textures=realloc(material->textures,material->texUnitCount*sizeof(unsigned int));
	material->textures[material->texUnitCount-1]=AEImageToTexture(image);
	AEImageDelete(image);
}