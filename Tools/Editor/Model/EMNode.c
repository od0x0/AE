#include "EMNode.h"

struct EMNodeEdit{
//Read only
	EMNodePropertyType type;
	size_t count;
	void* data;
	char* name;
	EMNodeEdit* parent;
};

EMNodeEdit* EMNodeEditNew(EMNodeEdit* parent, const char* name, void* data, size_t count, EMNodePropertyType type){
	EMNodeEdit* self=EUMemoryAllocate(sizeof(EMNodeEdit));
	self->parent=parent;
	self->name=AEStringDuplicate(name);
	self->count=count;
	self->type=type;
	size_t typeSize=0;
	switch (type) {
		case EMNodePropertyTypeUInt8:
			typeSize = sizeof(uint8_t);
			break;
		case EMNodePropertyTypeUInt32:
			typeSize = sizeof(uint32_t);
			break;
		case EMNodePropertyTypeReal32:
			typeSize = sizeof(float);
			break;
		default:
			typeSize = 0;
			break;
	}
	self->data=EUMemoryCopy(data, typeSize*count);
	return self;
}

EMNodeEdit* EMNodeEditValueGet(EMNodeEdit* self, void* dataOut, size_t* countOut, EMNodePropertyType* typeOut){
	size_t count=0;
	void* data=NULL;
	EMNodePropertyType type=EMNodePropertyTypeNone;
	EMNodeEdit* parent=NULL;
	if(self){
		count=self->count;
		data=self->data;
		type=self->type;
		parent=self->parent;
	}
	if(countOut) *countOut=count;
	if(typeOut) *typeOut=type;
	size_t typeSize=0;
	switch (type) {
		case EMNodePropertyTypeUInt8:
			typeSize = sizeof(uint8_t);
			break;
		case EMNodePropertyTypeUInt32:
			typeSize = sizeof(uint32_t);
			break;
		case EMNodePropertyTypeReal32:
			typeSize = sizeof(float);
			break;
		default:
			typeSize = 0;
			break;
	}
	if(dataOut and data) memcpy(dataOut, data, typeSize*count);
	return parent;
}

EMNodeEdit* EMNodeEditIntersection(EMNodeEdit* self, EMNodeEdit* other){
	if(not self) return NULL;
	EMNodeEdit* originalOther=other;
	while (other) {
		if(other==self) return self;
		other=other->parent;
	}
	return EMNodeEditIntersection(self->parent, originalOther);
}

EMNodeEdit* EMNodeEditLookup(EMNodeEdit* self, const char* name){
	if(not self) return NULL;
	if(strcmp(self->name, name)==0) return self;
	return EMNodeEditLookup(self->parent, name);
}

void EMNodeEditDelete(EMNodeEdit* self){
	if(not self) return;
	free(self->name);
	EUMemoryFree(self->data);
	EUMemoryFree(self);
}

EMNode* EMNodeNew(void){
	return EUMemoryAllocate(sizeof(EMNode));
}

void EMNodeDelete(EMNode* self){
	if(not self) return;
	EUMemoryFree(self);
}
