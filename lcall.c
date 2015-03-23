
#include "lcall.h"

#include <stdio.h>
#include <string.h>

#include "lcallapi.h"

// load a library and return the handler
static int lcall_load (lua_State *L){
	hlibrary hlib;
	const char * lib_name = luaL_checkstring(L, 1);
	printf("library name: %s\n", lib_name);
	hlib = lib_load(lib_name);
	printf("hlibrary @: %p\n", hlib);
	lua_pushlightuserdata(L, hlib);
	return 1;
}

// get a function pointer
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

char* c_stdcall = "__stdcall";

// do call by using `__stdcall` call convention.
static int _call_stdcall(lua_State *L){
	int i, n;
	hreturn hret;
	long l;
	const void *pproc;
	const char *pstring; // for lua lua_String
	int num; // for lua_Number, lua_Bool
	const void* p; // for lightuserdata
	n = lua_gettop(L);
	stack_save(l);
	for(i = n; i >= 2; i--){
		switch(lua_type(L, i)){
		case LUA_TNIL:
			arg_set(n, i, 0);
			break;
		case LUA_TNUMBER:
				num = (int)luaL_checknumber(L, i);
				arg_set(n, i, num);
			break;
		case LUA_TSTRING:
				pstring = luaL_checkstring(L, i);
				arg_set(n, i, pstring);
			break;
		case LUA_TLIGHTUSERDATA:
				p = lua_topointer(L, i);
				arg_set(n, i, p);
			break;
		case LUA_TBOOLEAN:
				num = lua_toboolean(L, i);
				arg_set(n, i, num);
			break;
		}
	}
	pproc = lua_topointer(L, 1);
	proc_call(pproc);
	ret_get(hret);
	stack_rewind(l);
	lua_pushlightuserdata(L, hret);
	return 1;
}

char* c_cdecl = "__cdecl";

// do call by using `__cdecl` call convention.
static int _call_cdecl(lua_State *L){
	// because __stdcall store and recover stack, so __cdecl equals __stdcall
	return _call_stdcall(L);
}

// dispatch to `call_*`
int _dispatch_call(lua_State *L, const char *proto){
	if(strcmp("__stdcall", proto) == 0){
		return _call_stdcall(L);
	}else if(strcmp("__cdecl", proto) == 0){
		return _call_cdecl(L);
	}
	return 0;
}

// check arguments and set arguments for `call_*`
static int lcall_call(lua_State *L){
	int n, i;
	const char * proto;

	// check proto
	proto = luaL_checkstring(L, 1);
	if(proto == NULL){
		luaL_error(L, ("need call convention like \"__stdcall\", \"__cdecl \" and so on.\n"));
		return 0;
	}

	// check function pointer
	if(!lua_islightuserdata(L, 2)){
		luaL_error(L, "invalid hprocedure");
		return 0;
	}

	// has unsupported type
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

	lua_remove(L, 1);

	return _dispatch_call(L, proto);

	return 0;
}

static int lcall_calls(lua_State *L){
	const char *proto, *lib_name, *proc_name;
	hlibrary hlib;
	hprocedure hproc;
	proto = luaL_checkstring(L, 1);
	lib_name = luaL_checkstring(L, 2);
	proc_name = luaL_checkstring(L, 3);
	hlib = lib_load(lib_name);
	hproc = lib_get(hlib, proc_name);
	lua_remove(L, 1);
	lua_remove(L, 1);
	lua_remove(L, 1);
	lua_pushlightuserdata(L, hproc);
	lua_insert(L, 1);
	return _dispatch_call(L, proto);
}

//////////////////////////////////////////////////////////////////////////

static const luaL_Reg mathlib[] ={
	{"load", lcall_load},
	{"get", lcall_get},
	{"call", lcall_call},
	{"calls", lcall_calls},

	{NULL, NULL},
};

LUAMOD_API int luaopen_lcall (lua_State *L){
	luaL_newlib(L, mathlib);
	return 1;
}

static const luaL_Reg lcalllibs[] = {
	{"lcall", luaopen_lcall},
	{NULL, NULL}
};

LUALIB_API void lcall_open(lua_State* L){
	const luaL_Reg *lib;
	/* "require" functions from 'lcalllibs' and set results to global table */
	for (lib = lcalllibs; lib->func; lib++) {
		luaL_requiref(L, lib->name, lib->func, 1);
		lua_pop(L, 1);  /* remove lib */
	}
}
