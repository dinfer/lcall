
#ifndef lcall_h
#define lcall_h

#include "../../../Lua-5.3.0/src/lua.h"
#include "../../../Lua-5.3.0/src/lauxlib.h"

typedef void* hlibrary;
typedef void* hprocedure;
typedef void* hreturn;

LUALIB_API void lcall_open(lua_State* L);

#endif
