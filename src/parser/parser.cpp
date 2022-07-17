#include "parser.h"

#include <core/errorcode.h>
#include <core/error.h>

#define CrParser_PrintTest(func, check) D(fprintf(stderr, "%*c> " func "[%d-%d]: %s?\n", p->level, ' ', mark, p->mark, check))
#define CrParser_PrintSuccess(func, check) D(fprintf(stderr, "%*c+ " func "[%d-%d]: %s succeeded!\n", p->level, ' ', mark, p->mark, check))
#define CrParser_PrintFail(func, check) D(fprintf(stderr, "%*c- " func "[%d-%d]: %s failed!\n", p->level, ' ', mark, p->mark, check))

#define RULE_HEAD()			\
D(p->level++);				\
if (p->error_indicator)	\
{							\
	D(p->level--);			\
	return NULL;			\
}

static inline void* CHECK_CALL(Parser* p, void* result)
{
	if (result == NULL)
	{
		assert(CrError_Occurred());
		p->error_indicator = 1;
	}
	return result;
}

#define CHECK(type, result) ((type) CHECK_CALL(p, result))

#define EXTRA start_lineno, start_col_offset, end_lineno, end_col_offset

static const int keywordListSize = KEYWORD_COUNT;
static KeywordToken keywords[][KEYWORDS_MAX] = {
	{ // 0
		{NULL, TOK_UNKNOWN}
	},
	{ // 1
		{NULL, TOK_UNKNOWN}
	},
	{ // 2
		{"if", TOK_IF},
		{"do", TOK_DO},
		{NULL, TOK_UNKNOWN}
	},
	{ // 3
		{"for", TOK_FOR},
		{NULL, TOK_UNKNOWN}},
	{ // 4
		{"else", TOK_ELSE},
		{"func", TOK_FUNC},
		{"Null", TOK_NULL},
		{"True", TOK_TRUE},
		{NULL, TOK_UNKNOWN}
	},
	{ // 5
		{"while", TOK_WHILE},
		{"class", TOK_CLASS},
		{"async", TOK_ASYNC},
		{"await", TOK_AWAIT},
		{"False", TOK_FALSE},
		{NULL, TOK_UNKNOWN}
	},
	{ // 6
		{NULL, TOK_UNKNOWN}
	},
	{ // 7
		{NULL, TOK_UNKNOWN}
	},
	{ // 8
		{NULL, TOK_UNKNOWN}
	},
	{ // 9
		{"extension", TOK_EXTENSION},
		{NULL, TOK_UNKNOWN}
	} };

// API function definitions

Parser* CrParser_New(TokState* tok, int startRule, int* error_code)
{
	Parser* p = (Parser*)Mem_Alloc(sizeof(Parser));
	if (p == NULL)
	{
		CrError_SetString(CrExc_SystemError, "out of memory");
		return NULL;
	}

	assert(tok != NULL);
	p->mode = startRule;
	p->error_indicator = 0;
	p->error_code = error_code;
	p->parsing_started = 0;
	p->flags = 0;
	p->starting_lineno = 0;
	p->starting_col_offset = 0;

	p->tok = tok;
	p->tokens = (Token**)Mem_Alloc(sizeof(Token*));
	if (!p->tokens)
	{
		Mem_Free(p->tokens);
		CrError_SetString(CrExc_MemoryError, "out of memory");
		return NULL;
	}
	p->tokens[0] = (Token*)Mem_Calloc(1, sizeof(Token));
	if (!p->tokens)
	{
		Mem_Free(p->tokens[0]);
		Mem_Free(p->tokens);
		CrError_SetString(CrExc_MemoryError, "out of memory");
		return NULL;
	}
	p->size = 1;
	p->fill = 0;

	p->keywordListSize = keywordListSize;
	p->keywords = keywords;

	p->mark = 0;
	p->level = 0;

	return p;
}

void* CrParser_Parse(Parser* p)
{
    return nullptr;
}

void CrParser_Free(Parser* p)
{
	for (int i = 0; i < p->size; i++)
	{
		Mem_Free(p->tokens[i]);
	}
	Mem_Free(p->tokens);
	Mem_Free(p);
}
