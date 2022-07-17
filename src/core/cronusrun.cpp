#include "cronusrun.h"

static CrStatus CrCore_CreateInterpreter(CrRuntimeState* runtime, CrThreadState** tstate_p)
{
	CrInterpreterState* interp = CrInterpreter_New();
	if (interp == NULL)
		return CrStatus_Error("failed to initialize interpreter");

	CrThreadState* tstate = CrThreadState_New(interp);
	if (tstate == NULL)
		return CrStatus_Error("can't make first thread");

	*tstate_p = tstate;
	return CrStatus_Ok();
}

static CrStatus CrCore_Init(CrRuntimeState* runtime, CrThreadState** tstate_p)
{
	CrStatus status;
	CrThreadState *tstate;
	status = CrCore_CreateInterpreter(runtime, &tstate);
	if (CrStatus_Exception(status))
		return status;
	*tstate_p = tstate;

	runtime->core_initialized = 1;
	return status;
}

CrStatus Cr_Initialize()
{
	CrStatus status = CrRuntime_Initialize();
	if (CrStatus_Exception(status))
		return status;

	CrRuntimeState* runtime = &CrRuntime;

	CrThreadState* tstate = NULL;
	if (!runtime->core_initialized)
		status = CrCore_Init(runtime, &tstate);
	if (CrStatus_Exception(status))
		return status;

	return CrStatus_Ok();
}

void Cr_Exit(int status)
{
	CrRuntime_Finalize();
	exit(status);
}

CrStatus CrRuntime_Initialize()
{
    if (CrRuntime.initialized)
        return CrStatus_Ok();

    return CrRuntimeState_Init(CrRuntime);
}

int CrRuntime_Finalize()
{
	int status = 0;

	if (!CrRuntime.initialized)
		return status;

	CrRuntimeState_Finalize(CrRuntime);
	return status;
}
