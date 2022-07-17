#pragma once

#include <port.h>
#include <core/status.h>
#include <core/interpreter.h>

typedef struct _runtimestate
{
	int initialized;		// Is Cronus initialized?
	int core_initialized;	// Is the runtime core initialized?

	// Will probably involve more interpreters in the future
	CrInterpreterState* main;
} CrRuntimeState;

extern CrRuntimeState CrRuntime;

CrStatus CrRuntimeState_Init(CrRuntimeState& runtime);
void CrRuntimeState_Finalize(CrRuntimeState& runtime);

CrThreadState* CrRuntimeState_GetThreadState(CrRuntimeState* runtime);
inline CrThreadState* CrThreadState_Get()
{
	return CrRuntimeState_GetThreadState(&CrRuntime);
}

CrInterpreterState* CrRuntimeState_GetInterpreterState(CrRuntimeState* runtime);
inline CrInterpreterState* CrInterpreterState_Get()
{
	return CrRuntimeState_GetInterpreterState(&CrRuntime);
}