#include "generator.h"

#include <core/error.h>
#include <core/errorcode.h>
#include <objects/stringobject.h>
#include <objects/intobject.h>
#include <objects/listobject.h>
#include <objects/floatobject.h>

#define EXTRA_EXPR(head, tail) head->lineno, (head)->col_offset, (tail)->end_lineno, (tail)->end_col_offset, p->arena

/* AST node pair functions */

CmpopExprPair* CrGen_CmpopExprPair(Parser* p, cmpop_type cmpop, expr_type expr)
{
	assert(expr != NULL);
	CmpopExprPair* cmpopexprpair = (CmpopExprPair*)CrArena_Alloc(p->arena, sizeof(CmpopExprPair));
	if (!cmpopexprpair)
		return NULL;
	cmpopexprpair->cmpop = cmpop;
	cmpopexprpair->expr = expr;
	return cmpopexprpair;
}

ast_int_seq* CrGen_GetCmpops(Parser* p, ast_seq* seq)
{
	Cr_size_t len = CrAST_SEQ_LENGTH(seq);
	assert(len > 0);

	ast_int_seq* new_seq = CrAST_NewIntSeq(len, p->arena);
	if (!new_seq)
		return NULL;
	for (Cr_size_t i = 0; i < len; i++)
	{
		CmpopExprPair* pair = (CmpopExprPair*)CrAST_SEQ_GET_UNTYPED(seq, i);
		CrAST_SEQ_SET(new_seq, i, pair->cmpop);
	}
	return new_seq;
}

ast_expr_seq* CrGen_GetExprs(Parser* p, ast_seq* seq)
{
	Cr_size_t len = CrAST_SEQ_LENGTH(seq);
	assert(len > 0);

	ast_expr_seq* new_seq = CrAST_NewExprSeq(len, p->arena);
	if (!new_seq)
		return NULL;
	for (Cr_size_t i = 0; i < len; i++)
	{
		CmpopExprPair* pair = (CmpopExprPair*)CrAST_SEQ_GET_UNTYPED(seq, i);
		CrAST_SEQ_SET(new_seq, i, pair->expr);
	}
	return new_seq;
}

/* Memoization functions */

/*	
*	Instrumentation to count the effectiveness of memoization.
*	The array counts the number of tokens skipped by memoization,
*	indexed by type.
*/

#define NSTATISTICS 2000
static long memo_statistics[NSTATISTICS];

void CrGen_Memo_ClearStats()
{
	for (int i = 0; i < NSTATISTICS; i++)
	{
		memo_statistics[i] = 0;
	}
}

CrObject* CrGen_Memo_GetStats()
{
	CrObject* ret = CrListObject_New(NSTATISTICS);
	if (ret == NULL)
	{
		return NULL;
	}
	for (int i = 0; i < NSTATISTICS; i++)
	{
		CrObject* value = CrIntObject_FromInt(memo_statistics[i]);
		if (value == NULL)
		{
			CrObject_DECREF(ret);
			return NULL;
		}
		// CrList_SetItem borrows a reference to value.
		if (CrList_SetItem(ret, i, value) < 0)
		{
			CrObject_DECREF(ret);
			return NULL;
		}
	}
	return ret;
}

int CrGen_IsMemoized(Parser* p, int type, void* pres)
{
	if (p->mark == p->fill)
	{
		if (CrGen_FillToken(p) < 0)
		{
			p->error_indicator = 1;
			return -1;
		}
	}

	Token* t = p->tokens[p->mark];

	for (Memo* m = t->memo; m != NULL; m = m->next)
	{
		if (m->type == type)
		{
			if (0 <= type && type < NSTATISTICS)
			{
				long count = m->mark - p->mark;
				// A memoized negative result counts for one.
				if (count <= 0)
				{
					count = 1;
				}
				memo_statistics[type] += count;
			}
			p->mark = m->mark;
			*(void**)(pres) = m->node;
			return 1;
		}
	}
	return 0;
}

// Here, mark is the start of the node, while p->mark is the end.
// If node==NULL, they should be the same.
int CrGen_Memo_Insert(Parser* p, int mark, int type, void* node)
{
	// Insert in front
	Memo* m = (Memo*)Mem_Alloc(sizeof(Memo));
	if (m == NULL)
		return -1;
	m->type = type;
	m->node = node;
	m->mark = p->mark;
	m->next = p->tokens[mark]->memo;
	p->tokens[mark]->memo = m;
	return 0;
}

