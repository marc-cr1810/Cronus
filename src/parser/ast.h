#pragma once

#include <port.h>
#include <objects/object.h>

typedef CrObject* identifier;
typedef CrObject* object;
typedef CrObject* string;
typedef CrObject* constant;

typedef struct _mod* mod_type;
typedef struct _stmt* stmt_type;
typedef struct _expr* expr_type;

typedef enum _expr_context { Load = 1, Store = 2, Del = 3 } expr_context_type;

typedef struct _comprehension* comprehension_type;
typedef struct _excepthandler* excepthandler_type;
typedef struct _arguments* arguments_type;
typedef struct _arg* arg_type;
typedef struct _keyword* keyword_type;
typedef struct _alias* alias_type;
typedef struct _withitem* withitem_type;
typedef struct _match_case* match_case_type;
typedef struct _pattern* pattern_type;

typedef struct _type_ignore* type_ignore_type;

/* Operator types */

typedef enum _boolop { 
	And = 1, 
	Or = 2 
} boolop_type;

typedef enum class _operator
{
	Add = 1,
	Sub,
	Mult,
	MatMult,
	Div,
	Mod,
	Pow,
	LShift,
	RShift,
	BitOr,
	BitXor,
	BitAnd,
	FloorDiv
} operator_type;

typedef enum class _unary_operator
{
	Invert = 1,
	Not,
	UAdd,
	USub
} unaryop_type;

typedef enum _cmpop {
	Eq = 1, NotEq = 2, Lt = 3, LtE = 4, Gt = 5, GtE = 6, Is = 7, IsNot = 8,
	In = 9, NotIn = 10
} cmpop_type;

/* AST node sequences */

#define AST_SEQ_HEAD \
	Cr_size_t size;     \
	void **elements;

typedef struct
{
	AST_SEQ_HEAD;
} ast_seq;

typedef struct
{
	AST_SEQ_HEAD;
	void* typed_elements[1];
} ast_generic_seq;

typedef struct
{
	AST_SEQ_HEAD;
	CrObject* typed_elements[1];
} ast_identifier_seq;

typedef struct
{
	AST_SEQ_HEAD;
	int typed_elements[1];
} ast_int_seq;

ast_generic_seq* CrAST_NewGenericSeq(Cr_size_t size);
ast_identifier_seq* CrAST_NewIdentifierSeq(Cr_size_t size);
ast_int_seq* CrAST_NewIntSeq(Cr_size_t size);

#define CrAST_SEQ_GET_UNTYPED(s, i) (s)->elements[(i)]
#define CrAST_SEQ_GET(s, i) (s)->typed_elements[(i)]
#define CrAST_SEQ_LENGTH(s) ((s) == NULL ? 0 : (s)->size)

#ifdef Cr_DEBUG
#define CrAST_SEQ_SET_UNTYPED(s, i, v) \
	do { \
		Cr_size_t ast_i = (i); \
		assert((s) != NULL); \
		assert(0 <= ast_i && ast_i < (s)->size); \
		(s)->elements[i] = (v); \
	} while(0)
#else
#define CrAST_SEQ_SET_UNTYPED(s, i, v) (s)->elements[i] = (v)
#endif

#ifdef Cr_DEBUG
#define CrAST_SEQ_SET(s, i, v) \
	do { \
		Cr_size_t ast_i = (i) \
		assert((s) != NULL); \
		assert(0 <= ast_i && ast_i < (s)->size) \
		(s)->typed_elements[i] = (v); \
	} while(0)
#else
#define CrAST_SEQ_SET(s, i, v) (s)->typed_elements[i] = (v)
#endif

typedef struct
{
	AST_SEQ_HEAD;
	mod_type typed_elements[1];
} ast_mod_seq;

typedef struct
{
	AST_SEQ_HEAD;
	stmt_type typed_elements[1];
} ast_stmt_seq;

typedef struct
{
	AST_SEQ_HEAD;
	expr_type typed_elements[1];
} ast_expr_seq;

typedef struct {
	AST_SEQ_HEAD;
	comprehension_type typed_elements[1];
} ast_comprehension_seq;

