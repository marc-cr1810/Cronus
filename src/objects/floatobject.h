#pragma once

#include "object.h"

typedef struct _floatobject
{
	CrObject_HEAD
		double ob_fval;
} CrFloatObject;

extern CrTypeObject CrFloatType;

/* Convert a C++ double to a CrFloatObject */
CrObject* CrFloatObject_FromDouble(double dval);