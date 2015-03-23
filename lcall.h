
#ifndef lcall_h
#define lcall_h

#include "../../../Lua-5.3.0/src/lua.h"
#include "../../../Lua-5.3.0/src/lauxlib.h"

typedef void* hlibrary;
typedef void* hprocedure;
typedef void* hreturn;

extern char* c_stdcall;
extern char* c_cdecl;

LUALIB_API void lcall_open(lua_State* L);

#endif
