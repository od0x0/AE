#include "../LuaBinding.h"
#include "../UI.h"
/////////////////////////////////////////////////////////////////////			AEFont

static int AELuaUI_FontInit(lua_State *L){
	AEFontInit();
	return 0;
}

static int AELuaUI_FontNew(lua_State *L){
	char* path=(char*)lua_tostring(L,-2);
	int size=(int)lua_tointeger(L,-1);
	void* font=AEFontNew(path,size);
	lua_pushlightuserdata(L,font);
	return 1;
}

static int AELuaUI_FontRelease(lua_State *L){
	void* font=lua_touserdata(L,-1);
	AEFontRelease(font);
	return 0;
}

static const struct luaL_reg AELuaUILibFont [] = {
	{"AEFontInit",AELuaUI_FontInit},
	{"AEFontNew",AELuaUI_FontNew},
	{"AEFontRelease",AELuaUI_FontRelease},
	{NULL, NULL}
};

/////////////////////////////////////////////////////////////////////			AEText


static int AELuaUI_Text(lua_State *L){
	char* string=(char*)lua_tostring(L,-4);
	void* font=lua_touserdata(L,-3);
	int x=lua_tointeger(L,-2);
	int y=lua_tointeger(L,-1);
	AEText(string,font,x,y);
	return 0;
}

static int AELuaUI_TextMultiline(lua_State *L){
	char* string=(char*)lua_tostring(L,-4);
	void* font=lua_touserdata(L,-3);
	int x=lua_tointeger(L,-2);
	int y=lua_tointeger(L,-1);
	AETextMultiline(string,font,x,y);
	return 0;
}

static int AELuaUI_TextEnd(lua_State *L){
	char* string=(char*)lua_tostring(L,-4);
	void* font=lua_touserdata(L,-3);
	int x=lua_tointeger(L,-2);
	int y=lua_tointeger(L,-1);
	AEVec2i v=AETextEnd(string,font,x,y);
	//Push the vec2i
	lua_newtable(L);
	lua_pushstring(L,"x"); lua_pushinteger(L,v.x); lua_settable(L,-3);
	lua_pushstring(L,"y"); lua_pushinteger(L,v.y); lua_settable(L,-3);
	return 1;
}

static const struct luaL_reg AELuaUILibText [] = {
	{"AEText", AELuaUI_Text},
	{"AETextMultiline",AELuaUI_TextMultiline},
	{"AETextEnd",AELuaUI_TextEnd},
	{"AETextLineSpaceFactorSet",NULL},
	{NULL, NULL}
};

/////////////////////////////////////////////////////////////////////			AETextBuffer


static int AELuaUI_TextBufferNew(lua_State *L){
	lua_pushlightuserdata(L,AETextBufferNew());
	return 1;
}

static int AELuaUI_TextBufferDelete(lua_State *L){
	void* textbuffer=lua_touserdata(L,-1);
	AETextBufferDelete(textbuffer);
	return 0;
}
//AETextBufferInsert

static int AELuaUI_TextBufferInsert(lua_State *L){
	void* textbuffer=lua_touserdata(L,-3);
	size_t offset=lua_tointeger(L,-2);
	char* text=lua_tostring(L,-1);
	AETextBufferInsert(textbuffer,offset,text);
	return 0;
}

static int AELuaUI_TextBufferBackspace(lua_State *L){
	void* textbuffer=lua_touserdata(L,-3);
	size_t offset=lua_tointeger(L,-2);
	size_t length=lua_tointeger(L,-1);
	AETextBufferBackspace(textbuffer,offset,length);
	return 0;
}

static int AELuaUI_TextBufferText(lua_State *L){
	void* textbuffer=lua_touserdata(L,-1);
	char* text=AETextBufferText(textbuffer);
	lua_pushstring(L,text);
	return 1;
}

static int AELuaUI_TextBufferLength(lua_State *L){
	void* textbuffer=lua_touserdata(L,-1);
	size_t length=AETextBufferLength(textbuffer);
	lua_pushinteger(L,length);
	return 1;
}

