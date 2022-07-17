#pragma once

#include <port.h>
#include <objects/object.h>

struct _interpreterstate; // Defined in "interpreter.h"

typedef struct _threadstate
{
	_interpreterstate* interp; // The interpreter the thread is owned by

	/* The exception currently being raised */
	CrObject* curr_exc_type;
	CrObject* curr_exc_value;
} CrThreadState;

CrThreadState* CrThreadState_New(_interpreterstate* interp);