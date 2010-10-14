#pragma once
#include "Core.h"
#include "List.h"
#include "VBO.h"

struct AEEnt;
typedef struct AEEnt AEEnt;

typedef void* (*AEEntEventFunc)(AEEnt* ent,int event,void* data);

struct AEEnt{
	float x,y,z;
	AEVec3 rotation;
	AEVBO* vbo;
	unsigned int texture;
	void* data;
	AEList* subents;
	AEEntEventFunc event;//Could also be used to figure out type
	//event could be considered "responder"
	//Events could be considered "questions"
};

#define AEEntEventInit 0
#define AEEntEventRelease 1
#define AEEntEventRender 2

void AEEntsInit(void);

static inline AEVec3 AEEntPositionGet(AEEnt* ent){return *((AEVec3*)ent);}
static inline void AEEntPositionSet(AEEnt* ent,AEVec3 position){*((AEVec3*)ent)=position;}
static inline AEVec3 AEEntRotationGet(AEEnt* ent){return ent->rotation;}
static inline void AEEntRotationSet(AEEnt* ent,AEVec3 rotation){ent->rotation=rotation;}

static inline void* AEEntSignal(AEEnt* ent,int event,void* data){return (*ent->event)(ent,event,data);}
static inline void AEEntSignalChildren(AEEnt* ent,int event,void* data){
	if(ent->subents==NULL) return;
	unsigned int length=AEListLength(ent->subents);
	AEEnt* subents=AEListAsArray(ent->subents,AEEnt);
	for(unsigned int i=0;i<length;i++){
		AEEntSignal(subents+i,event,data);
	}
}
void* AEEntEventDefault(AEEnt* ent,int event,void* data);
void AEEntInit(AEEnt* ent,AEEntEventFunc eventFunc,void* data);
void AEEntAdd(AEEnt* ent,AEEnt subent);
void AEEntRemove(AEEnt* ent,AEEnt subent);

int AEEntEventsGetOrAdd(char* name);
int AEEntEventsGet(char* name);
void AEEntEventsDelete(void);