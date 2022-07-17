#include "stringobject.h"

#include <core/error.h>

/*
	Methods
*/

static CrObject* string_concat(CrStringObject* a, CrObject* b)
{
	CrStringObject* result = NULL;
	if (!CrString_CheckExact(b))
	{
		CrError_SetString(CrExc_TypeError, "can only concat strings");
		return NULL;
	}

	if (Cr_SIZE(a) == 0)
	{
		result = (CrStringObject*)b;
		CrObject_INCREF(result);
		return (CrObject*)result;
	}
	if (Cr_SIZE(b) == 0)
	{
		result = a;
		CrObject_INCREF(result);
		return (CrObject*)result;
	}

	result = (CrStringObject*)CrStringObject_FromStringAndSize(NULL, Cr_SIZE(a) + Cr_SIZE(b));
	if (result != NULL)
	{
		memcpy(result->ob_svar, a->ob_svar, Cr_SIZE(a));
		memcpy(result->ob_svar + Cr_SIZE(a), ((CrStringObject*)b)->ob_svar, Cr_SIZE(b));
	}
	return (CrObject*)result;
}

static void string_dealloc(CrStringObject* self)
{
	if (self->ob_svar != 0)
		Mem_Free(self->ob_svar);
	Cr_TYPE(self)->tp_free((CrObject*)self);
}

CrTypeObject CrStringType = {
	VAROBJECT_HEAD_INIT(&CrStringType, 0)	// base
	"string",								// tp_name
	"String object type",					// tp_doc
	sizeof(CrStringObject),					// tp_size
	0,										// tp_itemsize
	TPFLAGS_DEFAULT | TPFLAGS_BASETYPE,		// tp_flags
	(destructor)string_dealloc,				// tp_dealloc
	0,										// tp_base
	Mem_Free								// tp_free
};

CrObject* CrStringObject_FromString(const char* bytes)
{
	return CrStringObject_FromStringAndSize(bytes, strlen(bytes));
}

CrObject* CrStringObject_FromStringAndSize(const char* bytes, Cr_size_t size)
{
	CrStringObject* obj;
	size_t alloc;

	if (size < 0)
	{
		CrError_SetString(CrExc_SystemError, "Negative size passed to CrStringObject_FromStringAndSize");
		return NULL;
	}

	obj = CrObject_NEW(CrStringObject, &CrStringType);
	if (obj == NULL)
		return NULL;
	if (size == 0)
	{
		obj->ob_svar = NULL;
		alloc = 0;
	}
	else
	{
		alloc = size + 1;
		obj->ob_svar = (char*)Mem_Alloc(alloc);
		if (bytes != NULL && size > 0)
			memcpy(obj->ob_svar, bytes, size);
		obj->ob_svar[size] = '\0'; // Trailing NULL byte (end of string)
	}
	VAROBJECT_SET_SIZE(obj, size);
	obj->ob_alloc = alloc;
	return (CrObject*)obj;
}

CrObject* CrString_Concat(CrObject* a, CrObject* b)
{
	if (!CrString_CheckExact(a))
	{
		CrError_SetString(CrExc_SystemError, "Tried to pass non-CrStringObject in CrString_Concat");
		return NULL;
	}

	return string_concat((CrStringObject*)a, b);
}

CrStatus CrString_InitTypes()
{
	if (CrType_Ready(&CrStringType) < 0)
		return CrStatus_Error("can't init string type");

	return CrStatus_Ok();
}
