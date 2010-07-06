#include "../LuaBinding.h"
#include "../LuaLibs.h"
#include "../Ent.h"
/////////////////////////////////////////////////////////////////////
/*
typedef struct{
	lua_State* L;
	
}AELuaEntSubData;

static void* AELuaEntEvent(AEEnt* ent,int event,void* data){
	switch(event){
		case AEEntEventInit:{
			AELuaEntSubData* subdata=calloc(1,sizeof(AELuaEntSubData));
			subdata->L=AELuaNew(0);
			AELuaEntSetup(subdata->L);
			
			ent->data=subdata;
			}break;
		case AEEntEventRelease:{
			AELuaEntSubData* subdata=ent->data;
			lua_close(subdata->L);
			free(subdata);
			ent->data=NULL;
			}break;
	}
	return AEEntDefautEvent(ent,event,data);
}
/////////////////////////////////////////////////////////////////////

static int AELuaEnt_LuaEntEvent(lua_State* L){
	
}

static int AELuaEnt_LuaEntEvent(lua_State* L){

}

static const struct luaL_reg AELuaEntLib [] = {
	{NULL, NULL}
};

///////////////////////////////////////////////////////////////////////
*/
/*
static int AELuaEntAccessors_position_get(lua_State* L){
	AEEnt* ent=lua_touserdata(L,-4);
	lua_pushnumber(L,ent->x);
	return 1;
}

static int AELuaEntAccessors_position_set(lua_State* L){
	AEEnt* ent=lua_touserdata(L,-4);
	ent->x=lua_tonumber(L,-2);
	return 0;
}

static const struct luaL_reg AELuaEntAccessors [] = {
	{"position_get",},
	{NULL, NULL}
};
*/
void AELuaEntSetup(lua_State* L){
	//lua_newtable(L);
	//luaL_register(L,NULL,);
	//lua_setfield(L,LUA_GLOBALINDEX,"AEEnt_accessors");
	/*AELuaAddLibrary(L,"",AELuaEntLib);
	lua_pushlightuserdata(L,AELuaEntEvent);
	lua_setfield(L,LUA_GLOBALINDEX,"AELuaEntType");
	lua_pushlightuserdata(L,AEEntDefautEvent);
	lua_setfield(L,LUA_GLOBALINDEX,"AEEntDefautType");*/
}

///////////