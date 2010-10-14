#include "A2RATNodeShader.h"

//static void A2RATNodeTypeShaderInit(A2RATNode* self){}

static void A2RATNodeTypeShaderDeinit(A2RATNode* self,...){
	free(A2RATNodeShaderTextGet(self,1));
}

A2RATNodeType A2RATNodeTypeShader_internal={
	.name="Shader",
	//.init=A2RATNodeTypeShaderInit,
	.deinit=A2RATNodeTypeShaderDeinit,
	.last=NULL
};

A2RATNodeType* A2RATNodeTypeShader(void){
	static bool initialized=false;
	A2RATNodeType* self=&A2RATNodeTypeShader_internal;
	if(not initialized){
		initialized=true;
		A2RATNodeTypesAdd(self);
	}
	return self;
}


void A2RATNodeShaderTextSet(A2RATNode* self, int detaillevel, char* text){
	free(A2RATNodeSlotGet(self, "textdefault"));
	if(text) text=strdup(text);
	
	switch (detaillevel) {
		case 0://Shaderless Fallback
			break;
		default:
			A2RATNodeSlotSet(self, "textdefault", text);
			break;
	}
}

char* A2RATNodeShaderTextGet(A2RATNode* self, int detaillevel){
	switch (detaillevel) {
		case 0://Shaderless Fallback
			return NULL;
		default:
			return A2RATNodeSlotGet(self, "textdefault");
	}
	return NULL;
}

void A2RATNodeShaderTypeSet(A2RATNode* self, A2RATNodeShaderType type){
	char* string=NULL;
	switch (type) {
		case A2RATNodeShaderTypeVertex:
			string="v";
			break;
		case A2RATNodeShaderTypeFragment:
			string="f";
			break;
		case A2RATNodeShaderTypeNone:
		default:
			string=NULL;
			break;
	}
	A2RATNodeSlotSet(self, "shadertype", string);
}

A2RATNodeShaderType A2RATNodeShaderTypeGet(A2RATNode* self){
	A2RATNodeShaderType type=A2RATNodeShaderTypeNone;
	char* string=A2RATNodeSlotGet(self, "shadertype");
	if(type) switch (string[0]) {
		case 'v':
			type=A2RATNodeShaderTypeVertex;
			break;
		case 'f':
			type=A2RATNodeShaderTypeFragment;
			break;
	}

	return type;
}

static void A2RATNodeShaderOptimize(A2RATNode* self){
	//The driver probably does a better optimizing job than we could.
	
}