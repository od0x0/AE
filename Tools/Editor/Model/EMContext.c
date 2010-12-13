//Internal header
#include "EMContext.h"
#include "EUMemory.h"

typedef struct{
	size_t refcount;
	EMPropertyType type;
	union{
		unsigned int uinteger;
		float real;
		char* string;
		struct{size_t size; void* bytes;} blob;
	}data;
}EMProperty;

typedef struct EMEdit EMEdit;

struct EMNode{
	EMContext* context;
	EMNodeType type;
	AEArray(EMEdit*) branches;
	EMEdit* currentBranch;
};

struct EMContext{
	AEArray(EMNode) nodes;
};

//Managing Properties.

EMProperty* EMPropertyNew(EMPropertyType type, void* data, size_t size){
	EMProperty* self=EUMemoryAllocate(sizeof(EMProperty));
	self->type=type;
	
	switch (self->type) {
		case EMPropertyTypeNone:
			break;
		case EMPropertyTypeUnsignedInt:
			self->data.uinteger=*(unsigned int*)data;
			break;
		case EMPropertyTypeFloat:
			self->data.real=*(float*)data;
			break;
		case EMPropertyTypeString:
			self->data.string=AEStringDuplicate(data);
			break;
		case EMPropertyTypeVec4:
			size=sizeof(AEVec4);
			//Fallthrough intended.
		case EMPropertyTypeBLOB:
			self->data.blob.size=size;
			self->data.blob.bytes=memcpy(EUMemoryAllocate(size), data, size);
			break;
		default:
			break;
	}
	
	return self;
}

void EMPropertyRead(EMProperty* self, EMPropertyType* typeOut, void* dataOut, size_t* sizeOut){
	if(typeOut) *typeOut=self->type;
	
	size_t size = 0;
	
	switch (self->type) {
		case EMPropertyTypeNone:
			break;
		case EMPropertyTypeUnsignedInt:
			size=sizeof(unsigned int);
			if(dataOut) *(unsigned int*)dataOut=self->data.uinteger;
			break;
		case EMPropertyTypeFloat:
			size=sizeof(float);
			if(dataOut) *(float*)dataOut=self->data.real;
			break;
		case EMPropertyTypeString:
			size=strlen(self->data.string)+1;
			if(dataOut) memcpy(dataOut, self->data.string, size);
			break;
		case EMPropertyTypeVec4:
			size=sizeof(AEVec4);
			//Fallthrough intended.
		case EMPropertyTypeBLOB:
			size=self->data.blob.size;
			if(dataOut) memcpy(dataOut, self->data.blob.bytes, size);
			break;
		default:
			break;
	}
	
	if(sizeOut) *sizeOut=size;
}

void EMPropertyDelete(EMProperty* self){
	if(not self) return;
	switch (self->type) {
		case EMPropertyTypeString:
			free(self->data.string);
			break;
		case EMPropertyTypeVec4:
			//Fallthrough intended.
		case EMPropertyTypeBLOB:
			EUMemoryFree(self->data.blob.bytes);
			break;
		default:
			break;
	}
	EUMemoryFree(self);
}


//Managing Edits.

struct EMEdit{
	EMEdit* last;
	char* name;
	EMProperty* value;
};

static EMEdit* EMEditSearch(EMEdit* self, const char* name){
	if(not self) return NULL;
	if(strcmp(self->name, name)==0) return self;
	return EMEditSearch(self->last, name);
}

static EMEdit* EMEditNew(EMEdit* last, const char* name, EMPropertyType type, void* data, size_t size){
	EMEdit* self = EUMemoryAllocate(sizeof(EMEdit));
	self->last = last;
	self->name = AEStringDuplicate(name);
	self->value=EMPropertyNew(type, data, size);
}

static void EMEditDelete(EMEdit* self){
	if(not self) return;
	EMEditDelete(self->last);
	free(self->name);
	EMPropertyDelete(self->value);
	EUMemoryFree(self);
}

//Managing Nodes.

EMNode* EMNodeNewWithContext(EMContext* context, EMNodeType type){
	EMNode node;
	memset(& node, 0, sizeof(EMNode));
	node.context=context;
	node.type=type;
	AEArrayAddBytes(& node.context->nodes, & node);
	size_t i=AEArrayLength(& node.context->nodes)-1;
	AEArrayInit(& node.branches);
	return AEArrayAsCArray(& node.context->nodes)+i;
}

void EMNodeUndo(EMNode* self){
	
}

void EMNodeRedo(EMNode* self){
	
}

void EMNodePropertyGet(EMNode* self, const char* name, void* data, size_t* size, EMPropertyType* type){
	EMEdit* edit = EMEditSearch(self->currentBranch, name);
	EMPropertyRead(edit->value, type, data, size);
}

void EMNodePropertySet(EMNode* self, const char* name, void* data, size_t size, EMPropertyType type){
	self->currentBranch = EMEditNew(self->currentBranch, name, type, data, size);
}

void EMNodeDelete(EMNode* self){
	if(not self) return;
	size_t length=AEArrayLength(& self->branches);
	for (size_t i=0; i<length; i++) {
		EMEditDelete(AEArrayAsCArray(& self->branches)[i]);
	}
	AEArrayDeinit(& self->branches);
	memset(self, 0, sizeof(EMNode));
	AEArrayRemoveBytes(& self->context->nodes, self);
}