static const struct luaL_reg AELuaUILibTextBuffer [] = {
	{"AETextBufferNew",AELuaUI_TextBufferNew},
	{"AETextBufferDelete",AELuaUI_TextBufferDelete},
	{"AETextBufferInsert",AELuaUI_TextBufferInsert},
	{"AETextBufferText",AELuaUI_TextBufferText},
	{"AETextBufferLength",AELuaUI_TextBufferLength},
	{NULL, NULL}
};

//////////////////////////////////////////////////////

static int AELuaUI_ImageNew(lua_State *L){
	int w=lua_tointeger(L,-2);
	int h=lua_tointeger(L,-1);
	AEImage* image=AEImageNew(w,h);
	lua_pushlightuserdata(L,image);
	return 1;
}

static int AELuaUI_ImageFromFile(lua_State *L){
	const char* filename=lua_tostring(L,-1);
	AEImage* image=AEImageFromFile((char*)filename);
	lua_pushlightuserdata(L,image);
	return 1;
}

static int AELuaUI_ImagePixel(lua_State *L){
	int x=lua_tointeger(L,-3);
	int y=lua_tointeger(L,-2);
	AEImage* image=lua_touserdata(L,-1);
	unsigned char* pixel=AEImagePixel(image,x,y);
	lua_pushlightuserdata(L,pixel);
	return 1;
}

static int AELuaUI_ImagePixelComponentSet(lua_State *L){
	float value=lua_tonumber(L,-1);
	int component=lua_tointeger(L,-2);
	unsigned char* pixel=lua_touserdata(L,-3);
	pixel[component]=value*255;
	return 0;
}

static int AELuaUI_ImagePixelComponentGet(lua_State *L){
	int component=lua_tointeger(L,-1);
	unsigned char* pixel=lua_touserdata(L,-2);
	lua_pushnumber(L,pixel[component]/255.0f);
	return 1;
}

static int AELuaUI_ImageBlit(lua_State *L){
	AEImage* to=lua_touserdata(L,-4);
	int x=lua_tointeger(L,-3);
	int y=lua_tointeger(L,-2);
	AEImage* from=lua_touserdata(L,-1);
	AEImageBlit(to,x,y,from);
	return 0;
}

static int AELuaUI_ImageToTexture(lua_State *L){
	AEImage* image=lua_touserdata(L,-1);
	unsigned int texture=AEImageToTexture(image);
	lua_pushinteger(L,texture);
	return 1;
}

static int AELuaUI_ImageDelete(lua_State *L){
	AEImage* image=lua_touserdata(L,-1);
	AEImageDelete(image);
	return 0;
}

static const struct luaL_reg AELuaUILibImage [] = {
	{"AEImageNew",AELuaUI_ImageNew},
	{"AEImageFromFile",AELuaUI_ImageFromFile},
	{"AEImagePixel",AELuaUI_ImagePixel},
	{"AEImagePixelComponentSet",AELuaUI_ImagePixelComponentSet},
	{"AEImagePixelComponentGet",AELuaUI_ImagePixelComponentGet},
	{"AEImageBlit",AELuaUI_ImageBlit},
	{"AEImageToTexture",AELuaUI_ImageToTexture},
	{"AEImageDelete",AELuaUI_ImageDelete},
	{NULL, NULL}
};

//////////////////////////////////////////////////////

static int AELuaUI_UIRenderMode(lua_State *L){
	int on=lua_tointeger(L,-1);
	AEUIRenderMode(on);
	return 0;
}

static const struct luaL_reg AELuaUILibUI [] = {
	{"AEUIRenderMode",AELuaUI_UIRenderMode},
	{NULL, NULL}
};

///////////////////////////////////////////////////////////////////////

void AELuaUISetup(lua_State* L){
	/*lua_pushvalue(L, LUA_GLOBALSINDEX);
    luaL_register(L, NULL, AELuaUILibFont);
	luaL_register(L, NULL, AELuaUILibText);
	luaL_register(L, NULL, AELuaUILibTextBuffer);
	luaL_register(L, NULL, AELuaUILibUI);
	lua_pop(L,1);*/
	AELuaAddLibrary(L,"",AELuaUILibFont);
	AELuaAddLibrary(L,"",AELuaUILibText);
	AELuaAddLibrary(L,"",AELuaUILibTextBuffer);
	AELuaAddLibrary(L,"",AELuaUILibImage);
	AELuaAddLibrary(L,"",AELuaUILibUI);
}

///////////