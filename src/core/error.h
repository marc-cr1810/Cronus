#pragma once

#include <objects/object.h>

/* Error handling definitions */

void CrError_SetNone(CrObject* exception);
void CrError_SetObject(CrObject* exception, CrObject* value);
void CrError_SetString(CrObject* exception, const char* string);
CrObject* CrError_Occurred();
void CrError_Clear();
void CrError_Restore(CrObject* type, CrObject* value);
void CrError_Format(CrObject* exception, const char* string);

/* Convenience functions */

void CrError_Print();

void CrError_NoMemory();
void CrError_BadInternalCall();

/* Error objects */

/* Exception Type object */
extern CrTypeObject CrExceptionType;

/* EXCEPTION_HEAD defines the initial segment of every exception class. */
#define EXCEPTION_HEAD CrObject_HEAD CrObject* type; CrObject* context; int exitcode;

typedef struct _viexceptionobject
{
	EXCEPTION_HEAD
} CrExceptionObject;

CrObject* CrExceptionObject_New(const char* type, int exitcode);

#define CrException_Check(self) CrType_HasFeature((CrTypeObject*)self, TPFLAGS_BASE_EXC_SUBCLASS)

/* Predefined exceptions */
extern CrObject* CrExc_Exception;
extern CrObject* CrExc_TypeError;
extern CrObject* CrExc_IndexError;
extern CrObject* CrExc_ValueError;

extern CrObject* CrExc_SyntaxError;
extern CrObject* CrExc_IndentationError;
extern CrObject* CrExc_TabError;

extern CrObject* CrExc_KeyboardInterrupt;
extern CrObject* CrExc_MemoryError;
extern CrObject* CrExc_SystemError;
extern CrObject* CrExc_RuntimeError;