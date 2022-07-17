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

int Cr_FileIsInteractive(std::ifstream* fp, CrObject* filename);

int CrRun_FileObject(std::ifstream* fp, const char* filename, bool close);
int CrRun_FileObject(std::ifstream* fp, CrObject* filename, bool close);
int CrRun_InteractiveObject(std::ifstream* fp, CrObject* filename);
int CrRun_InteractiveLoop(std::ifstream* fp, CrObject* filename);
int CrRun_SimpleFileObject(std::ifstream* fp, CrObject* filename, bool close);