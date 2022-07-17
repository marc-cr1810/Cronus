#pragma once

#include "object.h"

typedef struct _stringobject
{
	CrObject_VAR_HEAD;
	size_t ob_alloc;
	char* ob_svar;
} CrStringObject;

/* Type object */
extern CrTypeObject CrStringType;

/* Type check macros */
#define CrString_Check(self) CrObject_TypeCheck(self, &CrStringType)
#define CrString_CheckExact(self) Cr_IS_TYPE(self, &CrStringType)

/* Convert an array of bytes to a CrStringObject */
CrObject* CrStringObject_FromString(const char* bytes);
CrObject* CrStringObject_FromStringAndSize(const char* bytes, Cr_size_t size);

/* API Functions */
CrObject* CrString_Concat(CrObject* a, CrObject* b);