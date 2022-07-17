#include "mem.h"

void* Mem_Alloc(size_t size)
{
	if (size == 0)
		size = 1;
	return malloc(size);
}

void* Mem_Calloc(size_t elemCount, size_t elemSize)
{
	if (elemCount == 0 || elemSize == 0)
	{
		elemCount = 1;
		elemSize = 1;
	}
	return calloc(elemCount, elemSize);
}

void* Mem_Realloc(void* ptr, size_t new_size)
{
	if (new_size == 0)
		new_size = 1;
	return realloc(ptr, new_size);
}

void Mem_Free(void* ptr)
{
	free(ptr);
}