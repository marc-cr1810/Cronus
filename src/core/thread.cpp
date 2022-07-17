#include "thread.h"

#include <core/mem.h>
#include <core/interpreter.h>

CrThreadState* CrThreadState_New(_interpreterstate* interp)
{
    CrThreadState* tstate = (CrThreadState*)Mem_Alloc(sizeof(CrThreadState));
    if (tstate == NULL)
        return NULL;

    tstate->interp = interp;
    interp->thread = tstate;

    tstate->curr_exc_type = NULL;
    tstate->curr_exc_value = NULL;

    return tstate;
}
