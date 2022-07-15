#pragma once

#include <assert.h>
#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include <vector>
#include <regex>

#ifndef MS_WINDOWS
#include <sys/stat.h>
#include <cmath>
#endif

#include "config.h"

#ifdef MS_WINDOWS

typedef int8_t    Cr_int8_t;
typedef int16_t   Cr_int16_t;
typedef int32_t   Cr_int32_t;
typedef int64_t   Cr_int64_t;

typedef uint8_t   Cr_uint8_t;
typedef uint16_t  Cr_uint16_t;
typedef uint32_t  Cr_uint32_t;
typedef uint64_t  Cr_uint64_t;

#else

typedef __int8_t    Cr_int8_t;
typedef __int16_t   Cr_int16_t;
typedef __int32_t   Cr_int32_t;
typedef __int64_t   Cr_int64_t;

typedef __uint8_t   Cr_uint8_t;
typedef __uint16_t  Cr_uint16_t;
typedef __uint32_t  Cr_uint32_t;
typedef __uint64_t  Cr_uint64_t;

#endif

typedef float       Cr_float_t;
typedef double      Cr_double_t;

typedef std::string Cr_string_t;

typedef intptr_t Cr_intptr_t;
typedef Cr_intptr_t Cr_size_t;
typedef Cr_size_t Cr_hash_t;

/* Largest possible value of size_t. */
#define CR_SIZE_MAX SIZE_MAX

/* Largest positive value of type Cr_size_t. */
#define CR_SIZE_T_MAX ((Cr_size_t)(((size_t)-1)>>1))
/* Smallest negative value of type Cr_size_t. */
#define CR_SIZE_T_MIN (-CR_SIZE_T_MAX-1)

#define CR_CHARMASK(c) ((unsigned char)((c) & 0xFF))

template<typename First, typename ... T>
bool ValueIsIn(First&& first, T && ... t)
{
    return ((first == t) || ...);
}

/* Below "a" is a power of 2. */
/* Round down size "n" to be a multiple of "a". */
#define Cr_SIZE_ROUND_DOWN(n, a) ((size_t)(n) & ~(size_t)((a) - 1))
/* Round up size "n" to be a multiple of "a". */
#define Cr_SIZE_ROUND_UP(n, a) (((size_t)(n) + (size_t)((a) - 1)) & ~(size_t)((a) - 1))
/* Round pointer "p" down to the closest "a"-aligned address <= "p". */
#define Cr_ALIGN_DOWN(p, a) ((void *)((uintptr_t)(p) & ~(uintptr_t)((a) - 1)))
/* Round pointer "p" up to the closest "a"-aligned address >= "p". */
#define Cr_ALIGN_UP(p, a) ((void *)(((uintptr_t)(p) + (uintptr_t)((a) - 1)) & ~(uintptr_t)((a) - 1)))
/* Check if pointer "p" is aligned to "a"-bytes boundary. */
#define Cr_IS_ALIGNED(p, a) (!((uintptr_t)(p) & (uintptr_t)((a) - 1)))

#ifndef CrAPI_FUNC
#   define CrAPI_FUNC(RTYPE) RTYPE
#endif
#ifndef CrAPI_DATA
#   define CrAPI_DATA(RTYPE) extern RTYPE
#endif