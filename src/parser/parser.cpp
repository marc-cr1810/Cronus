#include "parser.h"

#include <core/errorcode.h>
#include <core/error.h>
#include <objects/stringobject.h>
#include <parser/generator.h>
#include <objects/boolobject.h>

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

#define EXTRA start_lineno, start_col_offset, end_lineno, end_col_offset, p->arena

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
		{"from", TOK_FROM},
		{NULL, TOK_UNKNOWN}
	},
	{ // 5
		{"while", TOK_WHILE},
		{"class", TOK_CLASS},
		{"async", TOK_ASYNC},
		{"await", TOK_AWAIT},
		{"False", TOK_FALSE},
		{"yield", TOK_YIELD},
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
#define type_statement_newline 1004
#define type_simple_statements 1005
#define type_simple_statement 1006
#define type_assignment 1007
#define type_star_expressions 1008
#define type_star_expression 1009
#define type_expression 1010
#define type_disjunction 1011
#define type_conjunction 1012
#define type_inversion 1013
#define type_comparison 1014
#define type_bitwise_or 1015
#define type_bitwise_xor 1016
#define type_bitwise_and 1017
#define type_shift_expr 1018
#define type_sum 1019
#define type_term 1020
#define type_factor 1021
#define type_power 1022
#define type_await_primary 1023
#define type_primary 1024
#define type_atom 1025
#define type_strings 1026
#define type_loop_strings 1027
#define type_looprule_14 1028
#define type_gatherrule_219 1029
#define type_star_targets 1030
#define type_star_target 1031
#define type_target_with_star_atom 1032
#define type_star_atom 1033
#define type_gatherrule_15 1034
#define type_yield_expr 1035

// Forward declarations
/*
* NOTE: All of this is temporary. Eventually I'll finish my code writing tool to generate all of these automatically from
* the rules. This way I won't have to hand write every single function by hand which would take forever.
* 
* This is just to test to make sure its working and get the formatting of the functions correct.
*/

static mod_type rule_file_mode(Parser* p);
static mod_type rule_interactive_mode(Parser* p);
static mod_type rule_eval_mode(Parser* p);
static expr_type rule_string_mode(Parser* p);

static ast_stmt_seq* rule_statement_newline(Parser* p);
static ast_stmt_seq* rule_simple_statements(Parser* p);

static stmt_type rule_simple_statement(Parser* p);
static stmt_type rule_assignment(Parser* p);
static expr_type rule_star_expressions(Parser* p);
static expr_type rule_star_expression(Parser* p);
static expr_type rule_expression(Parser* p);
static expr_type rule_disjunction(Parser* p);
static expr_type rule_conjunction(Parser* p);
static expr_type rule_inversion(Parser* p);
static expr_type rule_comparison(Parser* p);
static expr_type rule_bitwise_or(Parser* p);
static expr_type rule_bitwise_xor(Parser* p);
static expr_type rule_bitwise_and(Parser* p);
static expr_type rule_shift_expr(Parser* p);
static expr_type rule_sum(Parser* p);
static expr_type rule_term(Parser* p);
static expr_type rule_factor(Parser* p);
static expr_type rule_power(Parser* p);
static expr_type rule_await_primary(Parser* p);
static expr_type rule_primary(Parser* p);
static expr_type rule_atom(Parser* p);
static expr_type rule_strings(Parser* p);
static ast_seq* rule_loop_string(Parser* p);
static expr_type rule_star_targets(Parser* p);
static expr_type rule_star_target(Parser* p);
static expr_type rule_target_with_star_atom(Parser* p);
static expr_type rule_star_atom(Parser* p);
static expr_type rule_yield_expr(Parser* p);

// Loops, gathers, etc
static ast_seq* looprule_14(Parser* p);

static void* gatherrule_15(Parser* p);
static void* gatherrule_219(Parser* p);

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
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("interactive_mode", "statement_newline");
		ast_stmt_seq* stmt_sq;

		if (
			(stmt_sq = rule_statement_newline(p)) // statement_newline
			)
		{
			CrParser_PrintSuccess("interactive_mode", "statement_newline");
			result = CrAST_Interactive(stmt_sq, p->arena);
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("interactive_mode", "statement_newline");
	}
	result = NULL;
done:
	p->level--;
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
		p->level--;
		return NULL;
	}

	int start_lineno = p->tokens[mark]->lineno;
	int start_col_offset = p->tokens[mark]->col_offset;


	{ // simple_statements
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("statement_newline", "simple_statements");
		ast_stmt_seq* simple_stmts_var;
		if (
			(simple_stmts_var = rule_simple_statements(p)) // simple_statements
			)
		{
			CrParser_PrintSuccess("statement_newline", "simple_statements");
			result = simple_stmts_var;
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("statement_newline", "simple_statements");
	}
	{ // NEWLINE
		if (p->error_indicator)
		{
			p->level--;
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
				p->level--;
				return NULL;
			}
			int end_lineno = token->end_lineno;
			int end_col_offset = token->end_col_offset;
			result = (ast_stmt_seq*)CrGen_SingletonSeq(p, CHECK(stmt_type, CrAST_Pass(EXTRA)));
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
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
			p->level--;
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
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("statement_newline", "$");
	}
	result = NULL;
done:
	p->level--;
	return result;
}

