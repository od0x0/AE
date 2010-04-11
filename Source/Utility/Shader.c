#include "../Shader.h"
#include "../Utility.h"

AEShader* AEShaderLoad(char* filename){
	if(filename==NULL){
		return calloc(1,sizeof(AEShader));
	}
	
	FILE* file=fopen(filename,"r");
	if(!file){
		printf("AEShaderLoad(\"%s\"): Could not open file.",filename);
		exit(1);
	}
	
	AEShader* shader=AEShaderLoad(NULL);
	
	char buffer[512]={0};
	
	char buffer2[256]={0};
	char buffer3[256]={0};
	
	int arg=0;
	
	int textureUnitCount=0;
	
	int maxTextureUnits;
	glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB,&maxTextureUnits);
	
	while(!feof(file)){
		fgets(buffer,511,file);
		if(2==sscanf(buffer,"shader %s %s",buffer2,buffer3)){
			int type=GL_FRAGMENT_SHADER;
			if(strcmp(buffer2,"(vertex)")==0) type=GL_VERTEX_SHADER;
			else if(strcmp(buffer2,"(fragment)")==0) type=GL_FRAGMENT_SHADER;
			else{
				printf("AEShaderLoad(\"%s\"): Unknown shader type \"%s\".",filename,buffer2);
				exit(1);
			}
			
			GLuint s=glCreateShader(type);
			char* text=AEFileAsString(buffer3);
			glShaderSource(s, 1, (const GLchar**)&text,NULL);
			free(text);
			glCompileShader(s);
			
			if(type==GL_VERTEX_SHADER) shader->vshader=s;
			else shader->fshader=s;
		}
		else if(1==sscanf(buffer,"textureCount %i",&textureUnitCount)){
			if(textureUnitCount>maxTextureUnits) printf("Warning: Your graphics card doesn't support %i texture units, a maximum of %i",textureUnitCount,maxTextureUnits);
		}
		else if(NULL!=strstr(buffer,"link")){
			shader->program=glCreateProgram();
			glAttachShader(shader->program,shader->fshader);
			glAttachShader(shader->program,shader->vshader);
			glLinkProgram(shader->program);
			for(unsigned char i=textureUnitCount;i--;){
				char uniformName[256]={'t','e','x','t','u','r','e',i+'0','\0'};
				shader->textureUniforms[i]=glGetUniformLocation(shader->program,uniformName);
			}
		}
	}
	fclose(file);
	return shader;
}

void AEShaderBind(AEShader* shader,unsigned char textureUnitCount,unsigned int* textures){
	if(shader==NULL){
		for(unsigned char i=textureUnitCount;i--;){
			glActiveTexture(GL_TEXTURE0+i);
			AETextureBind(0);
		}
		glUseProgram(0);
		return;
	}
	
	glUseProgram(shader->program);
	for(unsigned char i=textureUnitCount;i--;){
		glActiveTexture(GL_TEXTURE0+i);
		AETextureBind(textures[i]);
		if(shader->textureUniforms[i]) glUniform1i(shader->textureUniforms[i],i);
	}
}

void AEShaderDelete(AEShader* shader){
	if(shader==NULL) return;
	glDeleteProgram(shader->program);
	glDeleteShader(shader->vshader);
	glDeleteShader(shader->fshader);
	free(shader->textureUniforms);
	free(shader);
}