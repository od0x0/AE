#pragma once
#include "AE.h"
#include "EUMemory.h"

typedef enum{
	EMNodeTypeNone
}EMNodeType;

typedef enum{
	EMNodePropertyTypeNone,
	EMNodePropertyTypeReal32,
	EMNodePropertyTypeUInt32,
	EMNodePropertyTypeUInt8
}EMNodePropertyType;

typedef struct EMNodeEdit EMNodeEdit;

EMNodeEdit* EMNodeEditNew(EMNodeEdit* self, const char* name, void* data, size_t count, EMNodePropertyType type);
EMNodeEdit* EMNodeEditValueGet(EMNodeEdit* self, void* data, size_t* count, EMNodePropertyType* type);
EMNodeEdit* EMNodeEditIntersection(EMNodeEdit* self, EMNodeEdit* other);
EMNodeEdit* EMNodeEditLookup(EMNodeEdit* self, const char* name);
void EMNodeEditDelete(EMNodeEdit* self);

typedef struct EMNode EMNode;
struct EMNode{
	EMNodeEdit* edit;
};

EMNode* EMNodeNew(void);
void EMNodeEditSet(EMNode* self, EMNodeEdit* edit);
EMNodeEdit* EMNodeEditGet(EMNode* self);
void EMNodeDelete(EMNode* self);
