#pragma once

#include <port.h>
#include <core/mem.h>

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
	const char* tp_name;			// CrObject type name
	const char* tp_doc;				// Documentation string
	size_t tp_size;
	size_t tp_itemsize;				// For memory allocation
	unsigned long tp_flags;			// Flags for specific features and behaviour

	/* Methods to implement standard operations */

	destructor tp_dealloc;

	
	struct _typeobject* tp_base;	// Strong reference on a heap type, borrowed reference on a static type
	freefunc tp_free;				// Low-level free memory routine
} CrTypeObject;

#define Cr_TYPE(ob)             (CrObject_CAST(ob)->ob_type)
#define Cr_SIZE(ob)             (CrVarObject_CAST(ob)->ob_size)

static inline int _Cr_IS_TYPE(const CrObject* ob, const CrTypeObject* type) {
	return Cr_TYPE(ob) == type;
}
#define Cr_IS_TYPE(ob, type) _Cr_IS_TYPE(CrObject_CAST_CONST(ob), type)

/* Generic type check */
int CrType_IsSubtype(CrTypeObject* a, CrTypeObject* b);
#define CrObject_TypeCheck(ob, tp) (Cr_IS_TYPE(ob, tp) || CrType_IsSubtype(Cr_TYPE(ob), (tp)))

/*
	Functions
*/

/* Create a new object */
CrObject* Object_New(CrTypeObject* type);
#define CrObject_NEW(type, typedef) (type *)Object_New(typedef)

/* Create a new reference to an object */
void ObjectNewRef(CrObject* obj);

/*
 * CrObject reference count modification
 *
 * Objects keep track of the amount of references to them they have.
 * If an objects reference count reaches 0, it is free'd from memory.
*/

/* Increase object reference count */
static inline void ObjectIncRef(CrObject* obj)
{
	obj->ob_refcount++;
}
#define CrObject_INCREF(obj) ObjectIncRef(CrObject_CAST(obj))

/* Decrease object reference count */
static inline void ObjectDecRef(CrObject* obj)
{
	obj->ob_refcount--;
	if (obj->ob_refcount == 0)
		obj->ob_type->tp_dealloc(obj);
}
#define CrObject_DECREF(obj) ObjectDecRef(CrObject_CAST(obj))

/* Increase object reference count incase object pointer can be NULL */
static inline void ObjectXIncRef(CrObject* obj)
{
	if (obj != NULL)
		ObjectIncRef(obj);
}
#define CrObject_XINCREF(obj) ObjectXIncRef(CrObject_CAST(obj))

/* Decrease object reference count incase object pointer can be NULL */
static inline void ObjectXDecRef(CrObject* obj)
{
	if (obj != NULL)
		ObjectDecRef(obj);
}
#define CrObject_XDECREF(obj) ObjectXDecRef(CrObject_CAST(obj))

// Create a new strong reference to an object:
// increment the reference count of the object and return the object.
static CrObject* Object_NewRef(CrObject* obj);
#define CrObject_NEWREF(obj) Object_NewRef(CrObject_CAST(obj));

// Similar to Cr_NewRef(), but the object can be NULL.
static CrObject* Object_XNewRef(CrObject* obj);
#define CrObject_XNEWREF(obj) Object_XNewRef(CrObject_CAST(obj));

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

int CrType_Ready(CrTypeObject* type);

/*
 *	Type object flags
*/

/* Set if the type object is dynamically allocated */
#define TPFLAGS_HEAPTYPE (1UL << 9)

/* Set if the type allows subclassing */
#define TPFLAGS_BASETYPE (1UL << 10)

/* Set if the type is 'ready' -- fully initialized */
#define TPFLAGS_READY (1UL << 12)

/* Set while the type is being 'readied', to prevent recursive ready calls */
#define TPFLAGS_READYING (1UL << 13)

/* Objects support garbage collection */
#define TPFLAGS_HAVE_GC (1UL << 14)

/* These flags are used to determine if a type is a subclass. */
#define TPFLAGS_LONG_SUBCLASS        (1UL << 24)
#define TPFLAGS_LIST_SUBCLASS        (1UL << 25)
#define TPFLAGS_TUPLE_SUBCLASS       (1UL << 26)
#define TPFLAGS_BYTES_SUBCLASS       (1UL << 27)
#define TPFLAGS_UNICODE_SUBCLASS     (1UL << 28)
#define TPFLAGS_DICT_SUBCLASS        (1UL << 29)
#define TPFLAGS_BASE_EXC_SUBCLASS    (1UL << 30)
#define TPFLAGS_TYPE_SUBCLASS        (1UL << 31)

#define TPFLAGS_DEFAULT 0

static inline int CrType_HasFeature(CrTypeObject* type, unsigned long feature)
{
	return ((type->tp_flags & feature) != 0);
}

#define CrType_FastSubClass(type, flag) CrType_HasFeature(type, flag)

/*
 CrNullStruct is an object of undefined type which can be used in contexts
 where NULL (null) is not suitable (since NULL often means 'error').

 Don't forget to apply CrObject_NEWREF() when returning this value!!!
 */

extern CrObject CrNullStruct; // Do not use directly
#define Cr_Null (&CrNullStruct)

/* Macro for returning Cr_Null from a function */
#define Cr_RETURN_NULL return CrObject_NEWREF(Cr_Null)