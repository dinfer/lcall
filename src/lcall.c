
#include "lcall.h"

#include <stdio.h>

#include "win32/win32.h"

static int lcall_load (lua_State *L){
	size_t len;
	hlibrary hlib;
	const char * lib_name = luaL_checklstring(L, 1, &len);
	printf("library name: %s\n", lib_name);
	hlib = lib_load(lib_name);
	printf("hlibrary @: %p\n", hlib);
	lua_pushlightuserdata(L, hlib);
	return 1;
}

static int lcall_get (lua_State *L){
	hlibrary hlib;
	hprocedure hproc;
	const char * proc_name;
	hlib = (hlibrary)lua_topointer(L, 1);
	if(!lua_islightuserdata(L, 1) || hlib == NULL){
		luaL_error(L, "invalid hlibrary");
		return 0;
	}
	proc_name = luaL_checkstring(L, 2);
	if(proc_name == NULL){
		luaL_error(L, "invalid procedure name");
		return 0;
	}
	hproc = lib_get(hlib, proc_name);
	printf("procedure at: %p\n", hproc);
	lua_pushlightuserdata(L, hproc);
	return 1;
}

static int lcall_call(lua_State *L){
	int n, i;
	const void* pproc;
	hreturn hret;
	if(!lua_islightuserdata(L, 1)){
		luaL_error(L, "invalid hprocedure");
		return 0;
	}
	n = lua_gettop(L);
	for(i = 2; i <= n; i++){
		switch(lua_type(L, i)){
		case LUA_TTABLE:
		case LUA_TFUNCTION:
		case LUA_TUSERDATA:
		case LUA_TTHREAD:
			luaL_error(L, "lcall does not support this kind of type temporaryly");
			return 0;
		}
	}
	for(i = n; i >= 2; i--){
		switch(lua_type(L, i)){
		case LUA_TNIL:
			arg_set(0);
			break;
		case LUA_TNUMBER:
			{
				int num = (int)luaL_checknumber(L, i);
				arg_set(num);
			}
			break;
		case LUA_TSTRING:
			{
				const char* pstring = luaL_checkstring(L, i);
				arg_set(pstring);
			}
			break;
		case LUA_TLIGHTUSERDATA:
			{
				const void * parg = lua_topointer(L, i);
				arg_set(parg);
			}
			break;
		case LUA_TBOOLEAN:
			{
				int ibool = lua_toboolean(L, i);
				arg_set(ibool);
			}
			break;
		}
	}
	pproc = lua_topointer(L, 1);
	proc_call(pproc);
	ret_get(hret);
	lua_pushlightuserdata(L, hret);
	return 1;
}

static int lcall_dump(lua_State *L){
	const char * lib_name, * proc_name;
	hlibrary hlib;
	lib_name = luaL_checkstring(L, 1);
	proc_name = luaL_checkstring(L, 2);
	if(!lib_name || !proc_name)
		return 0;
	hlib = lib_load(lib_name);
	printf("hlibrary @: %p\nhprocedure @: %p\n", hlib, lib_get(hlib, proc_name));
	return 0;
}

//////////////////////////////////////////////////////////////////////////

static const luaL_Reg mathlib[] ={
	{"load", lcall_load},
	{"get", lcall_get},
	{"call", lcall_call},

	// optional
	{"dump", lcall_dump},
	{NULL, NULL},
};

LUAMOD_API int luaopen_lcall (lua_State *L){
	luaL_newlib(L, mathlib);
	return 1;
}

static const luaL_Reg loadedlibs[] = {
	{"lcall", luaopen_lcall},
	{NULL, NULL}
};

LUALIB_API void lcall_open(lua_State* L){
	const luaL_Reg *lib;
	/* "require" functions from 'loadedlibs' and set results to global table */
	for (lib = loadedlibs; lib->func; lib++) {
		luaL_requiref(L, lib->name, lib->func, 1);
		lua_pop(L, 1);  /* remove lib */
	}
}
