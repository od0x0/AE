#include "EUExceptionCatcher.h"

void EUExceptionCatcherInitWithParent(EUExceptionCatcher* self, EUExceptionCatcher* parent){
	memset(self, 0, sizeof(EUExceptionCatcher));
	self->parent=parent;
}

bool EUExceptionCatcherTry(EUExceptionCatcher* self){
	return setjmp(self->jumpBuffer);
}

bool EUExceptionCatcherCaughtExceptionIsOfType(EUExceptionCatcher* self, char* type){
	if(self->type == NULL) return false;
	if(type == NULL) return true;
	if(strcmp(type, self->type) == 0) return true;
	return false;
}

void EUExceptionCatcherThrow(EUExceptionCatcher* self, char* type, void* userdata, const char* format, ...){
	va_list args;
	va_start(args, format);
	size_t length=vsnprintf(NULL, 0, format, args)+1;
	char* message=EUMemoryAllocate(length);
	vsnprintf(self->message, length, format, args);
	va_end(args);
	
	if(self == NULL){
		fprintf(stderr, "[Exception of type \"%s\"]: %s\n", type, message);
		EUMemoryFree(message);
		abort();
	}
	
	self->type=type;
	self->message=message;
	self->userdata=userdata;
	
	longjmp(self->jumpBuffer, 1);
}

void EUExceptionCatcherDeinit(EUExceptionCatcher* self){
	EUMemoryFree(self->message);
	memset(self, 0, sizeof(EUExceptionCatcher));
}