typedef struct {
	AST_SEQ_HEAD;
	excepthandler_type typed_elements[1];
} ast_excepthandler_seq;

typedef struct {
	AST_SEQ_HEAD;
	arguments_type typed_elements[1];
} ast_arguments_seq;

typedef struct {
	AST_SEQ_HEAD;
	arg_type typed_elements[1];
} ast_arg_seq;

typedef struct {
	AST_SEQ_HEAD;
	keyword_type typed_elements[1];
} ast_keyword_seq;

typedef struct {
	AST_SEQ_HEAD;
	alias_type typed_elements[1];
} ast_alias_seq;

typedef struct {
	AST_SEQ_HEAD;
	withitem_type typed_elements[1];
} ast_withitem_seq;

typedef struct {
	AST_SEQ_HEAD;
	match_case_type typed_elements[1];
} ast_match_case_seq;

typedef struct {
	AST_SEQ_HEAD;
	pattern_type typed_elements[1];
} ast_pattern_seq;

typedef struct
{
	AST_SEQ_HEAD;
	type_ignore_type typed_elements[1];
} ast_type_ignore_seq;

/* AST node types */

enum class module_kind
{
	Unknown_Kind = -1,
	Module_Kind,
	Interactive_Kind,
	Expression_Kind,
	FunctionType_Kind
};

struct _mod
{
	enum module_kind kind;
	union
	{
		struct
		{
			ast_stmt_seq* body;
			ast_type_ignore_seq* type_ignores;
		} Module;

		struct
		{
			ast_stmt_seq* body;
		} Interactive;

		struct
		{
			expr_type body;
		} Expression;

		struct
		{
			ast_expr_seq* argtypes;
			expr_type returns;
		} FunctionType;
	} v;
};

enum class stmt_kind 
{
	FunctionDef_kind = 1, AsyncFunctionDef_kind = 2, ClassDef_kind = 3,
	Return_kind = 4, Delete_kind = 5, Assign_kind = 6,
	AugAssign_kind = 7, AnnAssign_kind = 8, For_kind = 9,
	AsyncFor_kind = 10, While_kind = 11, If_kind = 12, With_kind = 13,
	AsyncWith_kind = 14, Match_kind = 15, Raise_kind = 16, Try_kind = 17,
	TryStar_kind = 18, Assert_kind = 19, Import_kind = 20,
	ImportFrom_kind = 21, Global_kind = 22, Nonlocal_kind = 23,
	Expr_kind = 24, Pass_kind = 25, Break_kind = 26, Continue_kind = 27
};

struct _stmt
{
	enum stmt_kind kind;
	union
	{
		struct {
			identifier name;
			arguments_type args;
			ast_stmt_seq* body;
			ast_expr_seq* decorator_list;
			expr_type returns;
			string type_comment;
		} FunctionDef;

		struct {
			identifier name;
			arguments_type args;
			ast_stmt_seq* body;
			ast_expr_seq* decorator_list;
			expr_type returns;
			string type_comment;
		} AsyncFunctionDef;

		struct {
			identifier name;
			ast_expr_seq* bases;
			ast_keyword_seq* keywords;
			ast_stmt_seq* body;
			ast_expr_seq* decorator_list;
		} ClassDef;

		struct {
			expr_type value;
		} Return;

		struct {
			ast_expr_seq* targets;
		} Delete;

		struct {
			ast_expr_seq* targets;
			expr_type value;
			string type_comment;
		} Assign;

		struct {
			expr_type target;
			operator_type op;
			expr_type value;
		} AugAssign;

		struct {
			expr_type target;
			expr_type annotation;
			expr_type value;
			int simple;
		} AnnAssign;

		struct {
			expr_type target;
			expr_type iter;
			ast_stmt_seq* body;
			ast_stmt_seq* orelse;
			string type_comment;
		} For;

		struct {
			expr_type target;
			expr_type iter;
			ast_stmt_seq* body;
			ast_stmt_seq* orelse;
			string type_comment;
		} AsyncFor;

		struct {
			expr_type test;
			ast_stmt_seq* body;
			ast_stmt_seq* orelse;
		} While;

		struct {
			expr_type test;
			ast_stmt_seq* body;
			ast_stmt_seq* orelse;
		} If;

