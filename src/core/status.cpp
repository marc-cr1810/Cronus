#include "status.h"

#ifdef _MSC_VER
/* Crsual Studio 2015> doesn't implement C99 __func__ in C */
#  define CrStatus_GET_FUNC() __FUNCTION__
#else
#  define CrStatus_GET_FUNC() __func__
#endif

#define CrStatus_OK() { CrStatus::CrStatus_TYPE_OK, }

#define CrStatus_ERROR(ERROR_MSG) \
	{ \
		CrStatus::CrStatus_TYPE_ERROR, \
		CrStatus_GET_FUNC(), \
		(ERROR_MSG), \
	}

#define CrStatus_NO_MEMORY() CrStatus_ERROR("memory allocation failed")

#define CrStatus_EXIT(EXIT_CODE) \
	{ \
		CrStatus::CrStatus_TYPE_EXIT, \
		"", \
		"", \
		(EXIT_CODE) \
	}

#define CrStatus_IS_ERROR(status) (status.type == CrStatus::CrStatus_TYPE_ERROR)
#define CrStatus_IS_EXIT(status) (status.type == CrStatus::CrStatus_TYPE_EXIT)
#define CrStatus_EXCEPTION(status) (status.type != CrStatus::CrStatus_TYPE_OK)

#define CrStatus_UPDATE_FUNC(status) do { status.func = CrStatus_GET_FUNC(); } while (0)

CrStatus CrStatus_Ok()
{
	CrStatus status = CrStatus_OK();
	return status;
}

CrStatus CrStatus_Error(const char* error_msg)
{
	assert(error_msg != NULL);
	CrStatus status = CrStatus_ERROR(error_msg);
	return status;
}

CrStatus CrStatus_NoMemory()
{
	return CrStatus_Error("memory allocation failed");
}

CrStatus CrStatus_Exit(int exit_code)
{
	CrStatus status = CrStatus_EXIT(exit_code);
	return status;
}

bool CrStatus_IsError(CrStatus status)
{
	return CrStatus_IS_ERROR(status);
}

bool CrStatus_IsExit(CrStatus status)
{
	return CrStatus_IS_EXIT(status);
}

bool CrStatus_Exception(CrStatus status)
{
	return CrStatus_EXCEPTION(status);
}
