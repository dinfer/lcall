#ifndef lcallapi_h
#define lcallapi_h


// common function declaration

// load dynamically link library
hlibrary lib_load(const char* libname);

// load a function of a loaded library
hprocedure lib_get(hlibrary h, const char* proc_name);

// include macros that embed asmble language

#if (defined LCALL_WINDOWS) && (defined LCALL_X86)
#include "arch/windows_x86.h"
#endif

#if (defined LCALL_LINUX) && (defined LCALL_X86)
#include "linux_x86.h"
#endif

#endif