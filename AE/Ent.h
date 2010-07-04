#pragma once
#include "HeaderBegin.h"
#include "Core.h"
#include "List.h"

struct AEEnt;
typedef struct AEEnt AEEnt;

typedef void* (*AEEntEventFunc)(AEEnt* ent,int event,void* data);

struct AEEnt{
	float x,y,z;
	AEVec3 rotation;
	void* data;
	AEList* subents;
	AEEntEventFunc event;//Could also be used to figure out type
	//event could be considered "responder"
	//Events could be considered "questions"
};

#define AEEntEventInit 0
#define AEEntEventRelease 1

inline void* AEEntSignal(AEEnt* ent,int event,void* data){return (*ent->event)(ent,event,data);}
inline void AEEntSignalChildren(AEEnt* ent,int event,void* data){
	if(ent->subents==NULL) return;
	unsigned int length=AEListLength(ent->subents);
	AEEnt* subents=AEListAsArray(ent->subents,AEEnt);
	for(unsigned int i=0;i<length;i++){
		AEEntSignal(subents+i,event,data);
	}
}
void* AEEntEventDefault(AEEnt* ent,int event,void* data);
AEEnt AEEntNew(AEEntEventFunc eventFunc,void* data);
void AEEntAdd(AEEnt* ent,AEEnt subent);
void AEEntRemove(AEEnt* ent,AEEnt subent);

int AEEntEventsGetOrAdd(char* name);
int AEEntEventsGet(char* name);
void AEEntEventsDelete(void);

#include "HeaderEnd.h"