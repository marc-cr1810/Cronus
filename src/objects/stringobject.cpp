#include "stringobject.h"

/*
	Methods
*/

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
	(destructor)string_dealloc,				// tp_dealloc
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
		///TODO: ERROR HANDLING
		//CrError_SetString(CrExc_SystemError, "Negative size passed to CrStringObject_FromStringAndSize");
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
