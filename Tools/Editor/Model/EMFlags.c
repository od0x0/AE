#include "EMFlags.h"

static void EMFlagDeinit(EMFlag* self){
	EUMemoryFree(self->name);
	if(self->type==EMFlagTypeString) EUMemoryFree(self->value.string);
	memset(self, 0, sizeof(EMFlag));
}

void EMFlagsInit(EMFlags* self){
	memset(self, 0, sizeof(EMFlags));
}

void EMFlagsSet(EMFlags* self, char* name, EMFlag* to){//Setting to null means deletion
	EMFlag* flag = (EMFlag*)EMFlagsGet(self, name);
	if(flag){
		if(to){
			if(flag->type==EMFlagTypeString) EUMemoryFree(flag->value.string);
			if(to->type==EMFlagTypeString) flag->value.string=AEStringDuplicate(to->value.string);
			else flag->value=to->value;
			flag->type=to->type;
		}
		else{
			EMFlagDeinit(flag);
			EMFlag top=self->flags[self->flagCount-1];
			*flag=top;
			self->flagCount--;
			self->flags=EUMemoryReallocate(self->flags, sizeof(EMFlag)*self->flagCount);
			return;
		}
	}
	
	if(to and not flag){
		self->flagCount++;
		self->flags=EUMemoryReallocate(self->flags, sizeof(EMFlag)*self->flagCount);
		memset(self->flags+(self->flagCount-1), 0, sizeof(EMFlag));
		self->flags[self->flagCount-1].name=AEStringDuplicate(name);
		EMFlagsSet(self, name, to);
	}
}

//A return of NULL means that it does not exist
const EMFlag* EMFlagsGet(EMFlags* self, char* name){
	for (size_t i=0; i<self->flagCount; i++)
		if(strcmp(self->flags[i].name, name) == 0) return self->flags+i;
	return NULL;
}

void EMFlagsDeinit(EMFlags* self){
	for (size_t i=0; i<self->flagCount; i++)
		EMFlagDeinit(self->flags+i);
	EUMemoryFree(self->flags);
	memset(self, 0, sizeof(EMFlags));
}