#include "../lua/lua.h"
#include "../lua/lauxlib.h"
#include "../lua/lualib.h"
#include "../Core.h"

lua_State* AELuaNew(int shouldLoadLibraries){
	lua_State *L = lua_open();
	if(shouldLoadLibraries) luaL_openlibs(L);
	return L;
}

void AELuaRun(lua_State* L,char* file){
	//Gotta love lazy operators here
	int error = luaL_loadfile(L,file) || lua_pcall(L, 0, 0, 0);
	if (error) {
		fprintf(stderr, "Lua Error: %s\n", lua_tostring(L, -1));
		lua_pop(L, 1);
	}
}

void AELuaAddFunction(lua_State* L,char* name,void* func){
	lua_register(L,name,func);
}

void AELuaAddLibrary(lua_State* L,char* libname,const luaL_reg* funcs){
	//An empty name is equal to a NULL as far as we care
	if(libname && libname[0]=='\0') libname=NULL;
	
	//If it has no name, then we put it in the global scope
	if(libname==NULL) lua_pushvalue(L, LUA_GLOBALSINDEX);
		
    luaL_register(L, libname, funcs);
	
	//We have to pop the value we pushed
	if(libname==NULL) lua_pop(L,1);
}