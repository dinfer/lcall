#ifndef win32_h
#define win32_h

#include "lcall.h"

// set argument to a function, according byte order and call protocol
#define arg_set(_nr_total, _current, _mword)\
	((void)_nr_total),((void)_current);\
	printf("set_arg: %d.%d:%p\n", _nr_total, _current, _mword);\
	__asm{push _mword}

// get return value. not implemented
#define ret_get(_mword_ret)\
	__asm{mov _mword_ret, eax}

// call function
#define proc_call(_mword)\
	__asm{call _mword}

// save stack status
#define stack_save(_mword)\
	__asm{mov _mword, esp}

// restore stac status
#define stack_rewind(_mword)\
	__asm{mov esp, _mword}

#endif
