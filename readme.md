# lcall

A very crude library to link dll dynamically and call some simple funcitons.

When porting , these two functions should be implemented:

* 'lib_load' returns the handler of a shared object('dlopen', 'LoadLibrary'). 
* 'lib_get' returns the address of the system call('dlsym', 'GetProcAddress'). 

macros should implement

* 'arg_set' passes arguments in a certain way('push' onto stack or 'mov' to registers, it depends on call protocol). 
* 'proc_call' invokes system call using the address('call address' or 'int n'). 'ret_get' stores return values.
* 'ret_get' store return value of a system call.

Many things to be considered.

* byte order
* machine word size
* the support of embeded assemble language.
* the alignment of arguemnt
* the calling convention
* the system api
and so on

-----

limit for now:
* only support LUA_TNIL(as NULL), LUA_TNUMBER(as int), LUA_TSTRING(as pointer), LUA_TLIGHTUSERDATA(as pointer), LUA_TBOOLEAN(as integer, 1 or 0).
* only support __stdcall.
* only support 32-bit archtecture.
* only support Windows for now.
* not tested