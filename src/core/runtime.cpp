#include "runtime.h"

CrRuntimeState CrRuntime = { 0 };

CrStatus CrRuntimeState_Init(CrRuntimeState& runtime)
{
	// Will be used further more in future
	// For now it does basically nothing

	runtime.initialized = 1;
	return CrStatus_Ok();
}

void CrRuntimeState_Finalize(CrRuntimeState& runtime)
{
	if (!runtime.initialized)
		return;

	Mem_Free(runtime.main->thread);
	Mem_Free(runtime.main);

	runtime.initialized = 0;
}

CrThreadState* CrRuntimeState_GetThreadState(CrRuntimeState* runtime)
{
	return runtime->main->thread;
}

CrInterpreterState* CrRuntimeState_GetInterpreterState(CrRuntimeState* runtime)
{
	return runtime->main;
}