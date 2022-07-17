#include "baseobject.h"

static void object_dealloc(CrObject* self)
{
	Cr_TYPE(self)->tp_free(self);
}

CrTypeObject CrBaseType = {
	VAROBJECT_HEAD_INIT(&CrBaseType, 0)		// base
	"type",									// tp_name
	"Base type",							// tp_doc
	sizeof(CrObject),						// tp_size
	0,										// tp_itemsize
	TPFLAGS_DEFAULT | TPFLAGS_BASETYPE,		// tp_flags
	0,										// tp_dealloc
	0,										// tp_base
	Mem_Free								// tp_free
};

CrTypeObject CrBaseObjectType = {
	VAROBJECT_HEAD_INIT(&CrBaseType, 0)		// base
	"object",								// tp_name
	"Base object type",						// tp_doc
	sizeof(CrObject),						// tp_size
	0,										// tp_itemsize
	TPFLAGS_DEFAULT | TPFLAGS_BASETYPE,		// tp_flags
	(destructor)object_dealloc,				// tp_dealloc
	0,										// tp_base
	Mem_Free								// tp_free
};