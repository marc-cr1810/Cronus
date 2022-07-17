#include "object.h"


static int type_is_subtype_chain(CrTypeObject* a, CrTypeObject* b)
{
	///TODO: Implement a base type object
	//do {
	//	if (a == b)
	//		return 1;
	//	a = a->tp_base;
	//} while (a != NULL);
	//return (b == &CrBaseObjectType);
	return 0;
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