#pragma once
#include "A2RVM.h"
#include "Table.h"
/*
typedef enum{
	A2RATNodeTypeNone,
	
	A2RATNodeTypeStaticEnt,
	A2RATNodeTypeDynamicEnt,
	
	A2RATNodeTypePointLight,
	
	A2RATNodeTypeTexture,
	
	A2RATNodeTypeVertexShader,
	A2RATNodeTypeFragmentShader,
	
	A2RATNodeTypeMaterial
}A2RATNodeType;

*/

typedef struct A2RATNode A2RATNode;
typedef void (*A2RATNodeMessageFunc)(A2RATNode* self,...);
typedef struct A2RATNodeType A2RATNodeType;
struct A2RATNodeType{
	char* name;
	A2RATNodeMessageFunc init;
	A2RATNodeMessageFunc deinit;
	A2RATNodeType* last;
};

A2RATNodeType* A2RATNodeTypeNone(void);

void A2RATNodeTypesAdd(A2RATNodeType* type);
A2RATNodeType* A2RATNodeTypesLookup(char* name);

A2RATNode* A2RATNodeNew(A2RATNodeType* type);
A2RATNode* A2RATNodeRetain(A2RATNode* self);
void A2RATNodeDelete(A2RATNode* self);

A2RATNodeType* A2RATNodeTypeGet(A2RATNode* self);

void* A2RATNodeSlotGet(A2RATNode* self, char* name);
void A2RATNodeSlotSet(A2RATNode* self, char* name, void* value);

void A2RATNodeOptimize(A2RATNode* self);