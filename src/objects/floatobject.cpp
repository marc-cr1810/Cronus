#include "floatobject.h"

/*
	Methods
*/

static void float_dealloc(CrFloatObject* self)
{
	Cr_TYPE(self)->tp_free((CrObject*)self);
}

CrTypeObject CrFloatType = {
	VAROBJECT_HEAD_INIT(&CrFloatType, 0)	// base
	"float",								// tp_name
	"Float object type",					// tp_doc
	sizeof(CrFloatObject),					// tp_size
	0,										// tp_itemsize
	TPFLAGS_DEFAULT | TPFLAGS_BASETYPE,		// tp_flags
	(destructor)float_dealloc,				// tp_dealloc
	0,										// tp_base
	Mem_Free								// tp_free
};

CrObject* CrFloatObject_FromDouble(double dval)
{
	CrFloatObject* obj = CrObject_NEW(CrFloatObject, &CrFloatType);
	obj->ob_fval = dval;
	return (CrObject*)obj;
}

CrStatus CrFloat_InitTypes()
{
	if (CrType_Ready(&CrFloatType) < 0)
		return CrStatus_Error("can't init float type");

	return CrStatus_Ok();
}
