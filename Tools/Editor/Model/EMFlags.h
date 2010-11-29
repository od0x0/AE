#pragma once
#include "AE.h"
#include "EUMemory.h"

//A tightfitting array of key/value pairs, O(n) lookup time.

typedef struct{
	char* name;
	enum{
		EMFlagTypeNumber,
		EMFlagTypeString
	}type;
	union{
		char* string;
		double number;
	}value;
}EMFlag;

typedef struct{
	//Read only!
	//This is going to be a tight fitting array, so we don't need to use AEArray
	EMFlag* flags;
	size_t flagCount;
}EMFlags;

void EMFlagsInit(EMFlags* self);
void EMFlagsSet(EMFlags* self, char* name, EMFlag* flag);//Setting to null means deletion
static inline void EMFlagsSetNumber(EMFlags* self, char* name, double number){
	EMFlag flag={.name=NULL, .type=EMFlagTypeNumber, .value={.number=number}};
	EMFlagsSet(self, name, &flag);
}
static inline void EMFlagsSetString(EMFlags* self, char* name, char* string){
	EMFlag flag={.name=NULL, .type=EMFlagTypeString, .value={.string=string}};
	EMFlagsSet(self, name, &flag);
}
//A return of NULL means that it does not exist
const EMFlag* EMFlagsGet(EMFlags* self, char* name);
static inline double EMFlagsGetNumber(EMFlags* self, char* name, double number){
	const EMFlag* flag=EMFlagsGet(self, name);
	if(flag->type==EMFlagTypeString) return atof(flag->value.string);
	return flag->value.number;
}
static inline char* EMFlagsGetString(EMFlags* self, char* name, char* string){
	const EMFlag* flag=EMFlagsGet(self, name);
	if(flag->type==EMFlagTypeString) return flag->value.string;
	return NULL;
}
void EMFlagsDeinit(EMFlags* self);