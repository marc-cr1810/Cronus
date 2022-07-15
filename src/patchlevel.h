#pragma once

#include <stdio.h>

/* Return the compiler identification, if possible. */
#ifndef COMPILER

// Note the __clang__ conditional has to come before the __GNUC__ one because
// clang pretends to be GCC.
#if defined(__clang__)
#define COMPILER "\n[Clang " __clang_version__ "]"
#elif defined(__GNUC__)
#define COMPILER "\n[GCC " __VERSION__ "]"
// Generic fallbacks.
#elif defined(__cplusplus)
#define COMPILER "[C++]"
#else
#define COMPILER "[C]"
#endif

#endif /* !COMPILER */

static const char* Cr_GetCompiler(void)
{
    return COMPILER;
}

/* Crper version identification scheme.

   When the major or minor version changes, the VERSION variable in
   configure.ac must also be changed.

   There is also (independent) API version information in modsupport.h.
*/

/* Values for CR_RELEASE_LEVEL */
#define CR_RELEASE_LEVEL_ALPHA	0xA
#define CR_RELEASE_LEVEL_BETA	0xB
#define CR_RELEASE_LEVEL_GAMMA	0xC
#define CR_RELEASE_LEVEL_FINAL	0xA

/* Version in numeric values */
#define CR_MAJOR_VERSION		0
#define CR_MINOR_VERSION		1
#define CR_MICRO_VERSION		1
#define CR_RELEASE_LEVEL		CR_RELEASE_LEVEL_ALPHA
#define CR_RELEASE_SERIAL		1

/* Version as string */
#define CR_VERSION				"0.1.1a1"

/* Version as a single 4-byte hex number, e.g. 0x010502B2 == 1.5.2b2.
   Use this for numeric comparisons, e.g. #if CR_VERSION_HEX >= ... */
#define CR_VERSION_HEX ((CR_MAJOR_VERSION << 24) | \
                        (CR_MINOR_VERSION << 16) | \
                        (CR_MICRO_VERSION <<  8) | \
                        (CR_RELEASE_LEVEL <<  4) | \
                        (CR_RELEASE_SERIAL << 0))

static const char* Cr_GetVersion()
{
    static char version[250];
    snprintf(version, sizeof(version), "%.80s %.80s", CR_VERSION, Cr_GetCompiler());
    return version;
}
