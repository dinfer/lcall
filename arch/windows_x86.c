#include "../lcall.h"

#include <Windows.h>

// load library dynamically
hlibrary lib_load(const char* libname){
	return (hlibrary)LoadLibraryA(libname);
}

// get a function address dynamically
hprocedure lib_get(hlibrary h, const char* proc_name){
	return (hprocedure)GetProcAddress((HMODULE)h, proc_name);
}