// Like CrGen_Memo_Insert(), but updates an existing node if found.
int CrGen_Memo_Update(Parser* p, int mark, int type, void* node)
{
	for (Memo* m = p->tokens[mark]->memo; m != NULL; m = m->next)
	{
		if (m->type == type)
		{
			// Update existing node
			m->node = node;
			m->mark = p->mark;
			return 0;
		}
	}
	// Insert new node
	return CrGen_Memo_Insert(p, mark, type, node);
}

/*
* 	Parser tokenizer functions
*/

static int CrGen_TokenizerError(Parser* p)
{
	if (CrError_Occurred())
	{
		return -1;
	}

	const char* msg = NULL;
	CrObject* errtype = CrExc_SyntaxError;
	switch (p->tok->done)
	{
	case E_TOKEN:
		msg = "invalid token";
		break;
	case E_EOFS:
		msg = "EOF while scanning triple-quoted string literal";
		break;
	case E_EOLS:
		msg = "EOL while scanning string literal";
		break;
	case E_EOF:
		msg = "unexpected EOF while parsing";
		break;
	case E_DEDENT:
		errtype = CrExc_IndentationError;
		msg = "unindent does not match any outer indentation level";
		break;
	case E_INTR:
		if (!CrError_Occurred())
		{
			CrError_SetNone(CrExc_KeyboardInterrupt);
		}
		return -1;
	case E_NOMEM:
		CrError_NoMemory();
		return -1;
	case E_TABSPACE:
		errtype = CrExc_TabError;
		msg = "inconsistent use of tabs and spaces in indentation";
		break;
	case E_TOODEEP:
		errtype = CrExc_IndentationError;
		msg = "too many levels of indentation";
		break;
	case E_LINECONT:
		msg = "unexpected character after line continuation character";
		break;
	default:
		msg = "unknown parsing error";
		break;
	}

	CrError_SetString(errtype, msg);

	return TOK_UNKNOWN;
}

static int CrGen_GetKeywordOrName(Parser* p, const char* name, int name_len)
{
	assert(name_len > 0);
	if (name_len >= p->keywordListSize ||
		p->keywords[name_len] == NULL ||
		p->keywords[name_len]->type == TOK_UNKNOWN)
	{
		return TOK_NAME;
	}
	for (KeywordToken* k = p->keywords[name_len]; k != NULL && k->type != -1; k++)
	{
		if (strncmp(k->name, name, name_len) == 0)
		{
			return k->type;
		}
	}
	return TOK_NAME;
}

int CrGen_FillToken(Parser* p)
{
	const char* start;
	const char* end;
	int type = CrTokenizer_Get(p->tok, &start, &end);

	// Skip '# type: ignore' comments
	while (type == TOK_TYPE_IGNORE)
	{
		type = CrTokenizer_Get(p->tok, &start, &end);
	}

	if (type == TOK_ENDMARKER && p->mode == PARSER_MODE_SINGLE_INPUT && p->parsing_started)
	{
		type = TOK_NEWLINE; /* Add an extra newline */
		p->parsing_started = 0;

		if (p->tok->indent && !(p->flags & PARSE_DONT_IMPLY_DEDENT))
		{
			p->tok->pendin = -p->tok->indent;
			p->tok->indent = 0;
		}
	}
	else
	{
		p->parsing_started = 1;
	}

	if (p->fill == p->size)
	{
		int newsize = p->size * 2;
		Token** new_tokens = (Token**)Mem_Realloc(p->tokens, newsize * sizeof(Token*));
		if (new_tokens == NULL)
		{
			CrError_NoMemory();
			return TOK_UNKNOWN;
		}
		p->tokens = new_tokens;

		for (int i = p->size; i < newsize; i++)
		{
			p->tokens[i] = (Token*)Mem_Alloc(sizeof(Token));
			if (p->tokens[i] == NULL)
			{
				p->size = i; // Needed, in order to cleanup correctly after parser fails
				CrError_NoMemory();
				return TOK_UNKNOWN;
			}
			memset(p->tokens[i], '\0', sizeof(Token));
		}
		p->size = newsize;
	}

	Token* t = p->tokens[p->fill];
	t->type = static_cast<token_type>((type == TOK_NAME) ? CrGen_GetKeywordOrName(p, start, (int)(end - start)) : type);
	t->value = CrStringObject_FromStringAndSize(start, end - start);
	if (t->value == NULL)
		return TOK_UNKNOWN;

	int lineno = type == TOK_STRING ? p->tok->first_lineno : p->tok->lineno;
	const char* line_start = type == TOK_STRING ? p->tok->multi_line_start : p->tok->line_start;
	int end_lineno = p->tok->lineno;
	int col_offset = -1;
	int end_col_offset = -1;
	if (start != NULL && start >= line_start)
	{
		col_offset = (int)(start - line_start);
	}
	if (end != NULL && end >= p->tok->line_start)
	{
		end_col_offset = (int)(end - p->tok->line_start);
	}

	t->lineno = p->starting_lineno + lineno;
	t->col_offset = p->tok->lineno == 1 ? p->starting_col_offset + col_offset : col_offset;
	t->end_lineno = p->starting_lineno + end_lineno;
	t->end_col_offset = p->tok->lineno == 1 ? p->starting_col_offset + end_col_offset : end_col_offset;

	p->fill += 1;

	if (type == TOK_ERRORTOKEN)
	{
		if (p->tok->done == E_DECODE)
		{
			CrError_SetString(CrExc_SystemError, "unknown error");
			return TOK_UNKNOWN;
		}
		return CrGen_TokenizerError(p);
	}

	return TOK_ENDMARKER;
}

