#pragma once

#include <port.h>
#include <core/status.h>
#include <objects/object.h>

typedef struct _intobject CrIntObject;

/* Type object */
extern CrTypeObject CrBoolType;

#define CrBool_Check(x) Cr_IS_TYPE(x, &CrBoolType)

/* Cr_True and CrFalse are the only two bools in existence
Dont forget to apply CrObject_INCREF() when returning either one! */

/* Do not use directly */
extern CrIntObject CrTrueStruct, CrFalseStruct;

/* Use these macros */
#define Cr_True ((CrObject *) &CrTrueStruct)
#define Cr_False ((CrObject *) &CrFalseStruct)

/* Macros for returning Cr_True and Cr_False */
#define Cr_RETURN_TRUE return CrObject_NEWREF(Cr_True);
#define Cr_RETURN_FALSE return CrObject_NEWREF(Cr_False);

/* Function to return bool from a int */
CrObject* CrBool_FromLong(int b);

/* Runtime lifecycle */
CrStatus CrBool_InitTypes();