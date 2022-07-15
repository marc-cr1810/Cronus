#include "object.h"

#include "core/crmem.h"

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