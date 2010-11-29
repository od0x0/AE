#pragma once
#include "EMNode.h"

typedef struct{
	VNodeID verseNode;
	AEVec3 min, max;
	EMFlags flags;
	AEArray(EMNode) nodes;
}EMMap;

size_t EMMapNodeCount(EMMap* self);
EMNode* EMMapNodeGet(EMMap* self, EMNodeID index);
EMNode* EMMapNodeForVerseNodeID(EMMap* self, VNodeID verseID);