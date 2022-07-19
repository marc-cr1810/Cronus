#include "listobject.h"

#include <objects/baseobject.h>
#include <core/error.h>

/*
* 
*		Helper functions
* 
*/


static int list_resize(CrListObject* self, Cr_size_t new_size)
{
	CrObject** items;
	size_t new_allocated, num_allocated_bytes;
	Cr_size_t allocated = self->allocated;

	if (allocated >= new_size && new_size >= (allocated >> 1))
	{
		assert(self->ob_items != NULL || new_size != 0);
		VAROBJECT_SET_SIZE(self, new_size);
		return 0;
	}

	/* This over-allocates proportional to the list size, making room
	 * for additional growth.  The over-allocation is mild, but is enough to give
	 * linear-time amortized behavior over a long sequence of appends() in the
	 * presence of a poorly-performing system realloc().
	 * Add padding to make the allocated size multiple of 4.
	 * The growth pattern is:  0, 4, 8, 16, 24, 32, 40, 52, 64, 76, ...
	 * Note: new_allocated won't overflow because the largest possible value
	 *       is VI_SIZE_T_MAX * (9 / 8) + 6 which always fits in a size_t.
	 */
	new_allocated = ((size_t)new_size + (new_size >> 3) + 6) & ~(size_t)3;
	/* Do not overallocate if the new size is closer to overallocated size
	 * than to the old size.
	 */
	if (new_size - Cr_SIZE(self) > (Cr_size_t)(new_allocated - new_size))
		new_allocated = ((size_t)new_size + 3) & ~(size_t)3;

	if (new_size == 0)
		new_allocated = 0;
	num_allocated_bytes = new_allocated * sizeof(CrObject*);
	items = (CrObject**)Mem_Realloc(self->ob_items, num_allocated_bytes);
	if (items == NULL)
	{
		CrError_SetString(CrExc_MemoryError, "out of memory");
		return -1;
	}
	self->ob_items = items;
	VAROBJECT_SET_SIZE(self, new_size);
	self->allocated = new_allocated;
	return 0;
}

static int append(CrListObject* self, CrObject* obj)
{
	Cr_size_t n = CrList_GET_SIZE(self);

	assert(obj != NULL);
	assert((size_t)n + 1 < CR_SIZE_T_MAX);
	if (list_resize(self, n + 1) < 0)
		return -1;

	CrObject_INCREF(obj);
	CrList_SET_ITEM(self, n, obj);
	return 0;
}

static inline int valid_index(Cr_size_t i, Cr_size_t limit)
{
	return (size_t)i < (size_t)limit;
}

//
//
//		Methods
//
//

static void list_dealloc(CrListObject* self)
{
	Cr_size_t i;
	// Untrack list for GC
	// begin trashcan
	if (self->ob_items != NULL)
	{
		i = Cr_SIZE(self);
		while (--i >= 0)
		{
			CrObject_DECREF(self->ob_items[i]);
		}
		Mem_Free(self->ob_items);
	}
	Cr_TYPE(self)->tp_free((CrObject*)self);

	// end trashcan
}

CrTypeObject CrListType = {
	VAROBJECT_HEAD_INIT(&CrListType, 0)		// base
	"list",									// tp_name
	"List object type",						// tp_doc
	sizeof(CrListObject),					// tp_size
	0,										// tp_itemsize
	TPFLAGS_DEFAULT | TPFLAGS_BASETYPE |	// tp_flags
		TPFLAGS_LIST_SUBCLASS,
	(destructor)list_dealloc,				// tp_dealloc
	&CrBaseObjectType,						// tp_base
	Mem_Free								// tp_free
};

CrObject* CrListObject_New(Cr_size_t size)
{
	CrListObject* obj;
	size_t alloc;

	obj = CrObject_NEW(CrListObject, &CrListType);
	if (obj == NULL)
		return NULL;
	if (size == 0)
	{
		obj->ob_items = NULL;
		alloc = 0;
	}
	else
	{
		alloc = size * sizeof(CrObject*);
		obj->ob_items = (CrObject**)Mem_Alloc(alloc);
	}
	VAROBJECT_SET_SIZE(obj, size);
	obj->allocated = size;
	return (CrObject*)obj;
}

int CrList_Append(CrObject* list, CrObject* new_item)
{
	if (CrList_Check(list) && (new_item != NULL))
		return append((CrListObject*)list, new_item);
	CrError_SetString(CrExc_SystemError, "Bad internal call");
	return -1;
}

int CrList_SetItem(CrObject* list, Cr_size_t i, CrObject* newitem)
{
	CrObject** p;
	if (CrList_Check(list))
	{
		CrObject_XDECREF(newitem);
		CrError_BadInternalCall();
		return -1;
	}
	if (!valid_index(i, Cr_SIZE(list)))
	{
		CrObject_XDECREF(newitem);
		CrError_SetString(CrExc_IndexError, "list assignment index out of range");
		return -1;
	}
	p = ((CrListObject*)list)->ob_items + i;
	CrObject_XSETREF(*p, newitem);
	return 0;
}

CrStatus CrList_InitTypes()
{
	if (CrType_Ready(&CrListType) < 0)
		return CrStatus_Error("can't init list type");

	return CrStatus_Ok();
}
