#pragma once

#include <port.h>
#include <core/thread.h>

#include <objects/object.h>

struct _runtimestate; // Defined in "runtime.h"

typedef struct _interpreterstate
{
	_runtimestate* runtime;
	CrThreadState* thread;
} CrInterpreterState;

CrInterpreterState* CrInterpreter_New();