		struct {
			ast_withitem_seq* items;
			ast_stmt_seq* body;
			string type_comment;
		} With;

		struct {
			ast_withitem_seq* items;
			ast_stmt_seq* body;
			string type_comment;
		} AsyncWith;

		struct {
			expr_type subject;
			ast_match_case_seq* cases;
		} Match;

		struct {
			expr_type exc;
			expr_type cause;
		} Raise;

		struct {
			ast_stmt_seq* body;
			ast_excepthandler_seq* handlers;
			ast_stmt_seq* orelse;
			ast_stmt_seq* finalbody;
		} Try;

		struct {
			ast_stmt_seq* body;
			ast_excepthandler_seq* handlers;
			ast_stmt_seq* orelse;
			ast_stmt_seq* finalbody;
		} TryStar;

		struct {
			expr_type test;
			expr_type msg;
		} Assert;

		struct {
			ast_alias_seq* names;
		} Import;

		struct {
			identifier module;
			ast_alias_seq* names;
			int level;
		} ImportFrom;

		struct {
			ast_identifier_seq* names;
		} Global;

		struct {
			ast_identifier_seq* names;
		} Nonlocal;

		struct {
			expr_type value;
		} Expr;
	} v;
	int lineno;
	int col_offset;
	int end_lineno;
	int end_col_offset;
};

enum class expr_kind
{
	BoolOp_kind = 1, NamedExpr_kind = 2, BinOp_kind = 3, UnaryOp_kind = 4,
	Lambda_kind = 5, IfExp_kind = 6, Dict_kind = 7, Set_kind = 8,
	ListComp_kind = 9, SetComp_kind = 10, DictComp_kind = 11,
	GeneratorExp_kind = 12, Await_kind = 13, Yield_kind = 14,
	YieldFrom_kind = 15, Compare_kind = 16, Call_kind = 17,
	FormattedValue_kind = 18, JoinedStr_kind = 19, Constant_kind = 20,
	Attribute_kind = 21, Subscript_kind = 22, Starred_kind = 23,
	Name_kind = 24, List_kind = 25, Tuple_kind = 26, Slice_kind = 27
};

struct _expr
{
	enum expr_kind kind;
	union
	{
		struct {
			boolop_type op;
			ast_expr_seq* values;
		} BoolOp;

		struct {
			expr_type left;
			operator_type op;
			expr_type right;
		} BinOp;

		struct {
			unaryop_type op;
			expr_type operand;
		} UnaryOp;

		struct {
			expr_type target;
			expr_type value;
		} NamedExpr;

		struct {
			arguments_type args;
			expr_type body;
		} Lambda;

		struct {
			expr_type test;
			expr_type body;
			expr_type orelse;
		} IfExp;

		struct {
			ast_expr_seq* keys;
			ast_expr_seq* values;
		} Dict;

		struct {
			ast_expr_seq* elts;
		} Set;

		struct {
			expr_type elt;
			ast_comprehension_seq* generators;
		} ListComp;

		struct {
			expr_type elt;
			ast_comprehension_seq* generators;
		} SetComp;

		struct {
			expr_type key;
			expr_type value;
			ast_comprehension_seq* generators;
		} DictComp;

		struct {
			expr_type elt;
			ast_comprehension_seq* generators;
		} GeneratorExp;

		struct {
			expr_type value;
		} Await;

		struct {
			expr_type value;
		} Yield;

		struct {
			expr_type value;
		} YieldFrom;

		struct {
			expr_type left;
			ast_int_seq* ops;
			ast_expr_seq* comparators;
		} Compare;

		struct {
			expr_type func;
			ast_expr_seq* args;
			ast_keyword_seq* keywords;
		} Call;

		struct {
			expr_type value;
			int conversion;
			expr_type format_spec;
		} FormattedValue;

		struct {
			ast_expr_seq* values;
		} JoinedStr;

		struct {
			constant value;
			string kind;
		} Constant;

		struct {
			expr_type value;
			identifier attr;
			expr_context_type ctx;
		} Attribute;

		struct {
			expr_type value;
			expr_type slice;
			expr_context_type ctx;
		} Subscript;

