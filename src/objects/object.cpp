#include "object.h"

#include <core/error.h>

#include <objects/baseobject.h>

CrTypeObject CrNullType = {
	VAROBJECT_HEAD_INIT(&CrBaseType, 0)		// base
	"NullType",								// tp_name
	0,										// tp_doc
	0,										// tp_size
	0,										// tp_itemsize
	TPFLAGS_DEFAULT,						// tp_flags
	0,										// tp_dealloc
	0,										// tp_base
	Mem_Free								// tp_free
};

CrObject CrNullStruct = {
	1, &CrNullType
};

static int type_is_subtype_chain(CrTypeObject* a, CrTypeObject* b)
{
	do {
		if (a == b)
			return 1;
		a = a->tp_base;
	} while (a != NULL);
	return (b == &CrBaseObjectType);
}

int CrType_IsSubtype(CrTypeObject* a, CrTypeObject* b)
{
	return type_is_subtype_chain(a, b);
}

CrObject* Object_New(CrTypeObject* type)
{
	CrObject* obj = (CrObject*)Mem_Alloc(type->tp_size);
	ObjectInit(obj, type);
	return obj;
}

void ObjectNewRef(CrObject* obj)
{
	obj->ob_refcount = 1;
}

CrObject* Object_NewRef(CrObject* obj)
{
	CrObject_INCREF(obj);
	return obj;
}

CrObject* Object_XNewRef(CrObject* obj)
{
	CrObject_XINCREF(obj);
	return obj;
}

int CrType_Ready(CrTypeObject* type)
{
	CrTypeObject* base;

	if (type->tp_flags & TPFLAGS_READY)
		return 0;

	assert((type->tp_flags & TPFLAGS_READYING) == 0);

	type->tp_flags |= TPFLAGS_READYING;

	if (type->tp_name == NULL)
	{
		CrError_Format(CrExc_SystemError, "type does not define the tp_name field");
		goto error;
	}

	// Initialize the tp_base (defaults to BaseObject unless thats's us)
	base = type->tp_base;
	if (base == NULL && type != &CrBaseObjectType)
	{
		base = &CrBaseObjectType;
		if (type->tp_flags & TPFLAGS_HEAPTYPE)
		{
			type->tp_base = (CrTypeObject*)CrObject_NEWREF((CrObject*)base);
		}
		else
		{
			type->tp_base = base;
		}
	}

	// Initlialize the base class
	if (base != NULL)
	{
		if (CrType_Ready(base) < 0)
			goto error;
	}

	// Initialize ob_type if NULL
	if (Cr_IS_TYPE(type, NULL) && base != NULL)
		CrObject_SET_TYPE(type, Cr_TYPE(base));
error:
	type->tp_flags &= ~TPFLAGS_READYING;
	return 0;
}
