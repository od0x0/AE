/*
Writen by Oliver Daids, 2009.

An Engine Core.
*/

#ifndef AEv1_h
#define AEv1_h
#ifdef __cplusplus
extern "C" {
//#error C++ Warning: STOP!!!!  Foul language is not permitted beyond this point.
#endif
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#pragma pack(push, 1)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///							Core
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////	types	///////////////////////////////////////
typedef struct{float x,y;}AEVec2;
typedef struct{float x,y,z;}AEVec3;
typedef struct{float x,y,z,w;}AEVec4;
typedef struct{int x,y;}AEVec2i;
typedef struct{int x,y,z;}AEVec3i;
typedef struct{int x,y,z,w;}AEVec4i;

typedef struct{unsigned int v[3],n[3],t[3];}AEFace;
typedef struct{
	struct{unsigned int v,f,t,n;}count;
	AEVec3* v;
	AEVec2* t;
	AEVec3* n;
	AEFace* f;
	void* aux;
}AEMesh;

typedef struct{//YadaYadaYada Packing
	AEVec2 t;
	AEVec3 v;
}AEVBOVert;

typedef struct{//YadaYadaYada Packing
	AEVec2 t;
	AEVec3 n,v;
}AEVBOVertWithNormal;

typedef struct{
	unsigned int vbo,nbo,ibo,vcount,icount,vallocated,iallocated,*indices;
	AEVec3* n;
	AEVBOVert* verts;
	char hasNormals;
}AEVBO;

typedef struct AEObject{
	//Position Info
	float x,y,z;
	AEVec3 rotation;
	//Essential data
	void (*event)(struct AEObject* object,int event,void* eventData);
	void* model;
	unsigned int texture;
	//Physical Body Data
	AEVec3 size,velocity;
	float mass;
	//Auxilary, for when it is easier not to extend this "class"
	void* aux;
	char isStatic;
}AEObject;

#define AEObjectEventInit 1
#define AEObjectEventRender 2
#define AEObjectEventCollide 3
#define AEObjectEventRelease 4
#define AEObjectEventRenderFromCompile 5

#define AEObjectDynamic 0
#define AEObjectStatic 1
#define AEObjectCompiled 2

//Types that are only used for a singleton

typedef struct{
	float x,y,z;
	AEVec3 rotation;
	//View data
	float fov,near,far;
}AECameraType;

typedef struct{
	int w,h;
	int r,g,b,a,stencil,depth,inFullscreen;
}AEScreenType;
///////////////////// Variables /////////////////

extern AECameraType AECamera;

extern AEScreenType AEScreen;

extern AEVec2i AEMouse;

extern AEObject** AEObjects;

extern unsigned int AEObjectCount;

extern int AEBlockKeyInput;

extern unsigned char AEVBOAddAllocateFreq;

extern unsigned int AETextureLoadFlags;

//////////////////////////////////	prototypes	////////////////////////////////////


/////////   
void AEInit(char* title,int w,int h);
void AEStart(void (*func)(float));
void AEQuit(void);
void AERefreshViewport(char in2d);
/////////
AEObject* AEObjectNew(void (*event)(AEObject* o,int event,void* data),void* data);
int AEObjectsCollide(AEObject* o,AEObject* o2);
void AEObjectDefaultEvent(AEObject* o,int event,void* data);
void AEObjectsSignal(int event,void* data);
void AEObjectsPhysics(float step,AEVec3 globalForce);
void AEObjectDelete(AEObject* o);
/////////
int AEKey(int key);
int AEMouseButton(char button);
void AEPollInput(void);
/////////
unsigned int AETextureLoad(const char* filename);
void AETextureBind(unsigned int texture);
void AETextureDelete(unsigned int texture);
/////////		I would rather you not use AEMeshes for rendering, mainly for loading .objs
 AEMesh* AEMeshLoad(const char* filename);
 void AEMeshDrawRaw(AEMesh* m);//Slow!!!!  Immediate mode, mainly for prototyping without much fuss
 AEVBO* AEMeshAsVBO(AEMesh* m,char isStatic,char hasNormals);
 void AEMeshDelete(AEMesh* m);
////////
AEVBO* AEVBOLoad(const char* filename,int isStatic,int hasNormals);	//Wraps AEMeshes for you
void AEVBOAdd(AEVBO* vbo,AEVBOVertWithNormal* v);
void AEVBODraw(AEVBO* vbo);
void AEVBOCompile(AEVBO* vbo,unsigned int* usages);
void AEVBODelete(AEVBO* vbo);
/////////
/////////
AEVec3 AEAngleTo(float x,float y,float z);  //Angle from origin (Simply masks a few atanf's)
void AEMoveXZ(void* object,float x,float z);
/* object's type must begin like

typedef struct{
	float x,y,z;
	AEVec3 rotation;
	...whatever...
*/
unsigned int AELinearSearch_internal(void* value,void* array,int length,int size);
#define AELinearSearch(val,array,len) AELinearSearch_internal(val,array,len,sizeof(*val))

#pragma pack(pop)

#ifdef __cplusplus
}//extern "C" 
#endif
#endif

/*
TODO:
-More Flexible/Simple VBO System
-Speed
-Simpler

I removed a lot of features that could be added separately later on, 
AEObjectEventRenderFromCompile, and the object states are remnants of that.
*/