		struct {
			expr_type value;
			expr_context_type ctx;
		} Starred;

		struct {
			identifier id;
			expr_context_type ctx;
		} Name;

		struct {
			ast_expr_seq* elts;
			expr_context_type ctx;
		} List;

		struct {
			ast_expr_seq* elts;
			expr_context_type ctx;
		} Tuple;

		struct {
			expr_type lower;
			expr_type upper;
			expr_type step;
		} Slice;
	} v;
	int lineno;
	int col_offset;
	int end_lineno;
	int end_col_offset;
};

struct _comprehension {
	expr_type target;
	expr_type iter;
	ast_expr_seq* ifs;
	int is_async;
};

enum _excepthandler_kind { ExceptHandler_kind = 1 };
struct _excepthandler {
	enum _excepthandler_kind kind;
	union {
		struct {
			expr_type type;
			identifier name;
			ast_stmt_seq* body;
		} ExceptHandler;
	} v;
	int lineno;
	int col_offset;
	int end_lineno;
	int end_col_offset;
};

struct _arguments {
	ast_arg_seq* posonlyargs;
	ast_arg_seq* args;
	arg_type vararg;
	ast_arg_seq* kwonlyargs;
	ast_expr_seq* kw_defaults;
	arg_type kwarg;
	ast_expr_seq* defaults;
};

struct _arg {
	identifier arg;
	expr_type annotation;
	string type_comment;
	int lineno;
	int col_offset;
	int end_lineno;
	int end_col_offset;
};

struct _keyword {
	identifier arg;
	expr_type value;
	int lineno;
	int col_offset;
	int end_lineno;
	int end_col_offset;
};

struct _alias {
	identifier name;
	identifier asname;
	int lineno;
	int col_offset;
	int end_lineno;
	int end_col_offset;
};

struct _withitem {
	expr_type context_expr;
	expr_type optional_vars;
};

struct _match_case {
	pattern_type pattern;
	expr_type guard;
	ast_stmt_seq* body;
};

enum _pattern_kind {
	MatchValue_kind = 1, MatchSingleton_kind = 2,
	MatchSequence_kind = 3, MatchMapping_kind = 4,
	MatchClass_kind = 5, MatchStar_kind = 6, MatchAs_kind = 7,
	MatchOr_kind = 8
};
struct _pattern {
	enum _pattern_kind kind;
	union {
		struct {
			expr_type value;
		} MatchValue;

		struct {
			constant value;
		} MatchSingleton;

		struct {
			ast_pattern_seq* patterns;
		} MatchSequence;

		struct {
			ast_expr_seq* keys;
			ast_pattern_seq* patterns;
			identifier rest;
		} MatchMapping;

		struct {
			expr_type cls;
			ast_pattern_seq* patterns;
			ast_identifier_seq* kwd_attrs;
			ast_pattern_seq* kwd_patterns;
		} MatchClass;

		struct {
			identifier name;
		} MatchStar;

		struct {
			pattern_type pattern;
			identifier name;
		} MatchAs;

		struct {
			ast_pattern_seq* patterns;
		} MatchOr;

	} v;
	int lineno;
	int col_offset;
	int end_lineno;
	int end_col_offset;
};

enum _type_ignore_kind { TypeIgnore_kind = 1 };
struct _type_ignore
{
	enum _type_ignore_kind kind;
	union
	{
		struct
		{
			int lineno;
			string tag;
		} TypeIgnore;
	} v;
};

/* Create nodes */

mod_type CrAST_Module(ast_stmt_seq* body, ast_type_ignore_seq* type_ignores);
mod_type CrAST_Interactive(ast_stmt_seq* body);
mod_type CrAST_FunctionType(ast_expr_seq* arg_types, expr_type returns);

stmt_type CrAST_FunctionDef(identifier name, arguments_type args, ast_stmt_seq* body,
	ast_expr_seq* decorator_list, expr_type returns, string type_comment, 
	int lineno, int col_offset, int end_lineno, int end_col_offset);
