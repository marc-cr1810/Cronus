#pragma once

#include "port.h"
#include "core/crmem.h"

struct _typeobject;

/* CrObject_HEAD defines the initial segment of every CrObject. */
#define CrObject_HEAD CrObject ob_base;

#define CrObject_HEAD_INIT(type) \
	{1, type},

#define VAROBJECT_HEAD_INIT(type, size) \
	{CrObject_HEAD_INIT(type) size},

/* OBJECT_VAR_HEAD defines the initial segment of all variable-size
   container objects.  
   These end with a declaration of an array with 1 element, but enough space 
   is malloc'ed so that the array actually has room for ob_size elements.
   Note that ob_size is an element count, not necessarily a byte count.
 */
#define CrObject_VAR_HEAD CrVarObject ob_base

typedef struct _object
{
	intptr_t ob_refcount;
	_typeobject* ob_type;
} CrObject;

/* Cast argument to CrObject* type. */
#define CrObject_CAST(op) ((CrObject*)(op))
#define CrObject_CAST_CONST(op) ((const CrObject*)(op))

typedef struct _varobject
{
	CrObject ob_base;
	intptr_t ob_size;
} CrVarObject;

/* Cast argument to CrVarObject* type. */
#define CrVarObject_CAST(op) ((CrVarObject*)(op))
#define CrVarObject_CAST_CONST(op) ((const CrVarObject*)(op))

/*
Type objects contain a string containing the type name (to help somewhat
in debugging), the allocation parameters (see Object_New() and Object_NewVar()),
and methods for accessing objects of the type.
Methods are optional, a nil pointer meaning that particular kind of access is 
not available for this type.
The DECREF() macro uses the tp_dealloc method without checking for a nil pointer;
it should always be implemented except if the implementation can guarantee that 
the reference count will never reach zero (e.g., for statically allocated type objects).

NB: the methods for certain type groups are now contained in separate
method blocks.
*/

typedef void (*freefunc)(void*);
typedef void (*destructor)(CrObject*);

typedef struct _typeobject
{
	CrObject_VAR_HEAD;
	const char* tp_name;	// CrObject type name
	const char* tp_doc;		// Documentation string
	size_t tp_size;
	size_t tp_itemsize; // For memory allocation

	/* Methods to implement standard operations */
	destructor tp_dealloc;

	freefunc tp_free; // Low-level free memory routine
} CrTypeObject;

#define Cr_TYPE(ob)             (CrObject_CAST(ob)->ob_type)

#define Cr_SIZE(ob)             (CrVarObject_CAST(ob)->ob_size)

/*
	Functions
*/

/* Create a new object */
CrObject* Object_New(CrTypeObject* type);
#define CrObject_NEW(type, typedef) (type *)Object_New(typedef)

/* Create a new reference to an object */
void ObjectNewRef(CrObject* obj);

/* Set the object type */
static inline void ObjectSetType(CrObject* obj, CrTypeObject* type)
{
	obj->ob_type = type;
}
#define CrObject_SET_TYPE(obj, type) ObjectSetType(CrObject_CAST(obj), type)

/* Set the objects element (or byte) size */
static inline void VarObjectSetSize(CrVarObject* obj, intptr_t size)
{
	obj->ob_size = size;
}
#define VAROBJECT_SET_SIZE(obj, size) VarObjectSetSize(CrVarObject_CAST(obj), size)

/* Initlialize an object */
static inline void ObjectInit(CrObject* obj, CrTypeObject* type)
{
	ObjectSetType(obj, type);
	ObjectNewRef(obj);
}