Token* CrGen_ExpectToken(Parser* p, int type)
{
	if (p->mark == p->fill)
	{
		if (CrGen_FillToken(p) < 0)
		{
			p->error_indicator = 1;
			return NULL;
		}
	}
	Token* t = p->tokens[p->mark];
	if (t->type != type)
		return NULL;
	p->mark += 1;
	return t;
}

Token* CrGen_ExpectForcedToken(Parser* p, int type)
{
	if (p->error_indicator == 1)
		return NULL;

	if (p->mark == p->fill)
	{
		if (CrGen_FillToken(p) < 0)
		{
			p->error_indicator = 1;
			return NULL;
		}
	}

	Token* t = p->tokens[p->mark];
	if (t->type != type)
	{
		CrError_SetString(CrExc_SyntaxError, "expected ':'");
		return NULL;
	}
	p->mark += 1;
	return t;
}

int CrGen_LookaheadWithInt(int positive, Token* (func)(Parser*, int), Parser* p, int arg)
{
	int mark = p->mark;
	void* result = func(p, arg);
	p->mark = mark;
	return (result != NULL) == positive;
}

int CrGen_Lookahead(int positive, void* (func)(Parser*), Parser* p)
{
	int mark = p->mark;
	void* res = (void*)func(p);
	p->mark = mark;
	return (res != NULL) == positive;
}

Token* CrGen_GetLastNonWhitespaceToken(Parser* p)
{
	assert(p->mark >= 0);
	Token* token = NULL;
	for (int m = p->mark - 1; m >= 0; m--)
	{
		token = p->tokens[m];
		if (token->type != TOK_ENDMARKER && (token->type < TOK_NEWLINE || token->type > TOK_DEDENT))
		{
			break;
		}
	}
	return token;
}

void* CrGen_StringToken(Parser* p)
{
	return CrGen_ExpectToken(p, TOK_STRING);
}

static CrObject* parse_number_raw(const char* s)
{
	const char* end;
	long x;
	double dx;

	assert(s != NULL);
	errno = 0;
	end = s + strlen(s) - 1;
	if (s[0] == '0')
	{

		x = (long)strtoul(s, (char**)&end, 0);
		if (x < 0 && errno == 0)
		{
			return CrIntObject_FromString(s, 0);
		}
	}
	else
	{
		x = strtol(s, (char**)&end, 0);
	}
	if (*end == '\0')
	{
		if (errno != 0)
		{
			return CrIntObject_FromString(s, 0);
		}
		return CrIntObject_FromInt(x);
	}
	dx = strtod(s, NULL);
	if (dx == -1.0 && CrError_Occurred())
	{
		return NULL;
	}
	return CrFloatObject_FromDouble(dx);
}

static CrObject* parse_number(const char* s)
{
	char* dup;
	char* end;
	CrObject* result = NULL;

	assert(s != NULL);

	if (strchr(s, '_') == NULL)
	{
		return parse_number_raw(s);
	}
	/* Create a duplicate without underscores. */
	dup = (char*)Mem_Alloc(strlen(s) + 1);
	if (dup == NULL)
	{
		CrError_NoMemory();
		return NULL;
	}
	end = dup;
	for (; *s; s++)
	{
		if (*s != '_')
		{
			*end++ = *s;
		}
	}
	*end = '\0';
	result = parse_number_raw(dup);
	Mem_Free(dup);
	return result;
}

