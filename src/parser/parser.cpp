#include "parser.h"

#include <core/errorcode.h>
#include <core/error.h>
#include <objects/stringobject.h>
#include <parser/generator.h>

#define MAXSTACK 6000

#define CrParser_PrintTest(func, check) D(fprintf(stderr, "%*c> " func "[%d-%d]: %s?\n", p->level, ' ', mark, p->mark, check))
#define CrParser_PrintSuccess(func, check) D(fprintf(stderr, "%*c+ " func "[%d-%d]: %s succeeded!\n", p->level, ' ', mark, p->mark, check))
#define CrParser_PrintFail(func, check) D(fprintf(stderr, "%*c- " func "[%d-%d]: %s failed!\n", p->level, ' ', mark, p->mark, check))

#define RULE_HEAD()			\
if (p->level++ == MAXSTACK) \
{							\
	p->error_indicator = 1;	\
	CrError_NoMemory();		\
}							\
if (p->error_indicator)		\
{							\
	p->level--;				\
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

/* Helper functions */

//
// RULE DEFINITIONS
//

/* Rule types */

#define type_file_mode 1000
#define type_interactive_mode 1001
#define type_eval_mode 1002
#define type_string_mode 1003

// Forward declarations

static mod_type rule_file_mode(Parser* p);
static mod_type rule_interactive_mode(Parser* p);
static mod_type rule_eval_mode(Parser* p);
static expr_type rule_string_mode(Parser* p);
static ast_stmt_seq* rule_statement_newline(Parser* p);

//
// Definitions
//

// file: statements? $
static mod_type rule_file_mode(Parser* p)
{
	CrError_SetString(CrExc_SystemError, "parser rule not implemented");
	return NULL;
}

// interactive: statement_newline
static mod_type rule_interactive_mode(Parser* p)
{
	RULE_HEAD();

	mod_type result = NULL;
	int mark = p->mark;
	{ // statement_newline
		if (p->error_indicator)
		{
			D(p->level--);
			return NULL;
		}
		CrParser_PrintTest("interactive_mode", "statement_newline");
		ast_stmt_seq* stmt_sq;

		if (
			(stmt_sq = rule_statement_newline(p)) // statement_newline
			)
		{
			CrParser_PrintSuccess("interactive_mode", "statement_newline");
			result = CrAST_Interactive(stmt_sq);
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				D(p->level--);
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("interactive_mode", "statement_newline");
	}
	result = NULL;
done:
	D(p->level--);
	return result;
}

// eval: expressions NEWLINE* $
static mod_type rule_eval_mode(Parser* p)
{
	CrError_SetString(CrExc_SystemError, "parser rule not implemented");
	return NULL;
}

// string: star_expressions
static expr_type rule_string_mode(Parser* p)
{
	CrError_SetString(CrExc_SystemError, "parser rule not implemented");
	return NULL;
}

// statement_newline: compound_stmt NEWLINE | simple_stmts | NEWLINE | $
static ast_stmt_seq* rule_statement_newline(Parser* p)
{
	RULE_HEAD();

	ast_stmt_seq* result = NULL;
	int mark = p->mark;
	if (p->mark == p->fill && CrGen_FillToken(p) < 0)
	{
		p->error_indicator = 1;
		D(p->level--);
		return NULL;
	}

	int start_lineno = p->tokens[mark]->lineno;
	int start_col_offset = p->tokens[mark]->col_offset;

	{ // NEWLINE
		if (p->error_indicator)
		{
			D(p->level--);
			return NULL;
		}
		CrParser_PrintTest("statement_newline", "NEWLINE");
		Token* newline_var;
		if (
			(newline_var = CrGen_ExpectToken(p, TOK_NEWLINE)) // token = 'NEWLINE'
			)
		{
			CrParser_PrintSuccess("statement_newline", "NEWLINE");
			Token* token = CrGen_GetLastNonWhitespaceToken(p);
			if (token == NULL)
			{
				D(p->level--);
				return NULL;
			}
			int end_lineno = token->end_lineno;
			int end_col_offset = token->end_col_offset;
			result = (ast_stmt_seq*)CrGen_SingletonSeq(p, CHECK(stmt_type, CrAST_Pass(EXTRA)));
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				D(p->level--);
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("statement_newline", "NEWLINE");
	}
	{ // $
		if (p->error_indicator)
		{
			D(p->level--);
			return NULL;
		}
		CrParser_PrintTest("statement_newline", "$");
		Token* endmarker_var;
		if (
			(endmarker_var = CrGen_ExpectToken(p, TOK_ENDMARKER)) // token = 'ENDMARKER'
			)
		{
			CrParser_PrintSuccess("statement_newline", "$");
			result = CrGen_InteractiveExit(p);
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				D(p->level--);
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("statement_newline", "$");
	}
	result = NULL;
done:
	D(p->level--);
	return result;
}

//
// End of Definitions
//

static int newline_in_string(Parser* p, const char* cur)
{
	for (const char* c = cur; c >= p->tok->buf; c--)
	{
		if (*c == '\'' || *c == '"')
		{
			return 1;
		}
	}
	return 0;
}

/* Check that the source for a single input statement really is a single
   statement by looking at what is left in the buffer after parsing.
   Trailing whitespace and comments are OK. */
static int bad_single_statement(Parser* p)
{
	const char* cur = strchr(p->tok->buf, '\n');

	/* Newlines are allowed if preceded by a line continuation character
	   or if they appear inside a string. */
	if (!cur || (cur != p->tok->buf && *(cur - 1) == '\\') || newline_in_string(p, cur))
	{
		return 0;
	}
	char c = *cur;

	for (;;)
	{
		while (c == ' ' || c == '\t' || c == '\n' || c == '\014')
		{
			c = *++cur;
		}

		if (!c)
		{
			return 0;
		}

		if (c != '#')
		{
			return 1;
		}

		// Suck up comment.
		while (c && c != '\n')
		{
			c = *++cur;
		}
	}
}

static void parser_reset_state(Parser* p)
{
	for (int i = 0; i < p->fill; i++)
		p->tokens[i]->memo = NULL;
	p->mark = 0;
}

static void* parser_parse(Parser* p)
{
	void* result = NULL;
	switch (p->mode)
	{
	case PARSER_MODE_FILE_INPUT:
		result = rule_file_mode(p);
		break;
	case PARSER_MODE_SINGLE_INPUT:
		result = rule_interactive_mode(p);
		break;
	case PARSER_MODE_EVAL_INPUT:
		result = rule_eval_mode(p);
		break;
	case PARSER_MODE_STRING_INPUT:
		result = rule_string_mode(p);
		break;
	}
	return result;
}

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
	void* result = parser_parse(p);
	if (result == NULL)
	{
		parser_reset_state(p);
		parser_parse(p);
		if (CrError_Occurred())
			return NULL;

		if (p->fill == 0)
			CrError_SetString(CrExc_SyntaxError, "error at start before reading any input");
		else if (p->tok->done == E_EOF)
			CrError_SetString(CrExc_SyntaxError, "unexpected EOF while parsing");
		else
		{
			if (p->tokens[p->fill - 1]->type == TOK_INDENT)
				CrError_SetString(CrExc_IndentationError, "unexpected indent");
			else if (p->tokens[p->fill - 1]->type == TOK_DEDENT)
				CrError_SetString(CrExc_IndentationError, "unexpected unindent");
			else
				CrError_SetString(CrExc_SyntaxError, "invalid syntax");
		}
		return NULL;
	}
	if (p->mode == PARSER_MODE_SINGLE_INPUT && bad_single_statement(p))
	{
		p->tok->done = E_BADSINGLE;
		CrError_SetString(CrExc_SyntaxError, "multiple statements found while compiling a single statement");
		return NULL;
	}
	return result;
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
