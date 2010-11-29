#include "AEMaterial.h"

struct AEMaterial{
	unsigned int refcount;
	AETexture* textures;
	char** textureNames;
	unsigned int* textureUniforms;
	unsigned char texUnitCount;
	unsigned int program,vshader,fshader;
	//unsigned char color[4];  //unneeded?
	GLuint timeUniform, lightmap;
	//float time;
};

static float AEMaterialTime=0.0;

static bool AEMaterialsShadersEnabled=false;

bool AEMaterialsShadersEnabledGet(void){
	return AEMaterialsShadersEnabled;
}

void AEMaterialsShadersEnabledSet(bool on){
	AEMaterialsShadersEnabled=on;
}

float AEMaterialsTimeGet(void){
	return AEMaterialTime;
}

void AEMaterialsTimeSet(float seconds){
	AEMaterialTime=seconds;
}

AEMaterial* AEMaterialRetain(AEMaterial* material){
	if(material) material->refcount++;
	return material;
}

AEMaterial* AEMaterialNew(void){
	return AEMaterialRetain(calloc(1,sizeof(AEMaterial)));
}



void AEMaterialDelete(AEMaterial* material){
	if(material==NULL) return;
	material->refcount--;
	if(material->refcount > 0) return;
	for(int i=0;i<material->texUnitCount;i++){
		AETextureDelete(material->textures[i]);
		free(material->textureNames[i]);
	}
	free(material->textures);
	free(material->textureUniforms);
	free(material->textureNames);
	glDeleteShader(material->fshader);
	glDeleteShader(material->vshader);
	glDeleteShader(material->program);
	free(material);
}

static unsigned int AEShaderTextureUnitCount=0;

void AEMaterialBindShaderOnlyEXT(AEMaterial* material){
	if(not AEMaterialsShadersEnabled) return;
	
	if(material==NULL){
		if(AEMaterialsShadersEnabled==0) for(unsigned char i=AEShaderTextureUnitCount;i--;){
			glActiveTexture(GL_TEXTURE0+i);
			AETextureBind(0);
		}
		AEShaderTextureUnitCount=0;
		if(AEMaterialsShadersEnabled) glUseProgram(0);
		return;
	}
	
	glUseProgram(material->program);
	if(material->timeUniform) glUniform1f((GLint)material->timeUniform-1,AEMaterialTime*0.001);
	
	//It's designed this way so that the last unit accessed is 0, so I have one less state to change
	for(unsigned char i = material->texUnitCount; i--; /*Intentionally nothing here*/){
		glActiveTexture(GL_TEXTURE0+i);
		if(material->textureUniforms) glUniform1i((GLint)material->textureUniforms[i]-1,i);
	}
}

void AEMaterialBind(AEMaterial* material){
	if(material==NULL){
		if(not AEMaterialsShadersEnabled) for(unsigned char i=AEShaderTextureUnitCount;i--;){
			glActiveTexture(GL_TEXTURE0+i);
			if(i) glDisable(GL_TEXTURE_2D);
			AETextureBind(0);
		}
		AEShaderTextureUnitCount=0;
		if(AEMaterialsShadersEnabled) glUseProgram(0);
		return;
	}
	
	if(AEMaterialsShadersEnabled){
		glUseProgram(material->program);
		if(material->timeUniform) glUniform1f((GLint)material->timeUniform-1,AEMaterialTime*0.001);
	}
	//It's designed this way so that the last unit accessed is 0, so I have one less state to change
	for(unsigned char i = material->texUnitCount; i--; /*Intentionally nothing here*/){
		glActiveTexture(GL_TEXTURE0+i);
		if(not AEMaterialsShadersEnabled) glEnable(GL_TEXTURE_2D);
		AETextureBind(material->textures[i]);
		if(AEMaterialsShadersEnabled && material->textureUniforms) glUniform1i((GLint)material->textureUniforms[i]-1,i);
	}
	//printf("Binding %s\n",AENodeTypesGet(AENodeTypeGet(node))->name);
	AEShaderTextureUnitCount=material->texUnitCount;
}

void AEMaterialBindLightmapEXT(AEMaterial* material,AETexture lightmap){
	unsigned char i = material->texUnitCount+1;
	
	if(not lightmap){
		glActiveTexture(GL_TEXTURE0+i);
		AETextureBind(0);
		if(not AEMaterialsShadersEnabled) glDisable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0);
	}

	glActiveTexture(GL_TEXTURE0+i);
	if(not AEMaterialsShadersEnabled) glEnable(GL_TEXTURE_2D);
	AETextureBind(material->textures[i]);
	if(AEMaterialsShadersEnabled) glUniform1i((GLint)material->lightmap-1,i);
	glActiveTexture(GL_TEXTURE0);
}

