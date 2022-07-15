#pragma once

#include "objects/object.h"

typedef struct _intobject
{
	CrObject_VAR_HEAD;
	Cr_int32_t ob_ival;
} CrIntObject;

/* Type object */
extern CrTypeObject CrIntType;


/* Convert a C++ int to a CrIntObject */
CrObject* CrIntObject_FromInt(Cr_int32_t ival);
CrObject* CrIntObject_FromString(const char* str, int base);