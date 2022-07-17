#pragma once

#include <port.h>

#define CR_CTF_LOWER  0x01
#define CR_CTF_UPPER  0x02
#define CR_CTF_ALPHA  (CR_CTF_LOWER|CR_CTF_UPPER)
#define CR_CTF_DIGIT  0x04
#define CR_CTF_ALNUM  (CR_CTF_ALPHA|CR_CTF_DIGIT)
#define CR_CTF_SPACE  0x08
#define CR_CTF_XDIGIT 0x10

extern const unsigned int Cr_ctype_table[256];

/* Unlike their C counterparts, the following macros are not meant to
 * handle an int with any of the values [EOF, 0-UCHAR_MAX]. The argument
 * must be a signed/unsigned char. */
#define Cr_ISLOWER(c)  (Cr_ctype_table[CR_CHARMASK(c)] & CR_CTF_LOWER)
#define Cr_ISUPPER(c)  (Cr_ctype_table[CR_CHARMASK(c)] & CR_CTF_UPPER)
#define Cr_ISALPHA(c)  (Cr_ctype_table[CR_CHARMASK(c)] & CR_CTF_ALPHA)
#define Cr_ISDIGIT(c)  (Cr_ctype_table[CR_CHARMASK(c)] & CR_CTF_DIGIT)
#define Cr_ISXDIGIT(c) (Cr_ctype_table[CR_CHARMASK(c)] & CR_CTF_XDIGIT)
#define Cr_ISALNUM(c)  (Cr_ctype_table[CR_CHARMASK(c)] & CR_CTF_ALNUM)
#define Cr_ISSPACE(c)  (Cr_ctype_table[CR_CHARMASK(c)] & CR_CTF_SPACE)

extern const unsigned char Cr_ctype_tolower[256];
extern const unsigned char Cr_ctype_toupper[256];

#define Cr_TOLOWER(c) (Cr_ctype_tolower[CR_CHARMASK(c)])
#define Cr_TOUPPER(c) (Cr_ctype_toupper[CR_CHARMASK(c)])