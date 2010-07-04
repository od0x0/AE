#pragma once
#include "HeaderBegin.h"

typedef struct{
	unsigned int subnodeCount;
	AENode** subnodes;
}AEEnv;

void AEEnvSignalNodes(AEEnv* env,int event,void* data);
void AEEnvAddNode(AEEnv* env,AENode* node);
void AEEnvRemoveNode(AEEnv* env,AENode* node);

#include "HeaderEnd.h"