// simple_statements: simple_statement NEWLINE
static ast_stmt_seq* rule_simple_statements(Parser* p)
{
	RULE_HEAD();

	ast_stmt_seq* result = NULL;
	int mark = p->mark;
	{ // simple_statement NEWLINE
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("simple_statements", "simple_statement NEWLINE");
		stmt_type stmt;
		Token* newline_var;
		if (
			(stmt = rule_simple_statement(p)) // simple_statement
			&&
			(newline_var = CrGen_ExpectToken(p, TOK_NEWLINE)) // token = 'NEWLINE'
			)
		{
			CrParser_PrintSuccess("simple_statements", "simple_statement NEWLINE");
			result = (ast_stmt_seq*)CrGen_SingletonSeq(p, stmt);
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("simple_statements", "simple_statement NEWLINE");
	}
done:
	p->level--;
	return result;
}

// simple_statement:
//     | assignment
//     | star_expressions
//     | &'return' return_statement
//     | &('import' | 'from') import_statement
//     | &'raise' raise_statement
//     | 'pass'
//     | &'del' del_statement
//     | &'yield' yield_statement
//     | &'assert' assert_statement
//     | 'break'
//     | 'continue'
//     | &'global' global_statement
//     | &'nonlocal' nonlocal_statement
static stmt_type rule_simple_statement(Parser* p)
{
	RULE_HEAD();

	stmt_type result = NULL;
	if (CrGen_IsMemoized(p, type_simple_statement, &result))
	{
		p->level--;
		return result;
	}
	int mark = p->mark;
	if (p->mark == p->fill && CrGen_FillToken(p) < 0)
	{
		p->error_indicator = 1;
		p->level--;
		return NULL;
	}

	int start_lineno = p->tokens[mark]->lineno;
	int start_col_offset = p->tokens[mark]->col_offset;
	{ // assignment
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("simple_statement", "assignment");
		stmt_type assignment_var;
		if (
			(assignment_var = rule_assignment(p)) // assignment
			)
		{
			CrParser_PrintSuccess("simple_statement", "assignment");
			result = assignment_var;
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("simple_statement", "assignment");
	}
	{ // star_expressions
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("simple_statement", "star_expressions");
		expr_type expr;
		if (
			(expr = rule_star_expressions(p)) // star_expressions
			)
		{
			CrParser_PrintSuccess("simple_statement", "star_expressions");
			Token* token = CrGen_GetLastNonWhitespaceToken(p);
			if (token == NULL)
			{
				p->level--;
				return NULL;
			}
			int end_lineno = token->end_lineno;
			int end_col_offset = token->end_col_offset;
			result = CrAST_Expr(expr, EXTRA);
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("simple_statement", "star_expressions");
	}
	result = NULL;
done:
	CrGen_Memo_Insert(p, mark, type_simple_statement, result);
	p->level--;
	return result;
}

// assignment:
//     | NAME ':' expression ['=' annotated_rhs]
//     | ('(' single_target ')' | single_subscript_attribute_target) ':' expression ['=' annotated_rhs]
//     | ((star_targets '='))+ (yield_expr | star_expressions) !'=' TYPE_COMMENT?
//     | single_target augassign ~ (yield_expr | star_expressions)
//     | invalid_assignment
static stmt_type rule_assignment(Parser* p)
{
	RULE_HEAD();

	stmt_type result = NULL;
	int mark = p->mark;
	if (p->mark == p->fill && CrGen_FillToken(p) < 0)
	{
		p->error_indicator = 1;
		p->level--;
		return NULL;
	}

	int start_lineno = p->tokens[mark]->lineno;
	int start_col_offset = p->tokens[mark]->col_offset;

	{ // ((star_targets '='))+ (yield_expr | star_expressions) !'=' TYPE_COMMENT?
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("assignment", "((star_targets '='))+ (yield_expr | star_expressions) !'=' TYPE_COMMENT?");
		ast_expr_seq* targets;
		void* expr;
		Token* tc;
		if (
			(targets = (ast_expr_seq*)looprule_14(p)) // ((star_targets '='))+
			&&
			(expr = gatherrule_15(p)) // yeild_expr | star_expressions
			&&
			(CrGen_LookaheadWithInt(0, CrGen_ExpectToken, p, TOK_EQUAL)) // token != '='
			&&
			(tc = CrGen_ExpectToken(p, TOK_TYPE_COMMENT), !p->error_indicator)
			)
		{
			CrParser_PrintSuccess("assignment", "((star_targets '='))+ (yield_expr | star_expressions) !'=' TYPE_COMMENT?");
			Token* token = CrGen_GetLastNonWhitespaceToken(p);
			if (token == NULL)
			{
				p->level--;
				return NULL;
			}
			int end_lineno = token->end_lineno;
			int end_col_offset = token->end_col_offset;
			result = CrAST_Assign(targets, (expr_type)expr, CrGen_NewTypeComment(p, tc), EXTRA);
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("assignment", "((star_targets '='))+ (yield_expr | star_expressions) !'=' TYPE_COMMENT?");
	}

	result = NULL;
done:
	p->level--;
	return result;
}

// star_expressions:
//     | star_expression ((',' star_expression))+ ','?
//     | star_expression ','
//     | star_expression
static expr_type rule_star_expressions(Parser* p)
{
	RULE_HEAD();

	expr_type result = NULL;
	int mark = p->mark;
	if (p->mark == p->fill && CrGen_FillToken(p) < 0)
	{
		p->error_indicator = 1;
		p->level--;
		return NULL;
	}

	int start_lineno = p->tokens[mark]->lineno;
	int start_col_offset = p->tokens[mark]->col_offset;
	{ // star_expression
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("star_expressions", "star_expression");
		expr_type star_expression_var;
		if (
			(star_expression_var = rule_star_expression(p)) // star_expression
			)
		{
			CrParser_PrintSuccess("star_expressions", "star_expression");
			result = star_expression_var;
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("star_expressions", "star_expression");
	}
	result = NULL;
done:
	p->level--;
	return result;
}

// star_expression: '*' bitwise_or | expression
static expr_type rule_star_expression(Parser* p)
{
	RULE_HEAD();

	expr_type result = NULL;
	if (CrGen_IsMemoized(p, type_star_expression, &result))
	{
		p->level--;
		return result;
	}

	int mark = p->mark;
	if (p->mark == p->fill && CrGen_FillToken(p) < 0)
	{
		p->error_indicator = 1;
		p->level--;
		return result;
	}

	int start_lineno = p->tokens[mark]->lineno;
	int start_col_offset = p->tokens[mark]->col_offset;
	{ // expression
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("star_expression", "expression");
		expr_type expression_var;
		if (
			(expression_var = rule_expression(p)) // expression
			)
		{
			CrParser_PrintSuccess("star_expression", "expression");
			result = expression_var;
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("star_expression", "expression");
	}
	result = NULL;
done:
	CrGen_Memo_Insert(p, mark, type_star_expression, result);
	p->level--;
	return result;
}

// expression: disjunction 'if' disjunction 'else' expression | disjunction | lambdef
static expr_type rule_expression(Parser* p)
{
	RULE_HEAD();

	expr_type result = NULL;
	if (CrGen_IsMemoized(p, type_expression, &result))
	{
		p->level--;
		return result;
	}

	int mark = p->mark;
	if (p->mark == p->fill && CrGen_FillToken(p) < 0)
	{
		p->error_indicator = 1;
		p->level--;
		return result;
	}

	int start_lineno = p->tokens[mark]->lineno;
	int start_col_offset = p->tokens[mark]->col_offset;
	{ // disjunction
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("expression", "disjunction");
		expr_type disjunction_var;
		if (
			(disjunction_var = rule_disjunction(p)) // disjunction
			)
		{
			CrParser_PrintSuccess("expression", "disjunction");
			result = disjunction_var;
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("expression", "disjunction");
	}
	result = NULL;
done:
	CrGen_Memo_Insert(p, mark, type_expression, result);
	p->level--;
	return result;
}

// disjunction: conjunction (('or' conjunction))+ | conjunction
static expr_type rule_disjunction(Parser* p)
{
	RULE_HEAD();

	expr_type result = NULL;
	if (CrGen_IsMemoized(p, type_disjunction, &result))
	{
		p->level--;
		return result;
	}

	int mark = p->mark;
	if (p->mark == p->fill && CrGen_FillToken(p) < 0)
	{
		p->error_indicator = 1;
		p->level--;
		return result;
	}

	int start_lineno = p->tokens[mark]->lineno;
	int start_col_offset = p->tokens[mark]->col_offset;
	{ // conjunction
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("disjunction", "conjunction");
		expr_type conjunction_var;
		if (
			(conjunction_var = rule_conjunction(p)) // conjunction
			)
		{
			CrParser_PrintSuccess("disjunction", "conjunction");
			result = conjunction_var;
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("disjunction", "conjunction");
	}
	result = NULL;
done:
	CrGen_Memo_Insert(p, mark, type_disjunction, result);
	p->level--;
	return result;
}

// conjunction: inversion (('and' inversion))+ | inversion
static expr_type rule_conjunction(Parser* p)
{
	RULE_HEAD();

	expr_type result = NULL;
	if (CrGen_IsMemoized(p, type_conjunction, &result))
	{
		p->level--;
		return result;
	}

	int mark = p->mark;
	if (p->mark == p->fill && CrGen_FillToken(p) < 0)
	{
		p->error_indicator = 1;
		p->level--;
		return result;
	}

	int start_lineno = p->tokens[mark]->lineno;
	int start_col_offset = p->tokens[mark]->col_offset;
	{ // inversion
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("conjunction", "inversion");
		expr_type inversion_var;
		if (
			(inversion_var = rule_inversion(p))
			)
		{
			CrParser_PrintSuccess("conjunction", "inversion");
			result = inversion_var;
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("conjunction", "inversion");
	}
	result = NULL;
done:
	CrGen_Memo_Insert(p, mark, type_conjunction, result);
	p->level--;
	return result;
}

// inversion: 'not' inversion | comparison
static expr_type rule_inversion(Parser* p)
{
	RULE_HEAD();

	expr_type result = NULL;
	if (CrGen_IsMemoized(p, type_inversion, &result))
	{
		p->level--;
		return result;
	}

	int mark = p->mark;
	if (p->mark == p->fill && CrGen_FillToken(p) < 0)
	{
		p->error_indicator = 1;
		p->level--;
		return result;
	}

	int start_lineno = p->tokens[mark]->lineno;
	int start_col_offset = p->tokens[mark]->col_offset;
	{ // comparison
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("inversion", "comparison");
		expr_type comparison_var;
		if (
			(comparison_var = rule_comparison(p)) // comparison
			)
		{
			CrParser_PrintSuccess("inversion", "comparison");
			result = comparison_var;
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("inversion", "comparison");
	}
	result = NULL;
done:
	CrGen_Memo_Insert(p, mark, type_inversion, result);
	p->level--;
	return result;
}

// comparison: bitwise_or compare_op_bitwise_or_pair+ | bitwise_or
static expr_type rule_comparison(Parser* p)
{
	RULE_HEAD();

	expr_type result = NULL;
	int mark = p->mark;
	if (p->mark == p->fill && CrGen_FillToken(p) < 0)
	{
		p->error_indicator = 1;
		p->level--;
		return result;
	}

	int start_lineno = p->tokens[mark]->lineno;
	int start_col_offset = p->tokens[mark]->col_offset;
	{ // bitwise_or
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("comparison", "bitwise_or");
		expr_type bitwise_or_var;
		if (
			(bitwise_or_var = rule_bitwise_or(p)) // bitwise_or
			)
		{
			CrParser_PrintSuccess("comparison", "bitwise_or");
			result = bitwise_or_var;
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("comparison", "bitwise_or");
	}
	result = NULL;
done:
	p->level--;
	return result;
}

// Left-recursive
// bitwise_or: bitwise_or '|' bitwise_xor | bitwise_xor
static expr_type raw_bitwise_or(Parser* p)
{
	RULE_HEAD();

	expr_type result = NULL;
	int mark = p->mark;
	if (p->mark == p->fill && CrGen_FillToken(p) < 0)
	{
		p->error_indicator = 1;
		p->level--;
		return result;
	}

	int start_lineno = p->tokens[mark]->lineno;
	int start_col_offset = p->tokens[mark]->col_offset;
	{ // bitwise_or '|' bitwise_xor
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("bitwise_xor", "bitwise_or '|' bitwise_xor");
		Token* _literal;
		expr_type a;
		expr_type b;
		if (
			(a = rule_bitwise_or(p)) // bitwise_or
			&&
			(_literal = CrGen_ExpectToken(p, TOK_VBAR)) // token = '|'
			&&
			(b = rule_bitwise_xor(p)) // bitwise_xor
			)
		{
			CrParser_PrintSuccess("bitwise_xor", "bitwise_or '|' bitwise_xor");
			Token* token = CrGen_GetLastNonWhitespaceToken(p);
			if (token == NULL)
			{
				p->level--;
				return NULL;
			}
			int end_lineno = token->end_lineno;
			int end_col_offset = token->end_col_offset;
			result = CrAST_BinOp(a, operator_type::BitOr, b, EXTRA);
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("bitwise_xor", "bitwise_or '|' bitwise_xor");
	}
	{ // bitwise_xor
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("bitwise_or", "bitwise_xor");
		expr_type bitwise_xor_var;
		if (
			(bitwise_xor_var = rule_bitwise_xor(p)) // bitwise_xor
			)
		{
			CrParser_PrintSuccess("bitwise_or", "bitwise_xor");
			result = bitwise_xor_var;
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("bitwise_or", "bitwise_xor");
	}
	result = NULL;
done:
	p->level--;
	return result;
}
static expr_type rule_bitwise_or(Parser* p)
{
	D(p->level++);
	expr_type result = NULL;
	if (CrGen_IsMemoized(p, type_bitwise_or, &result))
	{
		p->level--;
		return result;
	}
	int mark = p->mark;
	int resmark = p->mark;
	while (1)
	{
		int tmpvar = CrGen_Memo_Update(p, mark, type_bitwise_or, result);
		if (tmpvar)
		{
			p->level--;
			return result;
		}
		p->mark = mark;
		void* raw = raw_bitwise_or(p);
		if (p->error_indicator)
			return NULL;
		if (raw == NULL || p->mark <= resmark)
			break;
		resmark = p->mark;
		result = (expr_type)raw;
	}
	p->mark = resmark;
	p->level--;
	return result;
}

// Left-recursive
// bitwise_xor: bitwise_xor '^' bitwise_and | bitwise_and
static expr_type raw_bitwise_xor(Parser* p)
{
	RULE_HEAD();

	expr_type result = NULL;
	int mark = p->mark;
	if (p->mark == p->fill && CrGen_FillToken(p) < 0)
	{
		p->error_indicator = 1;
		p->level--;
		return result;
	}

	int start_lineno = p->tokens[mark]->lineno;
	int start_col_offset = p->tokens[mark]->col_offset;
	{ // bitwise_xor '^' bitwise_and
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("bitwise_xor", "bitwise_xor '^' bitwise_and");
		Token* _literal;
		expr_type a;
		expr_type b;
		if (
			(a = rule_bitwise_xor(p)) // bitwise_xor
			&&
			(_literal = CrGen_ExpectToken(p, TOK_CIRCUMFLEX)) // token = '^'
			&&
			(b = rule_bitwise_and(p)) // bitwise_and
			)
		{
			CrParser_PrintSuccess("bitwise_xor", "bitwise_xor '^' bitwise_and");
			Token* token = CrGen_GetLastNonWhitespaceToken(p);
			if (token == NULL)
			{
				p->level--;
				return NULL;
			}
			int end_lineno = token->end_lineno;
			int end_col_offset = token->end_col_offset;
			result = CrAST_BinOp(a, operator_type::BitXor, b, EXTRA);
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("bitwise_xor", "bitwise_xor '^' bitwise_and");
	}
	{ // bitwise_and
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("bitwise_xor", "bitwise_and");
		expr_type bitwise_and_var;
		if (
			(bitwise_and_var = rule_bitwise_and(p)) // bitwise_and
			)
		{
			CrParser_PrintSuccess("bitwise_xor", "bitwise_and");
			result = bitwise_and_var;
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("bitwise_xor", "bitwise_and");
	}
	result = NULL;
done:
	p->level--;
	return result;
}
static expr_type rule_bitwise_xor(Parser* p)
{
	D(p->level++);
	expr_type result = NULL;
	if (CrGen_IsMemoized(p, type_bitwise_xor, &result))
	{
		p->level--;
		return result;
	}
	int mark = p->mark;
	int resmark = p->mark;
	while (1)
	{
		int tmpvar = CrGen_Memo_Update(p, mark, type_bitwise_xor, result);
		if (tmpvar)
		{
			p->level--;
			return result;
		}
		p->mark = mark;
		void* raw = raw_bitwise_xor(p);
		if (p->error_indicator)
			return NULL;
		if (raw == NULL || p->mark <= resmark)
			break;
		resmark = p->mark;
		result = (expr_type)raw;
	}
	p->mark = resmark;
	p->level--;
	return result;
}

// Left-recursive
// bitwise_and: bitwise_and '&' shift_expr | shift_expr
static expr_type raw_bitwise_and(Parser* p)
{
	RULE_HEAD();

	expr_type result = NULL;
	int mark = p->mark;
	if (p->mark == p->fill && CrGen_FillToken(p) < 0)
	{
		p->error_indicator = 1;
		p->level--;
		return result;
	}

	int start_lineno = p->tokens[mark]->lineno;
	int start_col_offset = p->tokens[mark]->col_offset;
	{ // bitwise_and '&' shift_expr
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("bitwise_and", "bitwise_and '&' shift_expr");
		Token* _literal;
		expr_type a;
		expr_type b;
		if (
			(a = rule_bitwise_and(p)) // bitwise_and
			&&
			(_literal = CrGen_ExpectToken(p, TOK_AMPER)) // token = '&'
			&&
			(b = rule_shift_expr(p)) // shift_expr
			)
		{
			CrParser_PrintSuccess("bitwise_and", "bitwise_and '&' shift_expr");
			Token* token = CrGen_GetLastNonWhitespaceToken(p);
			if (token == NULL)
			{
				p->level--;
				return NULL;
			}
			int end_lineno = token->end_lineno;
			int end_col_offset = token->end_col_offset;
			result = CrAST_BinOp(a, operator_type::BitAnd, b, EXTRA);
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("bitwise_and", "bitwise_and '&' shift_expr");
	}
	{ // shift_expr
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("bitwise_and", "shift_expr");
		expr_type shift_expr_var;
		if (
			(shift_expr_var = rule_shift_expr(p)) // shift_expr
			)
		{
			CrParser_PrintSuccess("bitwise_and", "shift_expr");
			result = shift_expr_var;
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("bitwise_and", "shift_expr");
	}
	result = NULL;
done:
	p->level--;
	return result;
}
static expr_type rule_bitwise_and(Parser* p)
{
	D(p->level++);
	expr_type result = NULL;
	if (CrGen_IsMemoized(p, type_bitwise_and, &result))
	{
		p->level--;
		return result;
	}
	int mark = p->mark;
	int resmark = p->mark;
	while (1)
	{
		int tmpvar = CrGen_Memo_Update(p, mark, type_bitwise_and, result);
		if (tmpvar)
		{
			p->level--;
			return result;
		}
		p->mark = mark;
		void* raw = raw_bitwise_and(p);
		if (p->error_indicator)
			return NULL;
		if (raw == NULL || p->mark <= resmark)
			break;
		resmark = p->mark;
		result = (expr_type)raw;
	}
	p->mark = resmark;
	p->level--;
	return result;
}

// Left-recursive
// shift_expr: shift_expr '<<' sum | shift_expr '>>' sum | sum
static expr_type raw_shift_expr(Parser* p)
{
	RULE_HEAD();

	expr_type result = NULL;
	int mark = p->mark;
	if (p->mark == p->fill && CrGen_FillToken(p) < 0)
	{
		p->error_indicator = 1;
		p->level--;
		return result;
	}

	int start_lineno = p->tokens[mark]->lineno;
	int start_col_offset = p->tokens[mark]->col_offset;
	{ // shift_expr '<<' sum
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("shift_expr", "shift_expr '<<' sum");
		Token* _literal;
		expr_type a;
		expr_type b;
		if (
			(a = rule_shift_expr(p)) // shift_expr
			&&
			(_literal = CrGen_ExpectToken(p, TOK_LEFTSHIFT)) // token = '<<'
			&&
			(b = rule_sum(p)) // sum
			)
		{
			CrParser_PrintSuccess("shift_expr", "shift_expr '<<' sum");
			Token* token = CrGen_GetLastNonWhitespaceToken(p);
			if (token == NULL)
			{
				p->level--;
				return NULL;
			}
			int end_lineno = token->end_lineno;
			int end_col_offset = token->end_col_offset;
			result = CrAST_BinOp(a, operator_type::LShift, b, EXTRA);
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("shift_expr", "shift_expr '<<' sum");
	}
	{ // shift_expr '>>' sum
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("shift_expr", "shift_expr '>>' sum");
		Token* _literal;
		expr_type a;
		expr_type b;
		if (
			(a = rule_shift_expr(p)) // shift_expr
			&&
			(_literal = CrGen_ExpectToken(p, TOK_RIGHTSHIFT)) // token = '>>'
			&&
			(b = rule_sum(p)) // sum
			)
		{
			CrParser_PrintSuccess("shift_expr", "shift_expr '>>' sum");
			Token* token = CrGen_GetLastNonWhitespaceToken(p);
			if (token == NULL)
			{
				p->level--;
				return NULL;
			}
			int end_lineno = token->end_lineno;
			int end_col_offset = token->end_col_offset;
			result = CrAST_BinOp(a, operator_type::RShift, b, EXTRA);
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("shift_expr", "shift_expr '>>' sum");
	}
	{ // sum
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("shift_expr", "sum");
		expr_type sum_var;
		if (
			(sum_var = rule_sum(p)) // sum
			)
		{
			CrParser_PrintSuccess("shift_expr", "sum");
			result = sum_var;
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("shift_expr", "sum");
	}
	result = NULL;
done:
	p->level--;
	return result;
}
static expr_type rule_shift_expr(Parser* p)
{
	D(p->level++);
	expr_type result = NULL;
	if (CrGen_IsMemoized(p, type_shift_expr, &result))
	{
		p->level--;
		return result;
	}
	int mark = p->mark;
	int resmark = p->mark;
	while (1)
	{
		int tmpvar = CrGen_Memo_Update(p, mark, type_shift_expr, result);
		if (tmpvar)
		{
			p->level--;
			return result;
		}
		p->mark = mark;
		void* raw = raw_shift_expr(p);
		if (p->error_indicator)
			return NULL;
		if (raw == NULL || p->mark <= resmark)
			break;
		resmark = p->mark;
		result = (expr_type)raw;
	}
	p->mark = resmark;
	p->level--;
	return result;
}

// Left-recursive
// sum: sum '+' term | sum '-' term | term
static expr_type raw_sum(Parser* p)
{
	RULE_HEAD();

	expr_type result = NULL;
	int mark = p->mark;
	if (p->mark == p->fill && CrGen_FillToken(p) < 0)
	{
		p->error_indicator = 1;
		p->level--;
		return result;
	}

	int start_lineno = p->tokens[mark]->lineno;
	int start_col_offset = p->tokens[mark]->col_offset;
	{ // sum '+' term 
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("sum", "sum '+' term");
		Token* _literal;
		expr_type a;
		expr_type b;
		if (
			(a = rule_sum(p)) // sum
			&&
			(_literal = CrGen_ExpectToken(p, TOK_ADD)) // token = '+'
			&&
			(b = rule_term(p)) // term
			)
		{
			CrParser_PrintSuccess("sum", "sum '+' term");
			Token* token = CrGen_GetLastNonWhitespaceToken(p);
			if (token == NULL)
			{
				p->level--;
				return NULL;
			}
			int end_lineno = token->end_lineno;
			int end_col_offset = token->end_col_offset;
			result = CrAST_BinOp(a, operator_type::Add, b, EXTRA);
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("sum", "sum '+' term");
	}
	{ // sum '-' term 
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("sum", "sum '-' term");
		Token* _literal;
		expr_type a;
		expr_type b;
		if (
			(a = rule_sum(p)) // sum
			&&
			(_literal = CrGen_ExpectToken(p, TOK_MINUS)) // token = '-'
			&&
			(b = rule_term(p)) // term
			)
		{
			CrParser_PrintSuccess("sum", "sum '-' term");
			Token* token = CrGen_GetLastNonWhitespaceToken(p);
			if (token == NULL)
			{
				p->level--;
				return NULL;
			}
			int end_lineno = token->end_lineno;
			int end_col_offset = token->end_col_offset;
			result = CrAST_BinOp(a, operator_type::Sub, b, EXTRA);
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("sum", "sum '-' term");
	}
	{ // term
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("sum", "term");
		expr_type term_var;
		if (
			(term_var = rule_term(p)) // term
			)
		{
			CrParser_PrintSuccess("sum", "term");
			result = term_var;
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("sum", "term");
	}
	result = NULL;
done:
	p->level--;
	return result;
}
static expr_type rule_sum(Parser* p)
{
	D(p->level++);
	expr_type result = NULL;
	if (CrGen_IsMemoized(p, type_sum, &result))
	{
		p->level--;
		return result;
	}
	int mark = p->mark;
	int resmark = p->mark;
	while (1)
	{
		int tmpvar = CrGen_Memo_Update(p, mark, type_sum, result);
		if (tmpvar)
		{
			p->level--;
			return result;
		}
		p->mark = mark;
		void* raw = raw_sum(p);
		if (p->error_indicator)
			return NULL;
		if (raw == NULL || p->mark <= resmark)
			break;
		resmark = p->mark;
		result = (expr_type)raw;
	}
	p->mark = resmark;
	p->level--;
	return result;
}
// Left-recursive
// term:
//     | term '*' factor
//     | term '/' factor
//     | term '//' factor
//     | term '%' factor
//     | term '@' factor
//     | factor
static expr_type raw_term(Parser* p)
{
	RULE_HEAD();

	expr_type result = NULL;
	int mark = p->mark;
	if (p->mark == p->fill && CrGen_FillToken(p) < 0)
	{
		p->error_indicator = 1;
		p->level--;
		return result;
	}

	int start_lineno = p->tokens[mark]->lineno;
	int start_col_offset = p->tokens[mark]->col_offset;
	{ // term '*' factor 
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("term", "term '*' factor");
		Token* _literal;
		expr_type a;
		expr_type b;
		if (
			(a = rule_term(p)) // term
			&&
			(_literal = CrGen_ExpectToken(p, TOK_STAR)) // token = '*'
			&&
			(b = rule_factor(p)) // factor
			)
		{
			CrParser_PrintSuccess("term", "term '*' factor");
			Token* token = CrGen_GetLastNonWhitespaceToken(p);
			if (token == NULL)
			{
				p->level--;
				return NULL;
			}
			int end_lineno = token->end_lineno;
			int end_col_offset = token->end_col_offset;
			result = CrAST_BinOp(a, operator_type::Mult, b, EXTRA);
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("term", "term '*' factor");
	}
	{ // term '/' factor 
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("term", "term '/' factor");
		Token* _literal;
		expr_type a;
		expr_type b;
		if (
			(a = rule_term(p)) // term
			&&
			(_literal = CrGen_ExpectToken(p, TOK_FSLASH)) // token = '/'
			&&
			(b = rule_factor(p)) // factor
			)
		{
			CrParser_PrintSuccess("term", "term '/' factor");
			Token* token = CrGen_GetLastNonWhitespaceToken(p);
			if (token == NULL)
			{
				p->level--;
				return NULL;
			}
			int end_lineno = token->end_lineno;
			int end_col_offset = token->end_col_offset;
			result = CrAST_BinOp(a, operator_type::Div, b, EXTRA);
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("term", "term '/' factor");
	}
	{ // term '//' factor 
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("term", "term '//' factor");
		Token* _literal;
		expr_type a;
		expr_type b;
		if (
			(a = rule_term(p)) // term
			&&
			(_literal = CrGen_ExpectToken(p, TOK_DOUBLEFSLASH)) // token = '//'
			&&
			(b = rule_factor(p)) // factor
			)
		{
			CrParser_PrintSuccess("term", "term '//' factor");
			Token* token = CrGen_GetLastNonWhitespaceToken(p);
			if (token == NULL)
			{
				p->level--;
				return NULL;
			}
			int end_lineno = token->end_lineno;
			int end_col_offset = token->end_col_offset;
			result = CrAST_BinOp(a, operator_type::FloorDiv, b, EXTRA);
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("term", "term '//' factor");
	}
	{ // term '%' factor 
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("term", "term '%' factor");
		Token* _literal;
		expr_type a;
		expr_type b;
		if (
			(a = rule_term(p)) // term
			&&
			(_literal = CrGen_ExpectToken(p, TOK_PERCENT)) // token = '%'
			&&
			(b = rule_factor(p)) // factor
			)
		{
			CrParser_PrintSuccess("term", "term '%' factor");
			Token* token = CrGen_GetLastNonWhitespaceToken(p);
			if (token == NULL)
			{
				p->level--;
				return NULL;
			}
			int end_lineno = token->end_lineno;
			int end_col_offset = token->end_col_offset;
			result = CrAST_BinOp(a, operator_type::Mod, b, EXTRA);
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("term", "term '%' factor");
	}
	{ // term '@' factor 
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("term", "term '@' factor");
		Token* _literal;
		expr_type a;
		expr_type b;
		if (
			(a = rule_term(p)) // term
			&&
			(_literal = CrGen_ExpectToken(p, TOK_AT)) // token = '@'
			&&
			(b = rule_factor(p)) // factor
			)
		{
			CrParser_PrintSuccess("term", "term '@' factor");
			Token* token = CrGen_GetLastNonWhitespaceToken(p);
			if (token == NULL)
			{
				p->level--;
				return NULL;
			}
			int end_lineno = token->end_lineno;
			int end_col_offset = token->end_col_offset;
			result = CrAST_BinOp(a, operator_type::MatMult, b, EXTRA);
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("term", "term '@' factor");
	}
	{ // factor
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("term", "term");
		expr_type factor_var;
		if (
			(factor_var = rule_factor(p)) // factor
			)
		{
			CrParser_PrintSuccess("term", "factor");
			result = factor_var;
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("term", "factor");
	}
	result = NULL;
done:
	p->level--;
	return result;
}
static expr_type rule_term(Parser* p)
{
	D(p->level++);
	expr_type result = NULL;
	if (CrGen_IsMemoized(p, type_term, &result))
	{
		p->level--;
		return result;
	}
	int mark = p->mark;
	int resmark = p->mark;
	while (1)
	{
		int tmpvar = CrGen_Memo_Update(p, mark, type_term, result);
		if (tmpvar)
		{
			p->level--;
			return result;
		}
		p->mark = mark;
		void* raw = raw_term(p);
		if (p->error_indicator)
			return NULL;
		if (raw == NULL || p->mark <= resmark)
			break;
		resmark = p->mark;
		result = (expr_type)raw;
	}
	p->mark = resmark;
	p->level--;
	return result;
}

// factor: '+' factor | '-' factor | '~' factor | power
expr_type rule_factor(Parser* p)
{
	RULE_HEAD();

	expr_type result = NULL;
	if (CrGen_IsMemoized(p, type_factor, &result))
	{
		p->level--;
		return result;
	}

	int mark = p->mark;
	if (p->mark == p->fill && CrGen_FillToken(p) < 0)
	{
		p->error_indicator = 1;
		p->level--;
		return result;
	}

	int start_lineno = p->tokens[mark]->lineno;
	int start_col_offset = p->tokens[mark]->col_offset;
	{ // '+' factor
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("factor", "'+' factor");
		Token* _literal;
		expr_type a;
		if (
			(_literal = CrGen_ExpectToken(p, TOK_ADD)) // token = '+'
			&&
			(a = rule_factor(p)) // factor
			)
		{
			CrParser_PrintSuccess("factor", "'+' factor");
			Token* token = CrGen_GetLastNonWhitespaceToken(p);
			if (token == NULL)
			{
				p->level--;
				return NULL;
			}
			int end_lineno = token->end_lineno;
			int end_col_offset = token->end_col_offset;
			result = CrAST_UnaryOp(unaryop_type::UAdd, a, EXTRA);
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("factor", "'+' factor");
	}
	{ // '-' factor
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("factor", "'-' factor");
		Token* _literal;
		expr_type a;
		if (
			(_literal = CrGen_ExpectToken(p, TOK_MINUS)) // token = '-'
			&&
			(a = rule_factor(p)) // factor
			)
		{
			CrParser_PrintSuccess("factor", "'-' factor");
			Token* token = CrGen_GetLastNonWhitespaceToken(p);
			if (token == NULL)
			{
				p->level--;
				return NULL;
			}
			int end_lineno = token->end_lineno;
			int end_col_offset = token->end_col_offset;
			result = CrAST_UnaryOp(unaryop_type::USub, a, EXTRA);
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("factor", "'-' factor");
	}
	{ // '~' factor
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("factor", "'~' factor");
		Token* _literal;
		expr_type a;
		if (
			(_literal = CrGen_ExpectToken(p, TOK_TILDE)) // token = '~'
			&&
			(a = rule_factor(p)) // factor
			)
		{
			CrParser_PrintSuccess("factor", "'~' factor");
			Token* token = CrGen_GetLastNonWhitespaceToken(p);
			if (token == NULL)
			{
				p->level--;
				return NULL;
			}
			int end_lineno = token->end_lineno;
			int end_col_offset = token->end_col_offset;
			result = CrAST_UnaryOp(unaryop_type::Invert, a, EXTRA);
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("factor", "'~' factor");
	}
	{ // power
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("factor", "power");
		expr_type power_var;
		if (
			(power_var = rule_power(p)) // power
			)
		{
			CrParser_PrintSuccess("factor", "power");
			result = power_var;
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("factor", "power");
	}
	result = NULL;
done:
	CrGen_Memo_Insert(p, mark, type_factor, result);
	p->level--;
	return result;
}

// power: await_primary '**' factor | await_primary
static expr_type rule_power(Parser* p)
{
	RULE_HEAD();

	expr_type result = NULL;
	int mark = p->mark;
	if (p->mark == p->fill && CrGen_FillToken(p) < 0)
	{
		p->error_indicator = 1;
		p->level--;
		return result;
	}

	int start_lineno = p->tokens[mark]->lineno;
	int start_col_offset = p->tokens[mark]->col_offset;
	{ // await_primary '**' factor 
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("power", "await_primary '**' factor");
		Token* _literal;
		expr_type a;
		expr_type b;
		if (
			(a = rule_await_primary(p)) // await_primary
			&&
			(_literal = CrGen_ExpectToken(p, TOK_DOUBLESTAR)) // token = '**'
			&&
			(b = rule_factor(p)) // factor
			)
		{
			CrParser_PrintSuccess("power", "await_primary '**' factor");
			Token* token = CrGen_GetLastNonWhitespaceToken(p);
			if (token == NULL)
			{
				p->level--;
				return NULL;
			}
			int end_lineno = token->end_lineno;
			int end_col_offset = token->end_col_offset;
			result = CrAST_BinOp(a, operator_type::Pow, b, EXTRA);
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("power", "await_primary '**' factor");
	}
	{ // await_primary
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("power", "await_primary");
		expr_type await_primary_var;
		if (
			(await_primary_var = rule_await_primary(p)) // await_primary
			)
		{
			CrParser_PrintSuccess("power", "await_primary");
			result = await_primary_var;
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("power", "await_primary");
	}
	result = NULL;
done:
	p->level--;
	return result;
}

// await_primary: AWAIT primary | primary
static expr_type rule_await_primary(Parser* p)
{
	RULE_HEAD();

	expr_type result = NULL;
	if (CrGen_IsMemoized(p, type_await_primary, &result))
	{
		p->level--;
		return result;
	}

	int mark = p->mark;
	if (p->mark == p->fill && CrGen_FillToken(p) < 0)
	{
		p->error_indicator = 1;
		p->level--;
		return result;
	}

	int start_lineno = p->tokens[mark]->lineno;
	int start_col_offset = p->tokens[mark]->col_offset;
	{ // primary
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("await_primary", "primary");
		expr_type primary_var;
		if (
			(primary_var = rule_primary(p)) // primary
			)
		{
			CrParser_PrintSuccess("await_primary", "primary");
			result = primary_var;
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("await_primary", "primary");
	}
	result = NULL;
done:
	CrGen_Memo_Insert(p, mark, type_await_primary, result);
	p->level--;
	return result;
}

// Left-recursive
// primary:
//     | invalid_primary
//     | primary '.' NAME
//     | primary genexp
//     | primary '(' arguments? ')'
//     | primary '[' slices ']'
//     | atom
static expr_type raw_primary(Parser* p)
{
	RULE_HEAD();

	expr_type result = NULL;
	int mark = p->mark;
	if (p->mark == p->fill && CrGen_FillToken(p) < 0)
	{
		p->error_indicator = 1;
		p->level--;
		return result;
	}

	int start_lineno = p->tokens[mark]->lineno;
	int start_col_offset = p->tokens[mark]->col_offset;
	{ // atom
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("primary", "atom");
		expr_type atom_var;
		if (
			(atom_var = rule_atom(p)) // atom
			)
		{
			CrParser_PrintSuccess("primary", "atom");
			result = atom_var;
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("primary", "atom");
	}
	result = NULL;
done:
	p->level--;
	return result;
}
static expr_type rule_primary(Parser* p)
{
	D(p->level++);
	expr_type result = NULL;
	if (CrGen_IsMemoized(p, type_primary, &result))
	{
		p->level--;
		return result;
	}
	int mark = p->mark;
	int resmark = p->mark;
	while (1)
	{
		int tmpvar = CrGen_Memo_Update(p, mark, type_primary, result);
		if (tmpvar)
		{
			p->level--;
			return result;
		}
		p->mark = mark;
		void* raw = raw_primary(p);
		if (p->error_indicator)
			return NULL;
		if (raw == NULL || p->mark <= resmark)
			break;
		resmark = p->mark;
		result = (expr_type)raw;
	}
	p->mark = resmark;
	p->level--;
	return result;
}

// atom:
//     | NAME
//     | 'True'
//     | 'False'
//     | 'Null'
//     | &STRING strings
//     | NUMBER
//     | &'(' (tuple | group | genexp)
//     | &'[' (list | listcomp)
//     | &'{' (dict | set | dictcomp | setcomp)
//     | '...'
static expr_type rule_atom(Parser* p)
{
	RULE_HEAD();

	expr_type result = NULL;
	int mark = p->mark;
	if (p->mark == p->fill && CrGen_FillToken(p) < 0)
	{
		p->error_indicator = 1;
		p->level--;
		return result;
	}

	int start_lineno = p->tokens[mark]->lineno;
	int start_col_offset = p->tokens[mark]->col_offset;
	{ // NAME
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("atom", "NAME");
		expr_type name_var;
		if (
			(name_var = CrGen_NameToken(p)) // NAME
			)
		{
			CrParser_PrintSuccess("atom", "NAME");
			result = name_var;
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("atom", "NAME");
	}
	{ // 'True'
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("atom", "'True'");
		Token* keyword;
		if (
			(keyword = CrGen_ExpectToken(p, TOK_TRUE)) // 'True'
			)
		{
			CrParser_PrintSuccess("atom", "'True'");
			Token* token = CrGen_GetLastNonWhitespaceToken(p);
			if (token == NULL)
			{
				p->level--;
				return NULL;
			}
			int end_lineno = token->end_lineno;
			int end_col_offset = token->end_col_offset;
			result = CrAST_Constant((constant)Cr_True, NULL, EXTRA);
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("atom", "'True'");
	}
	{ // 'False'
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("atom", "'False'");
		Token* keyword;
		if (
			(keyword = CrGen_ExpectToken(p, TOK_FALSE)) // 'False'
			)
		{
			CrParser_PrintSuccess("atom", "'False'");
			Token* token = CrGen_GetLastNonWhitespaceToken(p);
			if (token == NULL)
			{
				p->level--;
				return NULL;
			}
			int end_lineno = token->end_lineno;
			int end_col_offset = token->end_col_offset;
			result = CrAST_Constant((constant)Cr_False, NULL, EXTRA);
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("atom", "'False'");
	}
	{ // 'Null'
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("atom", "'Null'");
		Token* keyword;
		if (
			(keyword = CrGen_ExpectToken(p, TOK_NULL)) // 'Null'
			)
		{
			CrParser_PrintSuccess("atom", "'Null'");
			Token* token = CrGen_GetLastNonWhitespaceToken(p);
			if (token == NULL)
			{
				p->level--;
				return NULL;
			}
			int end_lineno = token->end_lineno;
			int end_col_offset = token->end_col_offset;
			result = CrAST_Constant((constant)Cr_Null, NULL, EXTRA);
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("atom", "'Null'");
	}
	{ // &STRING strings
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("atom", "&STRING strings");
		expr_type strings_var;
		if (
			CrGen_Lookahead(1, CrGen_StringToken, p)
			&&
			(strings_var = rule_strings(p)) // strings
			)
		{
			CrParser_PrintSuccess("atom", "&STRING strings");
			result = strings_var;
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("atom", "&STRING strings");
	}
	{ // NUMBER
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("atom", "NUMBER");
		expr_type number_var;
		if (
			(number_var = CrGen_NumberToken(p)) // NUMBER
			)
		{
			CrParser_PrintSuccess("atom", "NUMBER");
			result = number_var;
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("atom", "NUMBER");
	}
	result = NULL;
done:
	p->level--;
	return result;
}

// strings: STRING+
expr_type rule_strings(Parser* p)
{
	RULE_HEAD();

	expr_type result = NULL;
	if (CrGen_IsMemoized(p, type_strings, &result))
	{
		p->level--;
		return result;
	}

	int mark = p->mark;
	{ // STRING+
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("strings", "STRING+");
		ast_seq* a;
		if (
			(a = rule_loop_string(p)) // STRING+
			)
		{
			CrParser_PrintSuccess("strings", "STRING+");
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("strings", "STRING+");
	}
	result = NULL;
done:
	CrGen_Memo_Insert(p, mark, type_strings, result);
	p->level--;
	return result;
}

// star_targets: star_target !',' | star_target ((',' star_target))* ','?
expr_type rule_star_targets(Parser* p)
{
	RULE_HEAD();

	expr_type result = NULL;
	int mark = p->mark;
	if (p->mark == p->fill && CrGen_FillToken(p) < 0)
	{
		p->error_indicator = 1;
		p->level--;
		return result;
	}

	int start_lineno = p->tokens[mark]->lineno;
	int start_col_offset = p->tokens[mark]->col_offset;
	{ // star_target !','
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("star_targets", "star_target !','");
		expr_type target;
		if (
			(target = rule_star_target(p)) // star_target
			&&
			(CrGen_LookaheadWithInt(0, CrGen_ExpectToken, p, TOK_COMMA)) // token = ','
			)
		{
			CrParser_PrintSuccess("star_targets", "star_target !','");
			result = target;
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("star_targets", "star_target !','");
	}
	result = NULL;
done:
	p->level--;
	return result;
}

// star_target: '*' (!'*' star_target) | target_with_star_atom
expr_type rule_star_target(Parser* p)
{
	RULE_HEAD();

	expr_type result = NULL;
	if (CrGen_IsMemoized(p, type_star_target, &result))
	{
		p->level--;
		return result;
	}
	int mark = p->mark;
	if (p->mark == p->fill && CrGen_FillToken(p) < 0)
	{
		p->error_indicator = 1;
		p->level--;
		return result;
	}

	int start_lineno = p->tokens[mark]->lineno;
	int start_col_offset = p->tokens[mark]->col_offset;
	{ // target_with_star_atom
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("star_target", "target_with_star_atom");
		expr_type target;
		if (
			(target = rule_target_with_star_atom(p)) // target_with_star_atom
			)
		{
			CrParser_PrintSuccess("star_target", "target_with_star_atom");
			result = target;
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("star_target", "target_with_star_atom");
	}
	result = NULL;
done:
	CrGen_Memo_Insert(p, mark, type_star_target, result);
	p->level--;
	return result;
}

// target_with_star_atom:
//     | t_primary '.' NAME !t_lookahead
//     | t_primary '[' slices ']' !t_lookahead
//     | star_atom
expr_type rule_target_with_star_atom(Parser* p)
{
	RULE_HEAD();

	expr_type result = NULL;
	if (CrGen_IsMemoized(p, type_target_with_star_atom, &result))
	{
		p->level--;
		return result;
	}
	int mark = p->mark;
	if (p->mark == p->fill && CrGen_FillToken(p) < 0)
	{
		p->error_indicator = 1;
		p->level--;
		return result;
	}

	int start_lineno = p->tokens[mark]->lineno;
	int start_col_offset = p->tokens[mark]->col_offset;
	{ // star_atom
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("target_with_star_atom", "star_atom");
		expr_type target;
		if (
			(target = rule_star_atom(p)) // star_atom
			)
		{
			CrParser_PrintSuccess("target_with_star_atom", "star_atom");
			result = target;
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("target_with_star_atom", "star_atom");
	}
	result = NULL;
done:
	CrGen_Memo_Insert(p, mark, type_target_with_star_atom, result);
	p->level--;
	return result;
}

// star_atom:
//     | NAME
//     | '(' target_with_star_atom ')'
//     | '(' star_targets_tuple_seq? ')'
//     | '[' star_targets_list_seq? ']'
expr_type rule_star_atom(Parser* p)
{
	RULE_HEAD();

	expr_type result = NULL;
	int mark = p->mark;
	if (p->mark == p->fill && CrGen_FillToken(p) < 0)
	{
		p->error_indicator = 1;
		p->level--;
		return result;
	}

	int start_lineno = p->tokens[mark]->lineno;
	int start_col_offset = p->tokens[mark]->col_offset;
	{ // NAME
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("star_atom", "NAME");
		expr_type name;
		if (
			(name = CrGen_NameToken(p)) // NAME
			)
		{
			CrParser_PrintSuccess("star_atom", "NAME");
			result = name;
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("star_atom", "NAME");
	}
	result = NULL;
done:
	p->level--;
	return result;
}

// yield_expr: 'yield' 'from' expression | 'yield' star_expressions?
expr_type rule_yield_expr(Parser* p)
{
	RULE_HEAD();

	expr_type result = NULL;
	int mark = p->mark;
	if (p->mark == p->fill && CrGen_FillToken(p) < 0)
	{
		p->error_indicator = 1;
		p->level--;
		return result;
	}

	int start_lineno = p->tokens[mark]->lineno;
	int start_col_offset = p->tokens[mark]->col_offset;
	{ // 'yield' 'from' expression
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("yield_expr", "'yield' 'from' expression");
		Token* literal;
		Token* literal_1;
		expr_type expr;
		if (
			(literal = CrGen_ExpectToken(p, TOK_YIELD)) // token = 'yield'
			&&
			(literal_1 = CrGen_ExpectToken(p, TOK_FROM)) // token = 'from'
			&&
			(expr = rule_expression(p)) // expression
			)
		{
			CrParser_PrintSuccess("yield_expr", "'yield' 'from' expression");
			Token* token = CrGen_GetLastNonWhitespaceToken(p);
			if (token == NULL)
			{
				p->level--;
				return NULL;
			}
			int end_lineno = token->end_lineno;
			int end_col_offset = token->end_col_offset;
			result = CrAST_YieldFrom(expr, EXTRA);
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("yield_expr", "'yield' 'from' expression");
	}
	{ // 'yield' star_expressions?
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("yield_expr", "'yield' star_expressions?");
		Token* literal;
		expr_type expr;
		if (
			(literal = CrGen_ExpectToken(p, TOK_YIELD)) // token = 'yield'
			&&
			(expr = rule_star_expressions(p), !p->error_indicator) // expression
			)
		{
			CrParser_PrintSuccess("yield_expr", "'yield' star_expressions?");
			Token* token = CrGen_GetLastNonWhitespaceToken(p);
			if (token == NULL)
			{
				p->level--;
				return NULL;
			}
			int end_lineno = token->end_lineno;
			int end_col_offset = token->end_col_offset;
			result = CrAST_Yield(expr, EXTRA);
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("yield_expr", "'yield' star_expressions?");
	}
	result = NULL;
done:
	p->level--;
	return result;
}

// loop_string: STRING
ast_seq* rule_loop_string(Parser* p)
{
	RULE_HEAD();

	void* result = NULL;
	int mark = p->mark;
	int start_mark = p->mark;
	void** children = (void**)Mem_Alloc(sizeof(void*));
	if (!children)
	{
		p->error_indicator = 1;
		CrError_NoMemory();
		p->level--;
		return NULL;
	}
	size_t children_capacity = 1;
	size_t n = 0;
	{ // STRING
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("loop_string", "STRING");
		expr_type string_var;
		while (
			(string_var = (expr_type)CrGen_StringToken(p)) // STRING
			)
		{
			result = string_var;
			if (n == children_capacity)
			{
				children_capacity *= 2;
				void** new_children = (void**)Mem_Realloc(children, children_capacity * sizeof(void*));
				if (!new_children)
				{
					p->error_indicator = 1;
					CrError_NoMemory();
					p->level--;
					return NULL;
				}
				children = new_children;
			}
			children[n++] = result;
			mark = p->mark;
		}
		p->mark = mark;
		CrParser_PrintFail("loop_string", "STRING");
	}
	if (n == 0 || p->error_indicator)
	{
		Mem_Free(children);
		p->level--;
		return NULL;
	}
	ast_seq* seq = (ast_seq*)CrAST_NewGenericSeq(n, p->arena);
	if (!seq)
	{
		Mem_Free(children);
		p->error_indicator = 1;
		CrError_NoMemory();
		p->level--;
		return NULL;
	}
	for (int i = 0; i < n; i++)
		CrAST_SEQ_SET_UNTYPED(seq, i, children[i]);
	Mem_Free(children);
	CrGen_Memo_Insert(p, start_mark, type_loop_strings, seq);
	p->level--;
	return seq;
}

// looprule_14: (star_targets '=')
ast_seq* looprule_14(Parser* p)
{
	RULE_HEAD();

	void* result = NULL;
	int mark = p->mark;
	int start_mark = p->mark;
	void** children = (void**)Mem_Alloc(sizeof(void*));
	if (!children)
	{
		p->error_indicator = 1;
		CrError_NoMemory();
		p->level--;
		return NULL;
	}
	size_t children_capacity = 1;
	size_t n = 0;
	{ // (star_targets '=')
		if (p->error_indicator)
		{
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("looprule_14", "(star_targets '=')");
		void* gatherrule_219_var;
		while (
			(gatherrule_219_var = gatherrule_219(p)) // (star_targets '=')
			)
		{
			result = gatherrule_219_var;
			if (n == children_capacity)
			{
				children_capacity *= 2;
				void** new_children = (void**)Mem_Realloc(children, children_capacity * sizeof(void*));
				if (!new_children)
				{
					p->error_indicator = 1;
					CrError_NoMemory();
					p->level--;
					return NULL;
				}
				children = new_children;
			}
			children[n++] = result;
			mark = p->mark;
		}
		p->mark = mark;
		CrParser_PrintFail("looprule_14", "(star_targets '=')");
	}
	if (n == 0 || p->error_indicator)
	{
		Mem_Free(children);
		p->level--;
		return NULL;
	}
	ast_seq* seq = (ast_seq*)CrAST_NewGenericSeq(n, p->arena);
	if (!seq)
	{
		Mem_Free(children);
		p->error_indicator = 1;
		CrError_NoMemory();
		p->level--;
		return NULL;
	}
	for (int i = 0; i < n; i++)
		CrAST_SEQ_SET_UNTYPED(seq, i, children[i]);
	Mem_Free(children);
	CrGen_Memo_Insert(p, start_mark, type_looprule_14, seq);
	p->level--;
	return seq;
}

// gatherrule_15: yield_expr | star_expressions
void* gatherrule_15(Parser* p)
{
	RULE_HEAD();

	void* result = NULL;
	int mark = p->mark;
	{ // yield_expr
		if (p->error_indicator) {
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("gatherrule_15", "yield_expr");
		expr_type yield_expr_var;
		if (
			(yield_expr_var = rule_yield_expr(p)) // yield_expr
			)
		{
			CrParser_PrintSuccess("gatherrule_15", "yield_expr");
			result = yield_expr_var;
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("gatherrule_15", "yield_expr");
	}
	{ // star_expressions
		if (p->error_indicator) {
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("gatherrule_15", "star_expressions");
		expr_type star_expressions_var;
		if (
			(star_expressions_var = rule_star_expressions(p)) // star_expressions
			)
		{
			CrParser_PrintSuccess("gatherrule_15", "star_expressions");
			result = star_expressions_var;
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("gatherrule_15", "star_expressions");
	}
	result = NULL;
done:
	p->level--;
	return result;
}

// gatherrule_219: star_targets '='
void* gatherrule_219(Parser* p)
{
	RULE_HEAD();

	void* result = NULL;
	int mark = p->mark;
	{ // star_targets '='
		if (p->error_indicator) {
			p->level--;
			return NULL;
		}
		CrParser_PrintTest("gatherrule_219", "star_targets '='");
		expr_type target;
		Token* literal;
		if (
			(target = rule_star_targets(p)) // star_targets
			&&
			(literal = CrGen_ExpectToken(p, TOK_EQUAL)) // token = '='
			)
		{
			CrParser_PrintSuccess("gatherrule_219", "star_targets '='");
			result = target;
			if (result == NULL && CrError_Occurred())
			{
				p->error_indicator = 1;
				p->level--;
				return NULL;
			}
			goto done;
		}
		p->mark = mark;
		CrParser_PrintFail("gatherrule_219", "star_targets '='");
	}
	result = NULL;
done:
	p->level--;
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

Parser* CrParser_New(TokState* tok, int startRule, int* error_code, CrArena* arena)
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

	p->arena = arena;

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
