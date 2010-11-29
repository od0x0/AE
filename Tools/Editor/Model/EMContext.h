#pragma once
#include "EUMemory.h"
#include "AE.h"
#include "verse.h"
#include "EMNode.h"

typedef struct EMContext EMContext;
typedef struct EMContextEditNode EMContextEditNode;
typedef enum{
	EMContextTagTypeNone,
	EMContextTagTypeFloat,
	EMContextTagTypeString,
	EMContextTagTypeVec4
}EMContextTagType;
typedef struct{
	EMContextTagType type;
	union{
		float floatNumber;
		char* string;
		AEVec4 vec4;
		void* other;
	}value;
}EMContextTag;

struct EMContext{
	//Stores the top of where it was terminated, 0 is the main leaf
	AEArray(EMContextEditNode*) editNodeLeaves;
	EMContextEditNode* currentEditNode;
	size_t highestID;
	VSession verseSession;
	char* connectedServer;
};

void EMContextInit(EMContext* self);
void EMContextFixedUpdate(EMContext* self, double seconds);
void EMContextConnectToServer(EMContext* self, const char* address, const char* username, const char* password);
void EMContextDisconnectFromServer(EMContext* self);
void EMContextDeinit(EMContext* self);
void EMContextDoEdit(EMContext* self, EMNodeID nodeID, const char* name, EMContextTag* value);

//
struct EMContextEditNode{
	//If 0, then it is loaded into memory, and not in a file. (offset of 1)
	//size_t fileOffset;
	size_t ID;
	EMContextEditNode* last;
	EMNodeID target;
	char* name;
	EMContextTag value;
};
void EMContextRevertToEdit(EMContext* self, EMContextEditNode* node);