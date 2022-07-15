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


/* Convert an array of bytes to a CrStringObject */
CrObject* CrStringObject_FromString(const char* bytes);
CrObject* CrStringObject_FromStringAndSize(const char* bytes, Cr_size_t size);