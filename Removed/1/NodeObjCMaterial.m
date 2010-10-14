#include "../NodeObjC.h"

static unsigned int AEShaderTextureUnitCount=0;

unsigned int AEShadersEnabled=0;

@implementation AENodeMaterial

-(void)dealloc{
	for(int i=0;i<material->texUnitCount;i++) AETextureDelete(textures[i]);
	free(textures);
	free(textureUniforms);
	glDeleteShader(fshader);
	glDeleteShader(vshader);
	glDeleteShader(program);
}

-(void)render{
	if(AEShadersEnabled) glUseProgram(program);
	//It's designed this way so that the last unit accessed is 0, so I have one less state to change
	for(unsigned char i = texUnitCount; i--; /*Intentionally nothing here*/){
		glActiveTexture(GL_TEXTURE0+i);
		AETextureBind(textures[i]);
		if(AEShadersEnabled && textureUniforms) glUniform1i(textureUniforms[i],i);
	}
	
	AEShaderTextureUnitCount=texUnitCount;
}

-(void)setTextureWithName:(char*)name withImage:(AEImage*)image{
	texUnitCount++;
	textures=realloc(textures,texUnitCount*sizeof(unsigned int));
	textures[texUnitCount-1]=AEImageToTexture(image);
	AEImageDelete(image);
}

-(void)setShaderWithVertex:(char*)vertexProgram withFragment:(char*)fragmentProgram{
		if(AEShadersEnabled==0) return;
	
	if(vertexProgram){
		if(vshader){
			glDeleteShader(vshader);
			vshader=0;
		}
		vshader=glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vshader, 1, (const GLchar**)&vertexProgram,NULL);
		glCompileShader(vshader);
	}
	
	if(fragmentProgram){
		if(vshader){
			glDeleteShader(fshader);
			fshader=0;
		}
		fshader=glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(fshader, 1, (const GLchar**)&fragmentProgram,NULL);
		glCompileShader(fshader);
	}
	
	if(vertexProgram==NULL && fragmentProgram==NULL){
		if(program){
			glDeleteProgram(program);
			program=0;
		}
		program=glCreateProgram();
		if(fshader) glAttachShader(program,fshader);
		if(vshader) glAttachShader(program,vshader);
		glLinkProgram(program);
	}
	
	free(textureUniforms);
	textureUniforms=NULL;
	
	if(texUnitCount)
		textureUniforms=calloc(1,texUnitCount*sizeof(unsigned int));
	for(unsigned char i=texUnitCount;i--;){
		char uniformName[256]={'t','e','x','t','u','r','e',i+'0','\0'};
		textureUniforms[i]=glGetUniformLocation(program,uniformName);
	}
}

@end

void AENodeMaterialBind(AENodeMaterial* material){
	if(material==NULL){
		if(AEShadersEnabled==0) for(unsigned char i=AEShaderTextureUnitCount;i--;){
			glActiveTexture(GL_TEXTURE0+i);
			AETextureBind(0);
		}
		AEShaderTextureUnitCount=0;
		if(AEShadersEnabled) glUseProgram(0);
		return;
	}
	[material render];
}