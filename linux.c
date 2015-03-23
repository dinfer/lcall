#include "linux.h"

// load dynamically link library
hlibrary lib_load(const char* libname){
	return (hlibrary)dlopen(libname, RTLD_NOW | RTLD_LOCAL);
}

// load a function of a loaded library
hprocedure lib_get(hlibrary h, const char* proc_name){
	return (hprocedure)dlsym(h, proc_name)
}