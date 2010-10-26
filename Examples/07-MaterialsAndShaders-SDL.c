#include "SDL.h"
//Textures are in Core.h
#include "AECore.h"
//VAs are in VA.h
#include "AEVA.h"
#include "AEMaterial.h"

int AESDLKey(int key);
int AESDLMouseButton(char button);

//Yes, globals are bad, but this is a simple example of something, not production-ready software.
AEMaterial* Material;
AEVA VertexArray, IndexArray;


void FrameUpdate(AEContext* context, double timeSinceLastCall, void* arg){
	//Advance the global material time clock
	AEMaterialsTimeSet(timeSinceLastCall + AEMaterialsTimeGet());
	
	//Bind material
	AEMaterialBind(Material);
	
	AEVABindIndices(& IndexArray);
	AEVABindInterleavedTNV(& VertexArray);
	AEVADraw(0, IndexArray.length);
	AEVABindIndices(NULL);
	AEVABindInterleavedTNV(NULL);
	
	//Unbind material
	AEMaterialBind(NULL);
}

void Cleanup(void){
	AEVADeinit(& VertexArray);
	AEVADeinit(& IndexArray);
	AEMaterialDelete(Material);
}

int main(int argc, char** argv){
	printf("\n\n\nDo You Want Shaders? y/n \n");
	//AEMaterialsShadersEnabledSet globally enables shaders
	AEMaterialsShadersEnabledSet('y'==getc(stdin));
	
	//AEContextActiveGet()->inFullscreen=true;
	AEContextInit(NULL, "A Simple Window", 800, 500);
	
	AECameraPositionSet(AECameraActiveGet(), 0, 0, 7);
	
	Material=AEMaterialNew();
	//Set the first material slot to the name "Diffuse" with the contents of the image we load into it
	AEMaterialTextureSet(Material, "Diffuse", AEImageLoad("Data/Texture.png"));
	//Give the material a shader (GLSL)
	char* fragmentShader=
	"uniform sampler2D Diffuse;\n"//Hey, remember this name?  It's the same name we gave the texture when we loaded it into the material.
	"void main(){\n"
	"	gl_FragColor = gl_Color * texture2D(Diffuse, gl_TexCoord[0].st);\n"
	"}\n"
	;
	char* vertexShader=
	"uniform float Time;\n"
	"void main(){\n"
	"	gl_FrontColor=gl_Color * vec4(sin(Time*1000.0));\n"
	"	gl_Position=ftransform();\n"
	"	gl_TexCoord[0] =gl_MultiTexCoord0;\n"
	"}\n"
	;
	AEMaterialShaderSet(Material,vertexShader,fragmentShader);
	AEMaterialShaderSet(Material,NULL,NULL); // NULL, NULL tells it to compile what it's collected so far
	
	AEVAInit(& VertexArray, false, AEVAVBOTypeStatic, 1, AEVADataFormat3NF3VF);
	AEVAInit(& IndexArray, true, AEVAVBOTypeStatic, 0, 0);
	
	AEVALoadFromObj(& VertexArray, & IndexArray, "Data/Cube.obj");
	
	atexit(Cleanup);
	AEContextCallbackSet(NULL, AEContextCallbackFrameUpdate, FrameUpdate, NULL);
	
	AEContextRun(NULL);
	return 0;
}