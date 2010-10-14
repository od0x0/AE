#pragma once
#include "Core.h"
#include "List.h"
#include "Image.h"
#include "TextBuffer.h"

//Deprecated

//The intermediate step from which we compile down to the RVM, not designed for speed or memory efficiency as this is designed mainly for static scene data

enum {
	A2RNodeTypeNone,
	A2RNodeTypeVFC,
	A2RNodeTypeObject,
	A2RNodeTypeMesh,
	A2RNodeTypeMaterial,
	A2RNodeTypeGroup,
	A2RNodeTypeLight
};

struct A2RNode;
typedef struct A2RNode A2RNode;

void A2RNodeObjectPositionSet(A2RNode* node,float x,float y,float z);
void A2RNodeObjectRotationSet(A2RNode* node,float x,float y,float z);
void A2RNodeObjectScaleSet(A2RNode* node,float scale);
void A2RNodeObjectMeshSet(A2RNode* node,A2RNode* mesh);
void A2RNodeObjectMaterialSet(A2RNode* node,A2RNode* material);
void A2RNodeLightPositionSet(A2RNode* node,float x,float y,float z);
void A2RNodeLightColorSet(A2RNode* node,float r,float g,float b);
void A2RNodeLightAttenuationSet(A2RNode* node,float attenuation);
void A2RNodeLightChildSet(A2RNode* node,A2RNode* child);
void A2RNodeVFCBoundsSet(A2RNode* node,float x,float y,float z,float radius);
void A2RNodeVFCChildSet(A2RNode* node,A2RNode* child);
void A2RNodeMeshVertexAdd(A2RNode* node,char* format,void* data);
void A2RNodeMaterialShaderAdd(A2RNode* node, char* vshader, char* fshader);
void A2RNodeMaterialImageAdd(A2RNode* node,AEImage* image);
void A2RNodeGroupChildAdd(A2RNode* node,A2RNode* child);

A2RNode* A2RNodeListNodeNew(AEList* nodelist, unsigned int type);

AEList* A2RNodeListNew(void);
void A2RNodeListDelete(AEList* nodelist);


//Internal use only

//We could save space by not using a union, but where's the fun in that?
struct A2RNode{
	unsigned int type;
	union{
		struct{
			AEVec3 center;
			float radius;
			A2RNode* child;
		} vfc;
		
		struct{
			AEVec3 center;
			AEVec3 rotation;
			float scale;
			A2RNode* mesh;
			A2RNode* material;
		} object;
		
		struct{
			char* format;
			AEList* uvs;//Of AEVec2
			AEList* colors;//Of AERBGAub
			AEList* normals;//Of AEVec3
			AEList* positions;//Of AEVec3
			unsigned char verticesPerPoly;
		} mesh;
		
		struct{
			AEList* images;//Of AEImage*
			AETextBuffer* vshader;
			AETextBuffer* fshader;
		} material;
		
		struct{
			AEList* children;//Of A2RNode*
		} group;
		
		//struct{AEVec3 center; AEVec3 rotation; float fov, near, far;} camera;
		
		struct{
			AEVec3 center;
			float radius;
			AERGBAub color;//a is attenuation
			A2RNode* child;
		} light;
	}data;
};
