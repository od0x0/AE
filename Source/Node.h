#pragma once
#include "HeaderStart.h"
#include "Core.h"

struct AENode;
typedef struct AENode AENode;

typedef void* (*AENodeEventFunc)(AENode* node,int event,void* data);

struct AENode{
	float x,y,z;
	AEVec3 rotation;
	void* data;
	AENodeEventFunc event;
};

#define AENodeEventInit 0
#deinfe AENodeEventRelease 1

void* AENodeEventDefault(AENode* node,int event,void* data);

int AENodeEventsGetOrAdd(char* name);
int AENodeEventsGet(char* name);
void AENodeEventsDelete(void);

#include "HeaderEnd.h"