#pragma once

#include <port.h>
#include <objects/object.h>

typedef struct _arena CrArena;

CrArena* CrArena_New();
void CrArena_Free(CrArena* arena);

void* CrArena_Alloc(CrArena* arena, size_t size);

int CrArena_AddCrObject(CrArena* arena, CrObject* obj);