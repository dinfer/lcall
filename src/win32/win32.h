#ifndef win32_h
#define win32_h

#include "../lcall.h"

// load dynamically link library
hlibrary lib_load(const char* libname);

// load a function of a loaded library
hprocedure lib_get(hlibrary h, const char* procname);

//////////////////////////////////////////////////////////////////////////

// set argument to a function, according byte order and call protocol
#define arg_set(_mword)\
	__asm{push _mword}

// get return value. not implemented
#define ret_get(_mword_ret)\
	__asm{mov _mword_ret, eax}

// call the function
#define proc_call(_proc)\
	__asm{call _proc}

#endif
