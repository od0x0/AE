#include "LuaBinding.h"
#include "Core.h"
#include "Camera.h"
#include "luagl.h"
#include "luaglu.h"
/*
static int AELuaCore_TextureBind(lua_State* L){
	unsigned int texture=lua_tointeger(L,-1);
	AETextureBind(texture);
	return 0;
}

static int AELuaCore_TextureDelete(lua_State* L){
	unsigned int texture=lua_tointeger(L,-1);
	AETextureDelete(texture);
	return 0;
}

static int AELuaCore_TextureLoad(lua_State* L){
	const char* filename=lua_tostring(L,-1);
	unsigned int texture=AETextureLoad((char*)filename);
	lua_pushinteger(L,texture);
	return 1;
}

static int AELuaCore_Key(lua_State* L){
	unsigned int button=0;
	if(lua_isstring(L,-1)){
		button=lua_tostring(L,-1)[0];
	}
	else button=lua_tointeger(L,-1);
	int state=AESDLKey(button);
//	printf("AEKey: %i\n",state);
//	exit(0);
	lua_pushboolean(L,state);
	return 1;
}

static int AELuaCore_MouseButton(lua_State* L){
	unsigned int button=lua_tointeger(L,-1);
	int state=AEMouseButton(button);
	lua_pushboolean(L,state);
	return 1;
}

static int AELuaCore_PollInput(lua_State* L){
	AEPollInput();
	return 0;
}

static int AELuaCore_Init(lua_State* L){
	const char* title=lua_tostring(L,-3);
	int w=lua_tointeger(L,-2);
	int h=lua_tointeger(L,-1);
	AEInit((char*)title,w,h);
	return 0;
}

static char AELuaCore_Start_helper_functionName[64];
lua_State* AELuaCore_Start_helper_LState=NULL;
static void AELuaCore_Start_helper(float step){
	
	lua_State* L=AELuaCore_Start_helper_LState;
	lua_getglobal(L,AELuaCore_Start_helper_functionName);
	lua_pushnumber(L,step);

	if (lua_pcall(L, 1, 0, 0)){
		luaL_error(L,"%s\n",lua_tostring(L, -1));
		exit(1);
	}
}

static int AELuaCore_Start(lua_State* L){
	const char* funcname=lua_tostring(L,-1);
	if(lua_isnil(L,-1)){
		AEStart(NULL);
		return 0;
	}
	strncpy(AELuaCore_Start_helper_functionName,funcname,sizeof(AELuaCore_Start_helper_functionName));
	AELuaCore_Start_helper_LState=L;
	AEStart(AELuaCore_Start_helper);
	return 0;
}

static int AELuaCore_Quit(lua_State* L){
	AEQuit();
	return 0;
}

static int AELuaCore_StateActiveGet(lua_State* L){
	//Ignore the old man behind the curtain
	AEState* state=AEStateActiveGet();
	
	lua_newtable(L);
	lua_pushinteger(L,state->w);
	lua_setfield(L,-2,"w");
	lua_pushinteger(L,state->h);
	lua_setfield(L,-2,"h");
	lua_pushinteger(L,state->r);
	lua_setfield(L,-2,"r");
	lua_pushinteger(L,state->g);
	lua_setfield(L,-2,"g");
	lua_pushinteger(L,state->b);
	lua_setfield(L,-2,"b");
	lua_pushinteger(L,state->a);
	lua_setfield(L,-2,"a");
	lua_pushinteger(L,state->stencil);
	lua_setfield(L,-2,"stencil");
	lua_pushinteger(L,state->depth);
	lua_setfield(L,-2,"depth");
	lua_pushinteger(L,state->inFullscreen);
	lua_setfield(L,-2,"inFullscreen");
	lua_pushinteger(L,state->blockKeyInput);
	lua_setfield(L,-2,"blockKeyInput");
	lua_pushinteger(L,state->textureLoadFlags);
	lua_setfield(L,-2,"textureLoadFlags");
	lua_pushnumber(L,state->fov);
	lua_setfield(L,-2,"fov");
	lua_pushnumber(L,state->far);
	lua_setfield(L,-2,"far");
	lua_pushnumber(L,state->near);
	lua_setfield(L,-2,"near");
	AELuaPushVec2(L,state->mouse);
	lua_setfield(L,-2,"mouse");
	
	return 1;
}

static int AELuaCore_StateActiveSet(lua_State* L){
	//Ignore the old man behind the curtain
	AEState state;
	
	lua_getfield(L,-2,"w");
	state.w=lua_tointeger(L,-1);
	lua_pop(L,1);
	
	lua_getfield(L,-2,"h");
	state.h=lua_tointeger(L,-1);
	lua_pop(L,1);
	
	lua_getfield(L,-2,"r");
	state.r=lua_tointeger(L,-1);
	lua_pop(L,1);
	
	lua_getfield(L,-2,"g");
	state.g=lua_tointeger(L,-1);
	lua_pop(L,1);
	
	lua_getfield(L,-2,"b");
	state.b=lua_tointeger(L,-1);
	lua_pop(L,1);
	
	lua_getfield(L,-2,"a");
	state.a=lua_tointeger(L,-1);
	lua_pop(L,1);
	
	lua_getfield(L,-2,"stencil");
	state.stencil=lua_tointeger(L,-1);
	lua_pop(L,1);
	
	lua_getfield(L,-2,"depth");
	state.depth=lua_tointeger(L,-1);
	lua_pop(L,1);
	
	lua_getfield(L,-2,"inFullscreen");
	state.inFullscreen=lua_tointeger(L,-1);
	lua_pop(L,1);
	
	lua_getfield(L,-2,"blockKeyInput");
	state.blockKeyInput=lua_tointeger(L,-1);
	lua_pop(L,1);
	
	lua_getfield(L,-2,"textureLoadFlags");
	state.textureLoadFlags=lua_tointeger(L,-1);
	lua_pop(L,1);
	
	lua_getfield(L,-2,"fov");
	state.fov=lua_tonumber(L,-1);
	lua_pop(L,1);
	
	lua_getfield(L,-2,"far");
	state.far=lua_tonumber(L,-1);
	lua_pop(L,1);
	
	lua_getfield(L,-2,"near");
	state.near=lua_tonumber(L,-1);
	lua_pop(L,1);
	
	lua_getfield(L,-2,"mouse");
		lua_getfield(L,-1,"x");
		state.mouse.x=lua_tonumber(L,-1);
		lua_pop(L,1);
		
		lua_getfield(L,-1,"y");
		state.mouse.y=lua_tonumber(L,-1);
		lua_pop(L,1);
	lua_pop(L,1);
	////Bah
	
	AEStateActiveSet(&state);
	return 0;
}

static const struct luaL_reg AELuaCoreLibCore [] = {
	{"AEInit",AELuaCore_Init},
	{"AEStart",AELuaCore_Start},
	{"AEQuit",AELuaCore_Quit},
	{"AETextureLoad",AELuaCore_TextureLoad},
	{"AETextureBind",AELuaCore_TextureBind},
	{"AETextureDelete",AELuaCore_TextureDelete},
	{"AEKey",AELuaCore_Key},
	{"AEMouseButton",AELuaCore_MouseButton},
	{"AEPollInput",AELuaCore_PollInput},
	{"AEStateActiveSet",AELuaCore_StateActiveSet},
	{"AEStateActiveGet",AELuaCore_StateActiveGet},
	{NULL, NULL}
};
///////////////////////////////////////////////////////////////////////

static int AELuaCore_CameraPositionGet(lua_State* L){
	AEVec3 v=AECameraPositionGet(AECameraActiveGet());
	AELuaPushVec3(L,v);
	return 1;
}

static int AELuaCore_CameraPositionSet(lua_State* L){
	AEVec3 v;
	//We skip the first argument, it's unused for now
//	lua_getfield(L,-1,"x");
	v.x=lua_tonumber(L,-3);
//	lua_pop(L,1);
		
//	lua_getfield(L,-1,"y");
	v.y=lua_tonumber(L,-2);
//	lua_pop(L,1);
	
	//lua_getfield(L,-1,"z");
	v.z=lua_tonumber(L,-1);
//	lua_pop(L,1);
	
	AECameraPositionSet(AECameraActiveGet(),v.x,v.y,v.z);
	
	return 0;
}

static int AELuaCore_CameraRotationGet(lua_State* L){
	AEVec3 v=AECameraRotationGet(AECameraActiveGet());
	AELuaPushVec3(L,v);
	return 1;
}

static int AELuaCore_CameraRotationSet(lua_State* L){
	AEVec3 v;
	//We skip the first argument, it's unused for now
	lua_getfield(L,-1,"x");
	v.x=lua_tonumber(L,-1);
	lua_pop(L,1);
		
	lua_getfield(L,-1,"y");
	v.y=lua_tonumber(L,-1);
	lua_pop(L,1);
	
	lua_getfield(L,-1,"z");
	v.z=lua_tonumber(L,-1);
	lua_pop(L,1);
	
	AECameraRotationSet(AECameraActiveGet(),v);
	
	return 0;
}

static int AELuaCore_CameraBoundsSet(lua_State* L){
	//We skip the first argument, it's unused for now
	float near=lua_tonumber(L,-3);
	float far=lua_tonumber(L,-2);
	float fov=lua_tonumber(L,-1);
	
	AECameraBoundsSet(AECameraActiveGet(),near,far,fov);
	
	return 0;
}

static int AELuaCore_CameraViewportSet(lua_State* L){
	//We skip the first argument, it's unused for now
	int w=lua_tonumber(L,-2);
	int h=lua_tonumber(L,-1);
	
	AECameraViewportSet(AECameraActiveGet(),w,h);
	
	return 0;
}

static int AELuaCore_CameraVFCheckSphere(lua_State* L){
	//We skip the first argument, it's unused for now
	float x=lua_tonumber(L,-4);
	float y=lua_tonumber(L,-3);
	float z=lua_tonumber(L,-2);
	float r=lua_tonumber(L,-1);
	
	int result=AECameraVFCheckSphere(AECameraActiveGet(),x,y,z,r);
	
	lua_pushboolean(L,result);
	
	return 1;
}

static const struct luaL_reg AELuaCoreLibCamera [] = {
	{"AECameraPositionGet",AELuaCore_CameraPositionGet},
	{"AECameraPositionSet",AELuaCore_CameraPositionSet},
	{"AECameraRotationGet",AELuaCore_CameraRotationGet},
	{"AECameraRotationSet",AELuaCore_CameraRotationSet},
	{"AECameraBoundsSet",AELuaCore_CameraBoundsSet},
	{"AECameraViewportSet",AELuaCore_CameraViewportSet},
	{"AECameraVFCheckSphere",AELuaCore_CameraVFCheckSphere},
	{NULL, NULL}
};

///////////////////////////////////////////////////////////////////////

void AELuaCoreSetup(lua_State* L){
	AELuaAddLibrary(L,"",AELuaCoreLibCore);
	AELuaAddLibrary(L,"",AELuaCoreLibCamera);
	lua_pushlightuserdata(L,NULL);
	lua_setglobal(L,"AENULL");
	luaopen_luagl(L);
	luaopen_luaglu(L);
}

///////////*/