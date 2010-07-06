#include "../AE/Core.h"
#include "../AE/Text.h"
#include "../AE/UI.h"
#include "../AE/LuaBinding.h"
#include "../AE/LuaLibs.h"

int main(int argc,char** argv){
	lua_State* L=AELuaNew(1);
	AELuaCoreSetup(L);
	AELuaRun(L,"Lua.lua");
	//AEStart(NULL);
	return 1;
}
