#include "../lua/lua.h"

void* AELuaNew(int shouldLoadLibraries){
	lua_State *L = lua_open();
	if(shouldLoadLibraries) luaL_openlibs(L);
	return L;
}

void AELuaRun(void* state,char* file){
	lua_State* L=state;
	//Gotta love lazy operators here
	int error = luaL_loadfile(L,file) || lua_pcall(L, 0, 0, 0);
	if (error) {
		fprintf(stderr, "Lua Error: %s\n", lua_tostring(L, -1));
		lua_pop(L, 1);
	}
}

void AELuaAddFunction(void* state,char* name,void* func){
	lua_State* L=state;
	lua_register(L,name,func);
}

void AELuaVarSet(){

}