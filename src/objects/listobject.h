#pragma once

#include <objects/object.h>
#include <core/status.h>

typedef struct _listobject
{
	CrObject_VAR_HEAD;
	/* Vector of pointers to list elements.  list[0] is ob_item[0], etc. */
	CrObject** ob_items;
	Cr_size_t allocated;
} CrListObject;

/* Type object */
extern CrTypeObject CrListType;

/* Type check macros */
#define CrList_Check(self) CrObject_TypeCheck(self, &CrListType)
#define CrList_CheckExact(self) Cr_IS_TYPE(self, &CrListType)

CrObject* CrListObject_New(Cr_size_t size);

/* API Functions */
int CrList_Append(CrObject* list, CrObject* new_item);
int CrList_SetItem(CrObject* list, Cr_size_t i, CrObject* newitem);

#define CrList_CAST(obj) (assert(CrList_Check(obj)), ((CrListObject*)obj))

#define CrList_GET_ITEM(obj, i)     (CrList_CAST(obj)->ob_items[i])
#define CrList_SET_ITEM(obj, i, v)  ((void)(CrList_CAST(obj)->ob_items[i] = (v)))
#define CrList_GET_SIZE(obj)        Cr_SIZE(CrList_CAST(obj))

/* Runtime lifecycle */
CrStatus CrList_InitTypes();