expr_type CrGen_NumberToken(Parser* p)
{
	Token* t = CrGen_ExpectToken(p, TOK_NUMBER);
	if (t == NULL)
		return NULL;

	char* raw_num = CrString_ToString(t->value);
	if (raw_num == NULL)
	{
		p->error_indicator = 1;
		return NULL;
	}

	CrObject* num = parse_number(raw_num);
	if (num == NULL)
	{
		p->error_indicator = 1;
		return NULL;
	}

	if (CrArena_AddCrObject(p->arena, num) < 0)
	{
		CrObject_DECREF(num);
		p->error_indicator = 1;
		return NULL;
	}

	return CrAST_Constant(num, NULL, t->lineno, t->col_offset, t->end_lineno, t->end_col_offset, p->arena);
}

static expr_type CrGen_NameFromToken(Parser* p, Token* t)
{
	if (t == NULL) {
		return NULL;
	}
	const char* s = CrString_ToString(t->value);
	if (!s) {
		p->error_indicator = 1;
		return NULL;
	}
	CrObject* id = CrGen_NewIdentifier(p, s);
	if (id == NULL) {
		p->error_indicator = 1;
		return NULL;
	}
	return CrAST_Name(id, Load, t->lineno, t->col_offset, t->end_lineno, t->end_col_offset, p->arena);
}

expr_type CrGen_NameToken(Parser* p)
{
	Token* t = CrGen_ExpectToken(p, TOK_NAME);
	return CrGen_NameFromToken(p, t);
}

ast_seq* CrGen_SingletonSeq(Parser* p, void* a)
{
	assert(a != NULL);
	ast_seq* seq = (ast_seq*)CrAST_NewGenericSeq(1, p->arena);
	if (!seq)
		return NULL;
	CrAST_SEQ_SET_UNTYPED(seq, 0, a);
	return seq;
}

static Cr_size_t CrGen_GetFlattenedSeqSize(ast_seq* seqs)
{
	Cr_size_t size = 0;
	for (Cr_size_t i = 0, l = CrAST_SEQ_LENGTH(seqs); i < l; i++)
	{
		ast_seq* inner_seq = (ast_seq*)CrAST_SEQ_GET_UNTYPED(seqs, i);
		size += CrAST_SEQ_LENGTH(inner_seq);
	}
	return size;
}

ast_seq* CrGen_SeqFlatten(Parser* p, ast_seq* seqs)
{
	Cr_size_t flattened_seq_size = CrGen_GetFlattenedSeqSize(seqs);
	assert(flattened_seq_size > 0);

	ast_seq* flattened_seq = (ast_seq*)CrAST_NewGenericSeq(flattened_seq_size, p->arena);
	if (!flattened_seq)
		return NULL;

	int flattened_seq_idx = 0;
	for (Cr_size_t i = 0, l = CrAST_SEQ_LENGTH(seqs); i < l; i++)
	{
		ast_seq* inner_seq = (ast_seq*)CrAST_SEQ_GET_UNTYPED(seqs, i);
		for (Cr_size_t j = 0, li = CrAST_SEQ_LENGTH(inner_seq); j < li; j++)
		{
			CrAST_SEQ_SET_UNTYPED(flattened_seq, flattened_seq_idx++, CrAST_SEQ_GET_UNTYPED(inner_seq, j));
		}
	}
	assert(flattened_seq_idx == flattened_seq_size);
	return flattened_seq;
}

// Creates an ast_seq* where all the elements have been changed to have ctx as context
static ast_expr_seq* CrGen_SetSeqContext(Parser* p, ast_expr_seq* seq, expr_context_type ctx)
{
	Cr_size_t len = CrAST_SEQ_LENGTH(seq);
	if (len == 0)
		return NULL;

	ast_expr_seq* new_seq = CrAST_NewExprSeq(len, p->arena);
	if (!new_seq)
		return NULL;

	for (Cr_size_t i = 0; i < len; i++)
	{
		expr_type expr = CrAST_SEQ_GET(seq, i);
		CrAST_SEQ_SET(new_seq, i, CrGen_SetExprContext(p, expr, ctx));
	}
	return new_seq;
}

