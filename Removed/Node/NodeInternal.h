#pragma once
#include "../Node.h"

#ifdef AENODE_USE_VERSE
#define int32 evint32
#define uint32 evuint32
#define int16 evint16
#define uint16 evuint16
#define int8 evint8
#define uint8 evuint8
#include "../verse/enough.h"
#undef int32
#undef uint32
#undef int16
#undef uint16
#undef int8
#undef uint8
#endif

extern unsigned int AENodesTime;
extern AEList* AENodes;

struct AENodeType;
typedef struct AENodeType AENodeType;

struct AENode{
	char* name;
	//void* aux;
	//unsigned int type;
	AENodeType* type;
	AETable* slots;
	size_t size;
	unsigned int timestamp;//Allows determining whether this node was hashed within this step
#ifdef AENODE_USE_VERSE
	ENode* enode;//For binding to Enough
#endif
	unsigned char refcount;//Eases the problem of memory managment
};

typedef struct{
	AENode super;
	struct{
		AEVec3 position;
		AEVec3 rotation;
	}transform;
	AEList/*<AENode*>*/* renderchildren;
	AEList/*<AENode*>*/* children;
	float radius;
}AENodeEnt;

void AENodeMeshDelete(AENode* node);
void AENodeMeshRender(AENode* node);
//void AENodeMeshStep(AENode* node,float stepInSeconds);

void AENodeMaterialDelete(AENode* node);
void AENodeMaterialRender(AENode* node);
//void AENodeMaterialStep(AENode* node,float stepInSeconds);

void AENodeEntDelete(AENode* node);
void AENodeEntRender(AENode* node);
//void AENodeEntStep(AENode* node,float stepInSeconds);