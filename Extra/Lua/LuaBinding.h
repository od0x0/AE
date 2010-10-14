#pragma once

#include "lua/lua.h"
#include "lua/lauxlib.h"
#include "lua/lualib.h"
#include "Core.h"

lua_State* AELuaNew(int shouldLoadLibraries);

void AELuaRun(lua_State* L,char* file);

void AELuaAddFunction(lua_State* L,char* name,void* func);

//"" or NULL puts it in the global namespace
void AELuaAddLibrary(lua_State* L,char* libname,const luaL_reg* funcs);

#define AELuaPushVec2(L,v) do{lua_newtable(L);\
		lua_pushstring(L,"x"); lua_pushnumber(L,v.x); lua_settable(L,-3);\
		lua_pushstring(L,"y"); lua_pushnumber(L,v.y); lua_settable(L,-3);\
		}while(0)

#define AELuaPushVec3(L,v) do{lua_newtable(L);\
		lua_pushstring(L,"x"); lua_pushnumber(L,v.x); lua_settable(L,-3);\
		lua_pushstring(L,"y"); lua_pushnumber(L,v.y); lua_settable(L,-3);\
		lua_pushstring(L,"z"); lua_pushnumber(L,v.z); lua_settable(L,-3);\
		}while(0)
		
#define AELuaPushVec4(L,v) do{lua_newtable(L);\
		lua_pushstring(L,"x"); lua_pushnumber(L,v.x); lua_settable(L,-3);\
		lua_pushstring(L,"y"); lua_pushnumber(L,v.y); lua_settable(L,-3);\
		lua_pushstring(L,"z"); lua_pushnumber(L,v.z); lua_settable(L,-3);\
		lua_pushstring(L,"w"); lua_pushnumber(L,v.w); lua_settable(L,-3);\
		}while(0)
		
#define AELuaPushTableBegin(L) lua_newtable(L)
#define AELuaPushTableEnd(L) do{}while(0)
#define AELuaPushTableAddField(L,key,valueaction) do{lua_pushstring(L,key); valueaction; lua_settable(L,-3);}while(0)
#define AELuaPushTableAddAtIndex(L,key,valueaction) do{lua_pushinteger(L,key); valueaction; lua_settable(L,-3);}while(0)

#define AELuaReadTableBegin(L,index)
#define AELuaReadTableField(L,tableindex,keyaction,valueaction) do{keyaction;lua_gettable(_L, -2);valueaction;lua_pop(_L, 1);}while(0)