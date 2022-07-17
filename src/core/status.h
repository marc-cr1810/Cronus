#pragma once

#include <port.h>

typedef struct _status
{
	enum _type
	{
		CrStatus_TYPE_OK = 0,
		CrStatus_TYPE_ERROR = 1,
		CrStatus_TYPE_EXIT = 2
	} type;
	const char* func;
	const char* error_msg;
	int exit_code;
} CrStatus;

CrStatus CrStatus_Ok();
CrStatus CrStatus_Error(const char* error_msg);
CrStatus CrStatus_NoMemory();
CrStatus CrStatus_Exit(int exit_code = 0);

bool CrStatus_IsError(CrStatus status);
bool CrStatus_IsExit(CrStatus status);
bool CrStatus_Exception(CrStatus status);