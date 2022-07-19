#include "boolobject.h"

#include <objects/intobject.h>

static CrObject* true_str = NULL;
static CrObject* false_str = NULL;

static void bool_dealloc(CrObject* self)
{
	Cr_TYPE(self)->tp_free(self);
}

/* The type object for bool.  Note that this cannot be subclassed! */

CrTypeObject CrBoolType = {
	VAROBJECT_HEAD_INIT(&CrBoolType, 0)		// base
	"bool",									// tp_name
	"Bool object type",						// tp_doc
	sizeof(CrObject),						// tp_size
	0,										// tp_itemsize
	TPFLAGS_DEFAULT | TPFLAGS_BASETYPE,		// tp_flags
	(destructor)bool_dealloc,				// tp_dealloc
	0,										// tp_base
	Mem_Free								// tp_free
};

/* The objects representing bool values False and True */

CrIntObject CrFalseStruct = {
	VAROBJECT_HEAD_INIT(&CrBoolType, 0)
	{ 0 }
};

CrIntObject CrTrueStruct = {
	VAROBJECT_HEAD_INIT(&CrBoolType, 0)
	{ 1 }
};

CrObject* CrBool_FromLong(int b)
{
	CrObject* result;

	if (b)
		result = Cr_True;
	else
		result = Cr_False;
	CrObject_INCREF(result);
	return result;
}

CrStatus CrBool_InitTypes()
{
	if (CrType_Ready(&CrBoolType) < 0)
		return CrStatus_Error("can't init bool type");

	return CrStatus_Ok();
}
