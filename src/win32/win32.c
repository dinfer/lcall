
#include "win32.h"
#include <Windows.h>

hlibrary lib_load(const char* libname){
	return (hlibrary)LoadLibraryA(libname);
}

hprocedure lib_get(hlibrary h, const char* procname){
	return (hprocedure)GetProcAddress((HMODULE)h, procname);
}
