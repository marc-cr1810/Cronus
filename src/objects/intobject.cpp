#include "intobject.h"

/*
	Methods
*/

static void int_dealloc(CrIntObject* self)
{
	Cr_TYPE(self)->tp_free((CrObject*)self);
}

CrTypeObject CrIntType = {
	VAROBJECT_HEAD_INIT(&CrIntType, 0)		// base
	"int",									// tp_name
	"Integer object type",					// tp_doc
	sizeof(CrIntObject),					// tp_size
	0,										// tp_itemsize
	TPFLAGS_DEFAULT | TPFLAGS_BASETYPE,		// tp_flags
	(destructor)int_dealloc,				// tp_dealloc
	0,										// tp_base
	Mem_Free								// tp_free
};

CrObject* CrIntObject_FromInt(Cr_int32_t ival)
{
	CrIntObject* obj = CrObject_NEW(CrIntObject, &CrIntType);
	obj->ob_ival = ival;
	return (CrObject*)obj;
}

CrObject* CrIntObject_FromString(const char* str, int base)
{
	CrIntObject* obj = CrObject_NEW(CrIntObject, &CrIntType);
	obj->ob_ival = std::stoi(str, nullptr, base);
	return (CrObject*)obj;
}

CrStatus CrInt_InitTypes()
{
	if (CrType_Ready(&CrIntType) < 0)
		return CrStatus_Error("can't init int type");

	return CrStatus_Ok();
}