stmt_type CrAST_AsyncFunctionDef(identifier name, arguments_type args, ast_stmt_seq* body,
	ast_expr_seq* decorator_list, expr_type returns, string type_comment,
	int lineno, int col_offset, int end_lineno, int end_col_offset);
stmt_type CrAST_ClassDef(identifier name, ast_expr_seq* bases, ast_keyword_seq* keywords, ast_stmt_seq* body, ast_expr_seq* decorator_list,
	int lineno, int col_offset, int end_lineno, int end_col_offset);
stmt_type CrAST_Return(expr_type value, int lineno, int col_offset, int end_lineno, int end_col_offset);
stmt_type CrAST_Delete(ast_expr_seq* targets, int lineno, int col_offset, int end_lineno, int end_col_offset);
stmt_type CrAST_Assign(ast_expr_seq* targets, expr_type value, string type_comment,
	int lineno, int col_offset, int end_lineno, int end_col_offset);
stmt_type CrAST_AugAssign(expr_type target, operator_type op, expr_type value,
	int lineno, int	col_offset, int end_lineno, int end_col_offset);
stmt_type CrAST_AnnAssign(expr_type target, expr_type annotation, expr_type value, int simple,
	int lineno, int col_offset, int end_lineno, int	end_col_offset);
stmt_type CrAST_For(expr_type target, expr_type iter, ast_stmt_seq* body, ast_stmt_seq* orelse, string type_comment,
	int lineno, int col_offset, int end_lineno, int end_col_offset);
stmt_type CrAST_AsyncFor(expr_type target, expr_type iter, ast_stmt_seq* body, ast_stmt_seq* orelse, string type_comment,
	int lineno, int col_offset, int end_lineno, int end_col_offset);
stmt_type CrAST_While(expr_type test, ast_stmt_seq* body, ast_stmt_seq* orelse, 
	int lineno, int col_offset, int end_lineno, int end_col_offset);
stmt_type CrAST_If(expr_type test, ast_stmt_seq* body, ast_stmt_seq* orelse,
	int lineno, int col_offset, int end_lineno, int end_col_offset);
stmt_type CrAST_With(ast_withitem_seq* items, ast_stmt_seq* body, string type_comment, 
	int lineno, int col_offset, int end_lineno, int	end_col_offset);
stmt_type CrAST_AsyncWith(ast_withitem_seq* items, ast_stmt_seq* body, string type_comment,
	int lineno, int col_offset, int end_lineno, int	end_col_offset);
stmt_type CrAST_Match(expr_type subject, ast_match_case_seq* cases,
	int lineno, int	col_offset, int end_lineno, int end_col_offset);
stmt_type CrAST_Raise(expr_type exc, expr_type cause, int lineno, int col_offset, int end_lineno, int end_col_offset);
stmt_type CrAST_Try(ast_stmt_seq* body, ast_excepthandler_seq* handlers, ast_stmt_seq* orelse, ast_stmt_seq* finalbody,
	int lineno, int	col_offset, int end_lineno, int end_col_offset);
stmt_type CrAST_TryStar(ast_stmt_seq* body, ast_excepthandler_seq* handlers, ast_stmt_seq* orelse, ast_stmt_seq* finalbody,
	int lineno, int	col_offset, int end_lineno, int end_col_offset);
stmt_type CrAST_Assert(expr_type test, expr_type msg, int lineno, int col_offset, int end_lineno, int end_col_offset);
stmt_type CrAST_Import(ast_alias_seq* names, int lineno, int col_offset, int end_lineno, int end_col_offset);
stmt_type CrAST_ImportFrom(identifier module, ast_alias_seq* names, int level, int lineno, int col_offset, int end_lineno, int end_col_offset);
stmt_type CrAST_Global(ast_identifier_seq* names, int lineno, int col_offset, int end_lineno, int end_col_offset);
stmt_type CrAST_Nonlocal(ast_identifier_seq* names, int lineno, int col_offset, int end_lineno, int end_col_offset);
stmt_type CrAST_Expr(expr_type value, int lineno, int col_offset, int end_lineno, int end_col_offset);
stmt_type CrAST_Pass(int lineno, int col_offset, int end_lineno, int end_col_offset);
stmt_type CrAST_Break(int lineno, int col_offset, int end_lineno, int end_col_offset);
stmt_type CrAST_Continue(int lineno, int col_offset, int end_lineno, int end_col_offset);

