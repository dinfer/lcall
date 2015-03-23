# lcall

A very crude library to enable lua call functions in dynamic link library like this:

```lua
lua.calls('__stdcall', 'user32', 'MessageBoxA', 0, 'Message Box Message', 'Caption', 0);
```

In order to use `lcall`, please include "lcall.h" and call `lcall_open` just like `lua_openlibs`.

Please refer the `test/*_example.lua` for more details , 

This library supports windows x86 for now. I'm coding it to make it run on linux.

-----

## Api Documentation

### lcall.load

```lua
lcall.load('library_name')
```

Returns the handler of a dynamic link library.

### lcall.get

```lua
lcall.get('function_name')
```

Return the address of a function in dynamic library.

### lcall.call

```lua
lcall.call('call_convention', address_of_function, arguments)
```

Calls the function `address_of_function` returned by `lcall.get`. The `call_convention` should be `__stdcall` or `__cdecl`. More call conventions will be supported in the future. `arguments` are passed to the target function, You can set them to meet your need.

Only `LUA_TNIL`, `LUA_TNUMBER`, `LUA_TBOOLEAN`, `LUA_TSTRING`, `LUA_TLIGHTUSERDATA` are supported. `LUA_TTABLE`, `LUA_TFUNCTION`, `LUA_TUSERDATA`, and `LUA_TTHREAD` are not support for now. the supported types are treated as follow:

* `LUA_TNIL`: int 0
* `LUA_TNUMBER`: int ?
* `LUA_TBOOLEAN`: int 1 or int 0
* `LUA_TSTRING`: void *
* `LUA_TLIGHTUSERDATA`: void *

The return value while be stored as a `lightuserdata`.

### lcall.calls

```lua
lcall.calls('call_convention', 'library_name', 'function_name', arguments);
```

An easy way to call a function in a dynamic link library. Using this function is equal to use code below:

```lua
lcall.call('call_convention', lcall.get(lcall.load('library_name'), 'function_name'), arguments);
```

-----

# Others

The aim of this project is to provide a way to call any function provided by operating system directly in Lua. Just like what the [FFI](http://luajit.org/ext_ffi.html) does.

But just like what official website said: **_The lib _**

There is a long way to go. A lot of things should be considered like:

* how to mapping from Lua table to C struct
* how to detect the call convention from a library, rather than set it in lua.
* how to manage many kinds of allocated resource for futrue reuse.
