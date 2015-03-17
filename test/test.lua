
local user32 = lcall.load('user32');
local MessageBoxA = lcall.get(user32, 'MessageBoxA');
local ret = lcall.call(MessageBoxA, 0, 'text', 'cap', 0); -- yes, no or cancel
print(ret); -- return value are lightuserdata

local msvcrt = lcall.load('msvcrt');
local printf = lcall.get(msvcrt, 'printf');
ret = lcall.call(printf, 'hello world!\n');
print(ret);

ret = lcall.call(printf, 'integer: %d, pointer: %p\n', 10, printf);
print(ret);