expr_type CrAST_BoolOp(boolop_type op, ast_expr_seq* values, int lineno, int col_offset, int end_lineno, int end_col_offset);
expr_type CrAST_BinOp(expr_type left, operator_type op, expr_type right, int lineno, int col_offset, int end_lineno, int end_col_offset);
expr_type CrAST_UnaryOp(unaryop_type op, expr_type operand, int lineno, int col_offset, int end_lineno, int end_col_offset);
expr_type CrAST_NamedExpr(expr_type target, expr_type value, int lineno, int col_offset, int end_lineno, int end_col_offset);
expr_type CrAST_Lambda(arguments_type args, expr_type body, int lineno, int col_offset, int end_lineno, int end_col_offset);
expr_type CrAST_IfExp(expr_type test, expr_type body, expr_type orelse, int lineno, int col_offset, int end_lineno, int end_col_offset);
expr_type CrAST_Dict(ast_expr_seq* keys, ast_expr_seq* values, int lineno, int col_offset, int end_lineno, int end_col_offset);
expr_type CrAST_Set(ast_expr_seq* elts, int lineno, int col_offset, int end_lineno, int end_col_offset);
expr_type CrAST_ListComp(expr_type elt, ast_comprehension_seq* generators, int lineno, int col_offset, int end_lineno, int end_col_offset);
expr_type CrAST_SetComp(expr_type elt, ast_comprehension_seq* generators, int lineno, int col_offset, int end_lineno, int end_col_offset);
expr_type CrAST_DictComp(expr_type key, expr_type value, ast_comprehension_seq* generators,
	int lineno, int col_offset, int end_lineno, int end_col_offset);
expr_type CrAST_GeneratorExp(expr_type elt, ast_comprehension_seq* generators, int lineno, int col_offset, int end_lineno, int end_col_offset);
expr_type CrAST_Await(expr_type value, int lineno, int col_offset, int end_lineno, int end_col_offset);
expr_type CrAST_Yield(expr_type value, int lineno, int col_offset, int end_lineno, int end_col_offset);
expr_type CrAST_YieldFrom(expr_type value, int lineno, int col_offset, int end_lineno, int end_col_offset);
expr_type CrAST_Compare(expr_type left, ast_int_seq* ops, ast_expr_seq* comparators,
	int lineno, int col_offset, int end_lineno, int end_col_offset);
expr_type CrAST_Call(expr_type func, ast_expr_seq* args, ast_keyword_seq* keywords,
	int lineno, int col_offset, int end_lineno, int end_col_offset);
expr_type CrAST_FormattedValue(expr_type value, int conversion, expr_type format_spec,
	int lineno, int col_offset, int end_lineno, int end_col_offset);
expr_type CrAST_JoinedStr(ast_expr_seq* values, int lineno, int col_offset, int end_lineno, int end_col_offset);
expr_type CrAST_Constant(constant value, string kind, int lineno, int col_offset, int end_lineno, int end_col_offset);
expr_type CrAST_Attribute(expr_type value, identifier attr, expr_context_type ctx, 
	int	lineno, int col_offset, int end_lineno, int end_col_offset);
expr_type CrAST_Subscript(expr_type value, expr_type slice, expr_context_type ctx, int lineno, int col_offset, int end_lineno, int end_col_offset);
expr_type CrAST_Starred(expr_type value, expr_context_type ctx, int lineno, int col_offset, int end_lineno, int end_col_offset);
expr_type CrAST_Name(identifier id, expr_context_type ctx, int lineno, int col_offset, int	end_lineno, int end_col_offset);
expr_type CrAST_List(ast_expr_seq* elts, expr_context_type ctx, int lineno, int col_offset, int end_lineno, int end_col_offset);
expr_type CrAST_Tuple(ast_expr_seq* elts, expr_context_type ctx, int lineno, int col_offset, int end_lineno, int end_col_offset);
expr_type CrAST_Slice(expr_type lower, expr_type upper, expr_type step, int lineno, int col_offset, int end_lineno, int end_col_offset);