void AEMaterialShaderSetMulti(AEMaterial* material,const char** vshadertextarray,const char** fshadertextarray){
	if(AEMaterialsShadersEnabled==false) return;
	
	if(vshadertextarray){
		if(material->vshader){
			glDeleteShader(material->vshader);
			material->vshader=0;
		}
		material->vshader=glCreateShader(GL_VERTEX_SHADER);
		size_t length=0;
		while(vshadertextarray[length]) length++;
		glShaderSource(material->vshader, length, (const GLchar**)vshadertextarray,NULL);
		glCompileShader(material->vshader);
		
		//Make it easier for copy/paste
		GLuint object=material->vshader;
		const char* shadertext=vshadertextarray[0];
		const char* shadertype="Vertex";
		
		GLint status=0;
		glGetShaderiv(object,GL_COMPILE_STATUS,&status);
		if(status==0){
			GLint maxlength=0;
			glGetShaderiv(object,GL_INFO_LOG_LENGTH,&maxlength);
			GLint actuallength=0;
			char* log=malloc(maxlength+1);
			glGetShaderInfoLog(object,maxlength,&actuallength,log);
			fprintf(stderr,"%s Shader Error: %s\nWith shader:\n%s\n",shadertype,log,shadertext);
			free(log);
		}
	}
	
	if(fshadertextarray){
		if(material->vshader){
			glDeleteShader(material->fshader);
			material->fshader=0;
		}
		material->fshader=glCreateShader(GL_FRAGMENT_SHADER);
		size_t length=0;
		while(fshadertextarray[length]) length++;
		glShaderSource(material->fshader, length, (const GLchar**)fshadertextarray,NULL);
		glCompileShader(material->fshader);
		
		//Make it easier for copy/paste
		GLuint object=material->fshader;
		const char* shadertext=fshadertextarray[0];
		const char* shadertype="Fragment";
		
		GLint status=0;
		glGetShaderiv(object,GL_COMPILE_STATUS,&status);
		if(status==0){
			GLint maxlength=0;
			glGetShaderiv(object,GL_INFO_LOG_LENGTH,&maxlength);
			GLint actuallength=0;
			char* log=malloc(maxlength+1);
			glGetShaderInfoLog(object,maxlength,&actuallength,log);
			fprintf(stderr,"%s Shader Error: %s\nWith shader:\n%s\n",shadertype,log,shadertext);
			free(log);
		}
	}
	
	if(vshadertextarray==NULL && fshadertextarray==NULL){
		if(material->program){
			glDeleteProgram(material->program);
			material->program=0;
		}
		material->program=glCreateProgram();
		if(material->fshader) glAttachShader(material->program,material->fshader);
		if(material->vshader) glAttachShader(material->program,material->vshader);
		glLinkProgram(material->program);
		
		//Make it easier for copy/paste
		GLuint object=material->program;
		//char* shadertext=NULL;
		//char* shadertype="Program";
		
		GLint status=0;
		glGetProgramiv(object,GL_LINK_STATUS,&status);
		if(status==0){
			GLint maxlength=0;
			glGetProgramiv(object,GL_INFO_LOG_LENGTH,&maxlength);
			GLint actuallength=0;
			char* log=malloc(maxlength+1);
			glGetProgramInfoLog(object,maxlength,&actuallength,log);
			fprintf(stderr,"%s(): Error Linking Shader.\n%s\n",__func__,log);
			//fprintf(stderr,"%s Shader Error: %s\nWith shader:\n%s\n",shadertype,log,shadertext);
			free(log);
		}
	}
	
	free(material->textureUniforms);
	material->textureUniforms=NULL;
	
	if(material->texUnitCount)
		material->textureUniforms=calloc(1,material->texUnitCount*sizeof(GLint));
	for(unsigned char i=material->texUnitCount;i--;){
		material->textureUniforms[i]=glGetUniformLocation(material->program,material->textureNames[i])+1;
	//	printf("Texture %i: %i\n",(int)i,(int)material->textureUniforms[i]);
	}
	
	material->timeUniform=glGetUniformLocation(material->program,"Time")+1;
	material->lightmap=glGetUniformLocation(material->program,"Lightmap")+1;
	//printf("Time: %i\n",(int)material->timeUniform);

}

void AEMaterialShaderSet(AEMaterial* material,const char* vshadertext,const char* fshadertext){
	const char* vshadertextarray[2]={vshadertext,NULL};
	const char* fshadertextarray[2]={fshadertext,NULL};
	AEMaterialShaderSetMulti(material, vshadertext ? vshadertextarray : NULL, fshadertext ? fshadertextarray : NULL);
}

void AEMaterialTextureSet(AEMaterial* material,const char* name,AEImage* image){
	material->texUnitCount++;
	material->textures=realloc(material->textures,material->texUnitCount*sizeof(AETexture));
	material->textureNames=realloc(material->textureNames,material->texUnitCount*sizeof(char*));
	material->textureNames[material->texUnitCount-1]=AEStringDuplicate(name);
	AETexture texture=AEImageToTexture(image);
	material->textures[material->texUnitCount-1]=texture;
	AEImageDelete(image);
}
