#include "error.h"

#include <core/cronusrun.h>
#include <objects/stringobject.h>

/* Forward declarations */

static void error_set_object(CrThreadState* tstate, CrObject* exception, CrObject* value);

/* Helper functions */

static inline CrObject* error_occured(CrThreadState* tstate)
{
	assert(tstate != NULL);
	return tstate->curr_exc_type;
}

static void error_restore(CrThreadState* tstate, CrObject* type, CrObject* value)
{
	CrObject* oldtype, * oldvalue;

	oldtype = tstate->curr_exc_type;
	oldvalue = tstate->curr_exc_value;

	tstate->curr_exc_type = type;
	tstate->curr_exc_value = value;

	CrObject_XDECREF(oldtype);
	CrObject_XDECREF(oldvalue);
}

static void error_clear(CrThreadState* tstate)
{
	error_restore(tstate, NULL, NULL);
}

static CrObject* error_format_string(CrThreadState* tstate, CrObject* exception, const char* string)
{
	if (CrException_Check(exception))
		return NULL;

	CrObject* result = NULL;
	CrObject* str1;
	CrObject* str2;

	str1 = ((CrExceptionObject*)exception)->type;
	if (str1 == NULL)
		return NULL;
	str2 = CrStringObject_FromString(string);
	if (str2 == NULL)
		return NULL;

	CrObject* sep_str = CrStringObject_FromString(": ");

	result = (CrObject*)CrString_Concat(CrString_Concat(str1, sep_str), str2);
	CrObject_XDECREF(str1);
	CrObject_XDECREF(str2);
	return result;
}

static CrObject* error_format(CrThreadState* tstate, CrObject* exception, const char* string)
{
	error_clear(tstate);
	CrObject* str = error_format_string(tstate, exception, string);
	error_set_object(tstate, exception, str);
	CrObject_XDECREF(str);
	return NULL;
}

static void error_set_object(CrThreadState* tstate, CrObject* exception, CrObject* value)
{
	if (exception == NULL || CrException_Check(exception))
	{
		error_format(tstate, CrExc_SystemError, "exception is not an Exception object");
		return;
	}

	CrObject_XINCREF(value);
	error_restore(tstate, exception, value);
}

static void error_print_exception(CrThreadState* tstate)
{
	if (error_occured(tstate))
	{
		CrStringObject* msg = (CrStringObject*)tstate->curr_exc_value;
		printf(msg->ob_svar);

		Cr_Exit(((CrExceptionObject*)tstate->curr_exc_type)->exitcode);
	}
}

/* API Function definitions */

void CrError_SetNone(CrObject* exception)
{
	CrThreadState* tstate = CrThreadState_Get();
	error_set_object(tstate, exception, (CrObject*)NULL);
}

void CrError_SetObject(CrObject* exception, CrObject* value)
{
	CrThreadState* tstate = CrThreadState_Get();
	error_set_object(tstate, exception, value);
}

void CrError_SetString(CrObject* exception, const char* string)
{
	CrThreadState* tstate = CrThreadState_Get();
	error_format(tstate, exception, string);
}

CrObject* CrError_Occurred()
{
	CrThreadState* tstate = CrThreadState_Get();
	return error_occured(tstate);
}

void CrError_Clear()
{
	CrThreadState* tstate = CrThreadState_Get();
	error_clear(tstate);
}

void CrError_Restore(CrObject* type, CrObject* value)
{
	CrThreadState* tstate = CrThreadState_Get();
	error_restore(tstate, type, value);
}

void CrError_Format(CrObject* exception, const char* string)
{
	CrThreadState* tstate = CrThreadState_Get();
	error_format(tstate, exception, string);
}

void CrError_Print()
{
	CrThreadState* tstate = CrThreadState_Get();
	error_print_exception(tstate);
}

void CrError_NoMemory()
{
	CrThreadState* tstate = CrThreadState_Get();
	if (CrExc_MemoryError == NULL)
	{
		std::cout << "out of memory" << std::endl;
		return;
	}
	CrObject* msg = CrStringObject_FromString("out of memory");
	error_set_object(tstate, CrExc_MemoryError, msg);
}

void CrError_BadInternalCall()
{
	CrThreadState* tstate = CrThreadState_Get();
	CrObject* msg = CrStringObject_FromString("bad argument to internal function");
	error_set_object(tstate, CrExc_SystemError, msg);
}

CrObject* CrExceptionObject_New(const char* type, int exitcode)
{
	CrExceptionObject* exc = CrObject_NEW(CrExceptionObject, &CrExceptionType);
	if (exc == NULL)
		return NULL;

	exc->type = CrStringObject_FromString(type);
	exc->context = NULL;
	exc->exitcode = exitcode;

	return (CrObject*)exc;
}

CrTypeObject CrExceptionType = {
	VAROBJECT_HEAD_INIT(&CrExceptionType, 0)	// base
	"exception",								// tp_name
	"Exception object type",				    // tp_doc
	sizeof(CrExceptionObject),					// tp_size
	0,										    // tp_itemsize
	TPFLAGS_DEFAULT | TPFLAGS_BASETYPE |	    // tp_flags
		TPFLAGS_BASE_EXC_SUBCLASS,
	0,										    // tp_dealloc
	0,											// tp_base
	Mem_Free								    // tp_free
};

CrObject* CrExc_Exception = CrExceptionObject_New("Exception", 1);
CrObject* CrExc_TypeError = CrExceptionObject_New("TypeError", 2);
CrObject* CrExc_IndexError = CrExceptionObject_New("IndexError", 3);
CrObject* CrExc_ValueError = CrExceptionObject_New("ValueError", 4);

CrObject* CrExc_SyntaxError = CrExceptionObject_New("SyntaxError", 5);
CrObject* CrExc_IndentationError = CrExceptionObject_New("IndentationError", 6);
CrObject* CrExc_TabError = CrExceptionObject_New("TabError", 7);

CrObject* CrExc_KeyboardInterrupt = CrExceptionObject_New("KeyboardInterrupt", 8);
CrObject* CrExc_MemoryError = CrExceptionObject_New("MemoryError", 9);
CrObject* CrExc_SystemError = CrExceptionObject_New("SystemError", 10);
CrObject* CrExc_RuntimeError = CrExceptionObject_New("RuntimeError", 11);