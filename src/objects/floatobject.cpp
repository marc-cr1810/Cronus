#include "floatobject.h"

/*
	Methods
*/

static void float_dealloc(CrFloatObject* self)
{
	Cr_TYPE(self)->tp_free((CrObject*)self);
}

CrTypeObject CrFloatType = {
	VAROBJECT_HEAD_INIT(&CrFloatType, 0) // base
	"float",							 // tp_name
	"Float object type",				 // tp_doc
	sizeof(CrFloatObject),				 // tp_size
	0,									 // tp_itemsize
	(destructor)float_dealloc,			 // tp_dealloc
	Mem_Free							 // tp_free
};

CrObject* CrFloatObject_FromDouble(double dval)
{
	CrFloatObject* obj = CrObject_NEW(CrFloatObject, &CrFloatType);
	obj->ob_fval = dval;
	return (CrObject*)obj;
}