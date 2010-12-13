#pragma once
#include "AE.h"

typedef struct EMContext EMContext;
typedef struct EMNode EMNode;
typedef enum{
	EMNodeTypeNone,
	EMNodeTypeEntity,
	EMNodeTypeCamera,
	EMNodeTypeLight,
	EMNodeTypeImage,
	EMNodeTypeMesh,
	EMNodeTypeMaterial,
	EMNodeTypeSoundEmitter,
	EMNodeTypeParticleEmitter
}EMNodeType;
typedef enum{
	EMPropertyTypeNone,
	EMPropertyTypeUnsignedInt,
	EMPropertyTypeFloat,
	EMPropertyTypeString,
	EMPropertyTypeVec4,
	EMPropertyTypeBLOB
}EMPropertyType;

EMContext* EMContextNew(void);
void EMContextDelete(EMContext* self);

EMNode* EMNodeNewWithContext(EMContext* self, EMNodeType type);
void EMNodeUndo(EMNode* self);
void EMNodeRedo(EMNode* self);
void EMNodePropertyGet(EMNode* self, const char* name, void* data, size_t* size, EMPropertyType* type);
void EMNodePropertySet(EMNode* self, const char* name, void* data, size_t size, EMPropertyType type);
void EMNodeDelete(EMNode* self);