static expr_type CrGen_SetNameContext(Parser* p, expr_type expr, expr_context_type ctx)
{
	return CrAST_Name(expr->v.Name.id, ctx, EXTRA_EXPR(expr, expr));
}

static expr_type CrGen_SetTupleContext(Parser* p, expr_type expr, expr_context_type ctx)
{
	return CrAST_Tuple(CrGen_SetSeqContext(p, expr->v.Tuple.elts, ctx), ctx, EXTRA_EXPR(expr, expr));
}

static expr_type CrGen_SetListContext(Parser* p, expr_type expr, expr_context_type ctx)
{
	return CrAST_List(CrGen_SetSeqContext(p, expr->v.Tuple.elts, ctx), ctx, EXTRA_EXPR(expr, expr));
}

static expr_type CrGen_SetSubscriptContext(Parser* p, expr_type expr, expr_context_type ctx)
{
	return CrAST_Subscript(expr->v.Subscript.value, expr->v.Subscript.slice, ctx, EXTRA_EXPR(expr, expr));
}

static expr_type CrGen_SetAttributeContext(Parser* p, expr_type expr, expr_context_type ctx)
{
	return CrAST_Attribute(expr->v.Attribute.value, expr->v.Attribute.attr, ctx, EXTRA_EXPR(expr, expr));
}

static expr_type CrGen_SetStarredContext(Parser* p, expr_type expr, expr_context_type ctx)
{
	return CrAST_Starred(CrGen_SetExprContext(p, expr->v.Starred.value, ctx), ctx, EXTRA_EXPR(expr, expr));
}

expr_type CrGen_SetExprContext(Parser* p, expr_type expr, expr_context_type ctx)
{
	assert(expr != NULL);

	expr_type new_expr = NULL;
	switch (expr->kind)
	{
	case expr_kind::Name_kind:
		new_expr = CrGen_SetNameContext(p, expr, ctx);
		break;
	case expr_kind::Tuple_kind:
		new_expr = CrGen_SetTupleContext(p, expr, ctx);
		break;
	case expr_kind::List_kind:
		new_expr = CrGen_SetListContext(p, expr, ctx);
		break;
	case expr_kind::Subscript_kind:
		new_expr = CrGen_SetSubscriptContext(p, expr, ctx);
		break;
	case expr_kind::Attribute_kind:
		new_expr = CrGen_SetAttributeContext(p, expr, ctx);
		break;
	case expr_kind::Starred_kind:
		new_expr = CrGen_SetStarredContext(p, expr, ctx);
		break;
	default:
		new_expr = expr;
	}
	return new_expr;
}

ast_stmt_seq* CrGen_InteractiveExit(Parser* p)
{
	if (p->error_code)
	{
		*(p->error_code) = E_EOF;
	}
	return NULL;
}

mod_type CrGen_ASTFromFileObject(std::ifstream* fp, CrObject* filename, int mode, const char* ps1, const char* ps2, int* error_code, CrArena* arena)
{
	TokState* tok = CrTokenizer_FromFile(fp, ps1, ps2);
	if (tok == NULL)
	{
		CrError_SetString(CrExc_SystemError, "failed to initialize tokenizer");
		return NULL;
	}

	tok->filename = filename;
	CrObject_INCREF(filename);

	mod_type result = NULL;

	Parser* p = CrParser_New(tok, mode, error_code, arena);
	if (p == NULL)
		goto error;

	result = (mod_type)CrParser_Parse(p);
	CrParser_Free(p);
error:
	CrTokenizer_Free(tok);
	return result;
}

CrObject* CrGen_NewIdentifier(Parser* p, const char* n)
{
	CrObject* id = CrStringObject_FromString(n);
	if (!id)
		goto error;

	if (CrArena_AddCrObject(p->arena, id) < 0)
	{
		CrObject_DECREF(id);
		goto error;
	}
	return id;
error:
	p->error_indicator = 1;
	return NULL;
}

CrObject* CrGen_NewTypeComment(Parser* p, Token* tc)
{
	CrObject* tco;
	if (tc == NULL)
		return NULL;
	const char* str = CrString_ToString(tc->value);
	if (str == NULL)
		goto error;

	tco = CrStringObject_FromString(str);
	if (tco == NULL)
		goto error;
	if (CrArena_AddCrObject(p->arena, tco) < 0)
	{
		CrObject_DECREF(tco);
		goto error;
	}
	return tco;
error:
	p->error_indicator = 1;
	return NULL;
}
