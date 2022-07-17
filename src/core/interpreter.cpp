#include "interpreter.h"

#include <core/runtime.h>

CrInterpreterState* CrInterpreter_New()
{
    CrInterpreterState* interp = (CrInterpreterState*)Mem_Alloc(sizeof(CrInterpreterState));
	if (interp == NULL)
		return NULL;

	_runtimestate* runtime = &CrRuntime;
	interp->runtime = runtime;

	runtime->main = interp;

    return interp;
}
