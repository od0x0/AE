#include "SDL.h"
//Textures are in Core.h
#include "Core.h"
//VAs are in VA.h
#include "VA.h"

int AESDLKey(int key);
int AESDLMouseButton(char button);

//Yes, globals are bad, but this is a simple example of something, not production-ready software.
AETexture Texture;
AEVA VertexArray, IndexArray;


void FrameUpdate(AEContext* context, double timeSinceLastCall, void* arg){
	//Binds the texture, obviously. (In most cases, it just wraps glBindTexture(GL_TEXTURE_2D, ...), so it works with multiple texunits the same as glBindTexure)
	AETextureBind(Texture);
	
	/*
	//The generic version will work most of the time, but it's not recommended for IndexArrays
	AEVABind(& VertexArray);
	AEVABind(& IndexArray);
	//^ Will work just fine, however, unbinding will not be possible, and it is slower than the more specific versions
	*/
	
	AEVABindIndices(& IndexArray);
	AEVABindInterleavedTNV(& VertexArray);
	//AEVADraw takes the range of indices to render
	AEVADraw(0, IndexArray.length);
	//Unbind our index array after we are done by passing NULL, this works for any of the AEVABind* functions, minus the generic one AEVABind
	AEVABindIndices(NULL);
	//This unbinds the vertex array
	AEVABindInterleavedTNV(NULL);
	//Unbind our texture
	AETextureBind(0);
}

void Cleanup(void){
	AEVADeinit(& VertexArray);
	AEVADeinit(& IndexArray);
	AETextureDelete(Texture);
}

int main(int argc, char** argv){
	//AEContextActiveGet()->inFullscreen=true;
	AEContextInit(NULL, "A Simple Window", 800, 500);
	
	//Move the camera back 7 units (OpenGL coordinates, +x is right, +y is up, -z is forward)
	AECameraPositionSet(AECameraActiveGet(), 0, 0, 7);
	
	//Load the texture with default flags (Usually Mipmaps and DXT compression are the defaults)
	Texture=AETextureLoad("Data/Texture.png");
	/*
	//You could also be specific with the flags and use
	AETextureLoadWithFlags("Data/Texture", AETextureLoadFlagDXTCompression | AETextureLoadFlagMipmaps);
	*/
	
	//We have to initialize the VertexArray and IndexArray before use
	//2nd parameter is if it is designed for indices
	//3rd parameter is the method of storage (AEVAVBOTypeStatic, AEVAVBOTypeDynamic, AEVAVBOTypeStream, AEVAVBOTypeNone)
	//4th parameter is the number of texture coords per vertex
	//5th parameter is the dataformat of everything else
	AEVAInit(& VertexArray, false, AEVAVBOTypeStatic, 1, AEVADataFormat3NF3VF);
	AEVAInit(& IndexArray, true, AEVAVBOTypeStatic, 0, 0);
	
	//Load the .obj model into the VertexArray, this is usually pretty slow, if you want speed, use the serialization routines
	AEVALoadFromObj(& VertexArray, & IndexArray, "Data/Cube.obj");
	
	atexit(Cleanup);
	AEContextCallbackSet(NULL, AEContextCallbackFrameUpdate, FrameUpdate, NULL);
	
	AEContextRun(NULL);
	return 0;
}