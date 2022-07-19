#include "lifecycle.h"

#include <Cronus.h>

CrStatus CrCore_InitTypes()
{
	CrStatus status;

	status = CrInt_InitTypes();
	if (CrStatus_Exception(status))
		return status;

	status = CrFloat_InitTypes();
	if (CrStatus_Exception(status))
		return status;

	status = CrString_InitTypes();
	if (CrStatus_Exception(status))
		return status;

	status = CrList_InitTypes();
	if (CrStatus_Exception(status))
		return status;

	status = CrBool_InitTypes();
	if (CrStatus_Exception(status))
		return status;

	return CrStatus_Ok();
}
