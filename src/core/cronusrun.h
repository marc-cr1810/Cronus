#pragma once

#include <port.h>
#include <objects/object.h>

#include <core/runtime.h>
#include <core/status.h>
#include <core/error.h>

CrStatus Cr_Initialize();

void Cr_Exit(int status);

CrStatus CrRuntime_Initialize();
int CrRuntime_Finalize();