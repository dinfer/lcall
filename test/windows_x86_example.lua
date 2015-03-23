user32 = lcall.load('user32');
MessageBoxA = lcall.get(user32, 'MessageBoxA');
ret = lcall.call('__stdcall', MessageBoxA, 0, 'test', 'cap', 3);
print('returned by MessageBoxA: ');
print(ret);

msvcrt = lcall.load('msvcrt');
printf = lcall.get(msvcrt, 'printf');
ret = lcall.call('__cdecl', printf, 'lua_Number is %d\nstring pointer is %s\nbool true is %d\nbool false is %d\nlightuserdata is %p\n', 1.0, 'LUA_STRING', true, false, printf);
print('returned by printf: ');
print(ret);

ret = lcall.calls('__stdcall', 'user32', 'MessageBoxA', 0, 'test', 'cap', 3);
print('returned by MessageBoxA: ');
print(ret);

lcall.calls('__stdcall', 'user32', 'MessageBeep', 0);
print('returned by MessageBeep: ');
print(ret);
