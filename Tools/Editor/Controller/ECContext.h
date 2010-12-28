#pragma once
#include "AE.h"
#include "EUMemory.h"
#include "ECOperationList.h"
#include "lua.h"
#include "LuaBinding.h"

typedef struct{
	AETable* operationListTable;
	lua_State* L;
}ECContext;

void ECContextInit(ECContext* self);
void ECContextAddOperation(ECContext* self, const char* listName, const char* name, ECOperationFunc function, void* userdata);
void ECContextDeinit(ECContext* self);