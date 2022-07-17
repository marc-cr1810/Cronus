#include "cronusrun.h"

#include <core/errorcode.h>
#include <core/lifecycle.h>
#include <objects/stringobject.h>
#include <parser/generator.h>

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

	CrCore_InitTypes();

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

int Cr_FileIsInteractive(std::ifstream* fp, CrObject* filename)
{
	if (CrString_Check(filename))
	{
		return (filename == NULL) ||
			(strcmp(((CrStringObject*)filename)->ob_svar, "<stdin>")) ||
			(strcmp(((CrStringObject*)filename)->ob_svar, "???"));
	}
	CrError_SetString(CrExc_SystemError, "Bad internal call");
	return -1;
}

int CrRun_FileObject(std::ifstream* fp, const char* filename, bool close)
{
	CrObject* filename_obj;
	if (filename != NULL)
	{
		filename_obj = CrStringObject_FromString(filename);
		if (filename_obj == NULL)
		{
			CrError_Print();
			return -1;
		}
	}
	else
		filename_obj = NULL;

	int result = CrRun_FileObject(fp, filename_obj, close);
	CrObject_XDECREF(filename_obj);
	return result;
}

int CrRun_FileObject(std::ifstream* fp, CrObject* filename, bool close)
{
	bool decref_filename = false;
	if (filename == NULL)
	{
		filename = CrStringObject_FromString("???");
		if (filename == NULL)
		{
			CrError_SetString(CrExc_SystemError, "Filename is NULL");
			CrError_Print();
			return -1;
		}
		decref_filename = true;
	}

	int result = 0;
	if (Cr_FileIsInteractive(fp, filename))
	{
		result = CrRun_InteractiveLoop(fp, filename);
		if (close)
			fp->close();
	}
	else
	{
		result = CrRun_SimpleFileObject(fp, filename, close);
	}

	if (decref_filename)
		CrObject_DECREF(filename);
	return result;
}

int CrRun_InteractiveObject(std::ifstream* fp, CrObject* filename)
{
	const char* ps1 = NULL, * ps2 = NULL;
	int error_code = 0;
	mod_type mod;

	if (Cr_FileIsInteractive(fp, filename))
	{
		ps1 = ">>> ";
		ps2 = "... ";
	}

	mod = CrGen_ASTFromFileObject(fp, filename, PARSER_MODE_SINGLE_INPUT, ps1, ps2, &error_code);
	if (mod == NULL)
	{
		if (error_code == E_EOF)
		{
			CrError_Clear();
			return E_EOF;
		}
		return -1;
	}

	return 0;
}

int CrRun_InteractiveLoop(std::ifstream* fp, CrObject* filename)
{
	int error = 0;
	int ret = 0;
	do
	{
		ret = CrRun_InteractiveObject(fp, filename);
		if (ret == -1 && CrError_Occurred())
		{
			CrError_Print();
		}
	} while (ret != E_EOF);
	return error;
}

int CrRun_SimpleFileObject(std::ifstream* fp, CrObject* filename, bool close)
{
	// Currently not supported
	return -1;
}
