#include "ast.h"

#include <core/mem.h>
#include <core/error.h>

ast_generic_seq* CrAST_NewGenericSeq(Cr_size_t size, CrArena* arena)
{
	ast_generic_seq* seq = NULL;
	size_t n = 0;

	// Check size is ok
	if (size < 0 || (size && (((size_t)size - 1) > (SIZE_MAX / sizeof(void*)))))
	{
		CrError_NoMemory();
		return NULL;
	}
	n += sizeof(ast_generic_seq);
	seq = (ast_generic_seq*)CrArena_Alloc(arena, n);
	if (!seq)
	{
		CrError_NoMemory();
		return NULL;
	}
	memset(seq, 0, n);
	seq->size = size;
	seq->elements = (void**)seq->typed_elements;
	return seq;
}

ast_identifier_seq* CrAST_NewIdentifierSeq(Cr_size_t size, CrArena* arena)
{
	ast_identifier_seq* seq = NULL;
	size_t n = 0;

	// Check size is ok
	if (size < 0 || (size && (((size_t)size - 1) > (SIZE_MAX / sizeof(void*)))))
	{
		CrError_NoMemory();
		return NULL;
	}
	n += sizeof(ast_identifier_seq);
	seq = (ast_identifier_seq*)CrArena_Alloc(arena, n);
	if (!seq)
	{
		CrError_NoMemory();
		return NULL;
	}
	memset(seq, 0, n);
	seq->size = size;
	seq->elements = (void**)seq->typed_elements;
	return seq;
}

ast_int_seq* CrAST_NewIntSeq(Cr_size_t size, CrArena* arena)
{
	ast_int_seq* seq = NULL;
	size_t n = 0;

	// Check size is ok
	if (size < 0 || (size && (((size_t)size - 1) > (SIZE_MAX / sizeof(void*)))))
	{
		CrError_NoMemory();
		return NULL;
	}
	n += sizeof(ast_int_seq);
	seq = (ast_int_seq*)CrArena_Alloc(arena, n);
	if (!seq)
	{
		CrError_NoMemory();
		return NULL;
	}
	memset(seq, 0, n);
	seq->size = size;
	seq->elements = (void**)seq->typed_elements;
	return seq;
}

ast_expr_seq* CrAST_NewExprSeq(Cr_size_t size, CrArena* arena)
{
	ast_expr_seq* seq = NULL;
	size_t n = 0;

	// Check size is ok
	if (size < 0 || (size && (((size_t)size - 1) > (SIZE_MAX / sizeof(void*)))))
	{
		CrError_NoMemory();
		return NULL;
	}
	n += sizeof(ast_expr_seq);
	seq = (ast_expr_seq*)CrArena_Alloc(arena, n);
	if (!seq)
	{
		CrError_NoMemory();
		return NULL;
	}
	memset(seq, 0, n);
	seq->size = size;
	seq->elements = (void**)seq->typed_elements;
	return seq;
}

//
//
//		Create node definitions
//
//

mod_type CrAST_Module(ast_stmt_seq* body, ast_type_ignore_seq* type_ignores, CrArena* arena)
{
	mod_type mod;
	mod = (mod_type)CrArena_Alloc(arena, sizeof(*mod));
	if (!mod)
		return NULL;
	mod->kind = module_kind::Module_Kind;
	mod->v.Module.body = body;
	mod->v.Module.type_ignores = type_ignores;
	return mod;
}

mod_type CrAST_Interactive(ast_stmt_seq* body, CrArena* arena)
{
	mod_type mod;
	mod = (mod_type)CrArena_Alloc(arena, sizeof(*mod));
	if (!mod)
		return NULL;
	mod->kind = module_kind::Interactive_Kind;
	mod->v.Interactive.body = body;
	return mod;
}

mod_type CrAST_FunctionType(ast_expr_seq* argtypes, expr_type returns, CrArena* arena)
{
	mod_type mod;
	if (!returns)
	{
		CrError_SetString(CrExc_ValueError, "field 'returns' is required for CrAST_FunctionType");
		return NULL;
	}
	mod = (mod_type)CrArena_Alloc(arena, sizeof(*mod));
	if (!mod)
		return NULL;
	mod->kind = module_kind::FunctionType_Kind;
	mod->v.FunctionType.argtypes = argtypes;
	mod->v.FunctionType.returns = returns;
	return mod;
}

stmt_type CrAST_FunctionDef(identifier name, arguments_type args, ast_stmt_seq* body, 
	ast_expr_seq* decorator_list, expr_type returns, string type_comment, 
	int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	stmt_type stmt;
	if (!name)
	{
		CrError_SetString(CrExc_ValueError, "field 'name' is required for CrAST_FunctionDef");
		return NULL;
	}
	if (!args)
	{
		CrError_SetString(CrExc_ValueError, "field 'args' is required for CrAST_FunctionDef");
		return NULL;
	}

	stmt = (stmt_type)CrArena_Alloc(arena, sizeof(*stmt));
	if (!stmt)
		return NULL;
	stmt->kind = stmt_kind::FunctionDef_kind;
	stmt->v.FunctionDef.name = name;
	stmt->v.FunctionDef.args = args;
	stmt->v.FunctionDef.body = body;
	stmt->v.FunctionDef.decorator_list = decorator_list;
	stmt->v.FunctionDef.returns = returns;
	stmt->v.FunctionDef.type_comment = type_comment;
	stmt->lineno = lineno;
	stmt->col_offset = col_offset;
	stmt->end_lineno = end_lineno;
	stmt->end_col_offset = end_col_offset;
	return stmt;
}

stmt_type CrAST_AsyncFunctionDef(identifier name, arguments_type args, ast_stmt_seq* body, ast_expr_seq* decorator_list, expr_type returns, string type_comment, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	stmt_type stmt;
	if (!name)
	{
		CrError_SetString(CrExc_ValueError, "field 'name' is required for CrAST_AsyncFunctionDef");
		return NULL;
	}
	if (!args)
	{
		CrError_SetString(CrExc_ValueError, "field 'args' is required for CrAST_AsyncFunctionDef");
		return NULL;
	}

	stmt = (stmt_type)CrArena_Alloc(arena, sizeof(*stmt));
	if (!stmt)
		return NULL;
	stmt->kind = stmt_kind::AsyncFunctionDef_kind;
	stmt->v.AsyncFunctionDef.name = name;
	stmt->v.AsyncFunctionDef.args = args;
	stmt->v.AsyncFunctionDef.body = body;
	stmt->v.AsyncFunctionDef.decorator_list = decorator_list;
	stmt->v.AsyncFunctionDef.returns = returns;
	stmt->v.AsyncFunctionDef.type_comment = type_comment;
	stmt->lineno = lineno;
	stmt->col_offset = col_offset;
	stmt->end_lineno = end_lineno;
	stmt->end_col_offset = end_col_offset;
	return stmt;
}

stmt_type CrAST_ClassDef(identifier name, ast_expr_seq* bases, ast_keyword_seq* keywords, ast_stmt_seq* body, ast_expr_seq* decorator_list, 
	int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	stmt_type stmt;
	if (!name)
	{
		CrError_SetString(CrExc_ValueError, "field 'name' is required for CrAST_ClassDef");
		return NULL;
	}

	stmt = (stmt_type)CrArena_Alloc(arena, sizeof(*stmt));
	if (!stmt)
		return NULL;
	stmt->kind = stmt_kind::ClassDef_kind;
	stmt->v.ClassDef.name = name;
	stmt->v.ClassDef.bases = bases;
	stmt->v.ClassDef.keywords = keywords;
	stmt->v.ClassDef.body = body;
	stmt->v.ClassDef.decorator_list = decorator_list;
	stmt->lineno = lineno;
	stmt->col_offset = col_offset;
	stmt->end_lineno = end_lineno;
	stmt->end_col_offset = end_col_offset;
	return stmt;
}

stmt_type CrAST_Return(expr_type value, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	stmt_type stmt;
	stmt = (stmt_type)CrArena_Alloc(arena, sizeof(stmt));
	if (!stmt)
		return NULL;
	stmt->kind = stmt_kind::Return_kind;
	stmt->v.Return.value = value;
	stmt->lineno = lineno;
	stmt->col_offset = col_offset;
	stmt->end_lineno = end_lineno;
	stmt->end_col_offset = end_col_offset;
	return stmt;
}

stmt_type CrAST_Delete(ast_expr_seq* targets, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	stmt_type stmt;
	stmt = (stmt_type)CrArena_Alloc(arena, sizeof(stmt));
	if (!stmt)
		return NULL;
	stmt->kind = stmt_kind::Delete_kind;
	stmt->v.Delete.targets = targets;
	stmt->lineno = lineno;
	stmt->col_offset = col_offset;
	stmt->end_lineno = end_lineno;
	stmt->end_col_offset = end_col_offset;
	return stmt;
}

stmt_type CrAST_Assign(ast_expr_seq* targets, expr_type value, string type_comment, 
	int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	stmt_type stmt;
	if (!value) {
		CrError_SetString(CrExc_ValueError, "field 'value' is required for CrAST_Assign");
		return NULL;
	}
	stmt = (stmt_type)CrArena_Alloc(arena, sizeof(*stmt));
	if (!stmt)
		return NULL;
	stmt->kind = stmt_kind::Assign_kind;
	stmt->v.Assign.targets = targets;
	stmt->v.Assign.value = value;
	stmt->v.Assign.type_comment = type_comment;
	stmt->lineno = lineno;
	stmt->col_offset = col_offset;
	stmt->end_lineno = end_lineno;
	stmt->end_col_offset = end_col_offset;
	return stmt;
}

stmt_type CrAST_AugAssign(expr_type target, operator_type op, expr_type value, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	stmt_type stmt;
	if (!target) {
		CrError_SetString(CrExc_ValueError, "field 'target' is required for CrAST_AugAssign");
		return NULL;
	}
	if (!static_cast<int>(op)) {
		CrError_SetString(CrExc_ValueError, "field 'op' is required for CrAST_AugAssign");
		return NULL;
	}
	if (!value) {
		CrError_SetString(CrExc_ValueError,	"field 'value' is required for CrAST_AugAssign");
		return NULL;
	}
	stmt = (stmt_type)CrArena_Alloc(arena, sizeof(*stmt));
	if (!stmt)
		return NULL;
	stmt->kind = stmt_kind::AugAssign_kind;
	stmt->v.AugAssign.target = target;
	stmt->v.AugAssign.op = op;
	stmt->v.AugAssign.value = value;
	stmt->lineno = lineno;
	stmt->col_offset = col_offset;
	stmt->end_lineno = end_lineno;
	stmt->end_col_offset = end_col_offset;
	return stmt;
}

stmt_type CrAST_AnnAssign(expr_type target, expr_type annotation, expr_type value, int simple, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	stmt_type stmt;
	if (!target) {
		CrError_SetString(CrExc_ValueError,	"field 'target' is required for CrAST_AnnAssign");
		return NULL;
	}
	if (!annotation) {
		CrError_SetString(CrExc_ValueError, "field 'annotation' is required for CrAST_AnnAssign");
		return NULL;
	}
	stmt = (stmt_type)CrArena_Alloc(arena, sizeof(*stmt));
	if (!stmt)
		return NULL;
	stmt->kind = stmt_kind::AnnAssign_kind;
	stmt->v.AnnAssign.target = target;
	stmt->v.AnnAssign.annotation = annotation;
	stmt->v.AnnAssign.value = value;
	stmt->v.AnnAssign.simple = simple;
	stmt->lineno = lineno;
	stmt->col_offset = col_offset;
	stmt->end_lineno = end_lineno;
	stmt->end_col_offset = end_col_offset;
	return stmt;
}

stmt_type CrAST_For(expr_type target, expr_type iter, ast_stmt_seq* body, ast_stmt_seq* orelse, string type_comment, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	stmt_type stmt;
	if (!target) {
		CrError_SetString(CrExc_ValueError, "field 'target' is required for CrAST_For");
		return NULL;
	}
	if (!iter) {
		CrError_SetString(CrExc_ValueError, "field 'iter' is required for CrAST_For");
		return NULL;
	}
	stmt = (stmt_type)CrArena_Alloc(arena, sizeof(*stmt));
	if (!stmt)
		return NULL;
	stmt->kind = stmt_kind::For_kind;
	stmt->v.For.target = target;
	stmt->v.For.iter = iter;
	stmt->v.For.body = body;
	stmt->v.For.orelse = orelse;
	stmt->v.For.type_comment = type_comment;
	stmt->lineno = lineno;
	stmt->col_offset = col_offset;
	stmt->end_lineno = end_lineno;
	stmt->end_col_offset = end_col_offset;
	return stmt;
}

stmt_type CrAST_AsyncFor(expr_type target, expr_type iter, ast_stmt_seq* body, ast_stmt_seq* orelse, string type_comment, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	stmt_type stmt;
	if (!target) {
		CrError_SetString(CrExc_ValueError, "field 'target' is required for CrAST_AsyncFor");
		return NULL;
	}
	if (!iter) {
		CrError_SetString(CrExc_ValueError, "field 'iter' is required for CrAST_AsyncFor");
		return NULL;
	}
	stmt = (stmt_type)CrArena_Alloc(arena, sizeof(*stmt));
	if (!stmt)
		return NULL;
	stmt->kind = stmt_kind::AsyncFor_kind;
	stmt->v.AsyncFor.target = target;
	stmt->v.AsyncFor.iter = iter;
	stmt->v.AsyncFor.body = body;
	stmt->v.AsyncFor.orelse = orelse;
	stmt->v.AsyncFor.type_comment = type_comment;
	stmt->lineno = lineno;
	stmt->col_offset = col_offset;
	stmt->end_lineno = end_lineno;
	stmt->end_col_offset = end_col_offset;
	return stmt;
}

stmt_type CrAST_While(expr_type test, ast_stmt_seq* body, ast_stmt_seq* orelse, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	stmt_type stmt;
	if (!test) {
		CrError_SetString(CrExc_ValueError, "field 'test' is required for CrAST_While");
		return NULL;
	}
	stmt = (stmt_type)CrArena_Alloc(arena, sizeof(*stmt));
	if (!stmt)
		return NULL;
	stmt->kind = stmt_kind::While_kind;
	stmt->v.While.test = test;
	stmt->v.While.body = body;
	stmt->v.While.orelse = orelse;
	stmt->lineno = lineno;
	stmt->col_offset = col_offset;
	stmt->end_lineno = end_lineno;
	stmt->end_col_offset = end_col_offset;
	return stmt;
}

stmt_type CrAST_If(expr_type test, ast_stmt_seq* body, ast_stmt_seq* orelse, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	stmt_type stmt;
	if (!test) {
		CrError_SetString(CrExc_ValueError, "field 'test' is required for CrAST_If");
		return NULL;
	}
	stmt = (stmt_type)CrArena_Alloc(arena, sizeof(*stmt));
	if (!stmt)
		return NULL;
	stmt->kind = stmt_kind::If_kind;
	stmt->v.If.test = test;
	stmt->v.If.body = body;
	stmt->v.If.orelse = orelse;
	stmt->lineno = lineno;
	stmt->col_offset = col_offset;
	stmt->end_lineno = end_lineno;
	stmt->end_col_offset = end_col_offset;
	return stmt;
}

stmt_type CrAST_With(ast_withitem_seq* items, ast_stmt_seq* body, string type_comment, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	stmt_type stmt;
	stmt = (stmt_type)CrArena_Alloc(arena, sizeof(*stmt));
	if (!stmt)
		return NULL;
	stmt->kind = stmt_kind::With_kind;
	stmt->v.With.items = items;
	stmt->v.With.body = body;
	stmt->v.With.type_comment = type_comment;
	stmt->lineno = lineno;
	stmt->col_offset = col_offset;
	stmt->end_lineno = end_lineno;
	stmt->end_col_offset = end_col_offset;
	return stmt;
}

stmt_type CrAST_AsyncWith(ast_withitem_seq* items, ast_stmt_seq* body, string type_comment, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	stmt_type stmt;
	stmt = (stmt_type)CrArena_Alloc(arena, sizeof(*stmt));
	if (!stmt)
		return NULL;
	stmt->kind = stmt_kind::AsyncWith_kind;
	stmt->v.AsyncWith.items = items;
	stmt->v.AsyncWith.body = body;
	stmt->v.AsyncWith.type_comment = type_comment;
	stmt->lineno = lineno;
	stmt->col_offset = col_offset;
	stmt->end_lineno = end_lineno;
	stmt->end_col_offset = end_col_offset;
	return stmt;
}

stmt_type CrAST_Match(expr_type subject, ast_match_case_seq* cases, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	stmt_type stmt;
	if (!subject) {
		CrError_SetString(CrExc_ValueError, "field 'subject' is required for CrAST_Match");
		return NULL;
	}
	stmt = (stmt_type)CrArena_Alloc(arena, sizeof(*stmt));
	if (!stmt)
		return NULL;
	stmt->kind = stmt_kind::Match_kind;
	stmt->v.Match.subject = subject;
	stmt->v.Match.cases = cases;
	stmt->lineno = lineno;
	stmt->col_offset = col_offset;
	stmt->end_lineno = end_lineno;
	stmt->end_col_offset = end_col_offset;
	return stmt;
}

stmt_type CrAST_Raise(expr_type exc, expr_type cause, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	stmt_type stmt;
	stmt = (stmt_type)CrArena_Alloc(arena, sizeof(*stmt));
	if (!stmt)
		return NULL;
	stmt->kind = stmt_kind::Raise_kind;
	stmt->v.Raise.exc = exc;
	stmt->v.Raise.cause = cause;
	stmt->lineno = lineno;
	stmt->col_offset = col_offset;
	stmt->end_lineno = end_lineno;
	stmt->end_col_offset = end_col_offset;
	return stmt;
}

stmt_type CrAST_Try(ast_stmt_seq* body, ast_excepthandler_seq* handlers, ast_stmt_seq* orelse, ast_stmt_seq* finalbody, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	stmt_type stmt;
	stmt = (stmt_type)CrArena_Alloc(arena, sizeof(*stmt));
	if (!stmt)
		return NULL;
	stmt->kind = stmt_kind::Try_kind;
	stmt->v.Try.body = body;
	stmt->v.Try.handlers = handlers;
	stmt->v.Try.orelse = orelse;
	stmt->v.Try.finalbody = finalbody;
	stmt->lineno = lineno;
	stmt->col_offset = col_offset;
	stmt->end_lineno = end_lineno;
	stmt->end_col_offset = end_col_offset;
	return stmt;
}

stmt_type CrAST_TryStar(ast_stmt_seq* body, ast_excepthandler_seq* handlers, ast_stmt_seq* orelse, ast_stmt_seq* finalbody, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	stmt_type stmt;
	stmt = (stmt_type)CrArena_Alloc(arena, sizeof(*stmt));
	if (!stmt)
		return NULL;
	stmt->kind = stmt_kind::TryStar_kind;
	stmt->v.TryStar.body = body;
	stmt->v.TryStar.handlers = handlers;
	stmt->v.TryStar.orelse = orelse;
	stmt->v.TryStar.finalbody = finalbody;
	stmt->lineno = lineno;
	stmt->col_offset = col_offset;
	stmt->end_lineno = end_lineno;
	stmt->end_col_offset = end_col_offset;
	return stmt;
}

stmt_type CrAST_Assert(expr_type test, expr_type msg, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	stmt_type stmt;
	if (!test) {
		CrError_SetString(CrExc_ValueError, "field 'test' is required for CrAST_Assert");
		return NULL;
	}
	stmt = (stmt_type)CrArena_Alloc(arena, sizeof(*stmt));
	if (!stmt)
		return NULL;
	stmt->kind = stmt_kind::Assert_kind;
	stmt->v.Assert.test = test;
	stmt->v.Assert.msg = msg;
	stmt->lineno = lineno;
	stmt->col_offset = col_offset;
	stmt->end_lineno = end_lineno;
	stmt->end_col_offset = end_col_offset;
	return stmt;
}

stmt_type CrAST_Import(ast_alias_seq* names, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	stmt_type stmt;
	stmt = (stmt_type)CrArena_Alloc(arena, sizeof(*stmt));
	if (!stmt)
		return NULL;
	stmt->kind = stmt_kind::Import_kind;
	stmt->v.Import.names = names;
	stmt->lineno = lineno;
	stmt->col_offset = col_offset;
	stmt->end_lineno = end_lineno;
	stmt->end_col_offset = end_col_offset;
	return stmt;
}

stmt_type CrAST_ImportFrom(identifier module, ast_alias_seq* names, int level, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	stmt_type stmt;
	stmt = (stmt_type)CrArena_Alloc(arena, sizeof(*stmt));
	if (!stmt)
		return NULL;
	stmt->kind = stmt_kind::ImportFrom_kind;
	stmt->v.ImportFrom.module = module;
	stmt->v.ImportFrom.names = names;
	stmt->v.ImportFrom.level = level;
	stmt->lineno = lineno;
	stmt->col_offset = col_offset;
	stmt->end_lineno = end_lineno;
	stmt->end_col_offset = end_col_offset;
	return stmt;
}

stmt_type CrAST_Global(ast_identifier_seq* names, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	stmt_type stmt;
	stmt = (stmt_type)CrArena_Alloc(arena, sizeof(*stmt));
	if (!stmt)
		return NULL;
	stmt->kind = stmt_kind::Global_kind;
	stmt->v.Global.names = names;
	stmt->lineno = lineno;
	stmt->col_offset = col_offset;
	stmt->end_lineno = end_lineno;
	stmt->end_col_offset = end_col_offset;
	return stmt;
}

stmt_type CrAST_Nonlocal(ast_identifier_seq* names, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	stmt_type stmt;
	stmt = (stmt_type)CrArena_Alloc(arena, sizeof(*stmt));
	if (!stmt)
		return NULL;
	stmt->kind = stmt_kind::Nonlocal_kind;
	stmt->v.Nonlocal.names = names;
	stmt->lineno = lineno;
	stmt->col_offset = col_offset;
	stmt->end_lineno = end_lineno;
	stmt->end_col_offset = end_col_offset;
	return stmt;
}

stmt_type CrAST_Expr(expr_type value, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	if (!value)
	{
		CrError_SetString(CrExc_ValueError, "field 'value' is required for CrAST_Expr");
		return NULL;
	}
	stmt_type stmt;
	stmt = (stmt_type)CrArena_Alloc(arena, sizeof(stmt));
	if (!stmt)
		return NULL;
	stmt->kind = stmt_kind::Expr_kind;
	stmt->v.Expr.value = value;
	stmt->lineno = lineno;
	stmt->col_offset = col_offset;
	stmt->end_lineno = end_lineno;
	stmt->end_col_offset = end_col_offset;
	return stmt;
}

stmt_type CrAST_Pass(int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	stmt_type stmt;
	stmt = (stmt_type)CrArena_Alloc(arena, sizeof(stmt));
	if (!stmt)
		return NULL;
	stmt->kind = stmt_kind::Pass_kind;
	stmt->lineno = lineno;
	stmt->col_offset = col_offset;
	stmt->end_lineno = end_lineno;
	stmt->end_col_offset = end_col_offset;
	return stmt;
}

stmt_type CrAST_Break(int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	stmt_type stmt;
	stmt = (stmt_type)CrArena_Alloc(arena, sizeof(stmt));
	if (!stmt)
		return NULL;
	stmt->kind = stmt_kind::Break_kind;
	stmt->lineno = lineno;
	stmt->col_offset = col_offset;
	stmt->end_lineno = end_lineno;
	stmt->end_col_offset = end_col_offset;
	return stmt;
}

stmt_type CrAST_Continue(int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	stmt_type stmt;
	stmt = (stmt_type)CrArena_Alloc(arena, sizeof(stmt));
	if (!stmt)
		return NULL;
	stmt->kind = stmt_kind::Continue_kind;
	stmt->lineno = lineno;
	stmt->col_offset = col_offset;
	stmt->end_lineno = end_lineno;
	stmt->end_col_offset = end_col_offset;
	return stmt;
}

expr_type CrAST_BoolOp(boolop_type op, ast_expr_seq* values, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	expr_type expr;
	if (!op) {
		CrError_SetString(CrExc_ValueError, "field 'op' is required for CrAST_BoolOp");
		return NULL;
	}
	expr = (expr_type)CrArena_Alloc(arena, sizeof(*expr));
	if (!expr)
		return NULL;
	expr->kind = expr_kind::BoolOp_kind;
	expr->v.BoolOp.op = op;
	expr->v.BoolOp.values = values;
	expr->lineno = lineno;
	expr->col_offset = col_offset;
	expr->end_lineno = end_lineno;
	expr->end_col_offset = end_col_offset;
	return expr;
}

expr_type CrAST_BinOp(expr_type left, operator_type op, expr_type right, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	if (!left)
	{
		CrError_SetString(CrExc_ValueError, "field 'left' is required for CrAST_BinOp");
		return NULL;
	}
	if (!static_cast<int>(op))
	{
		CrError_SetString(CrExc_ValueError, "field 'op' is required for CrAST_BinOp");
		return NULL;
	}
	if (!right)
	{
		CrError_SetString(CrExc_ValueError, "field 'right' is required for CrAST_BinOp");
		return NULL;
	}
	expr_type expr;
	expr = (expr_type)CrArena_Alloc(arena, sizeof(*expr));
	if (!expr)
		return NULL;
	expr->kind = expr_kind::BinOp_kind;
	expr->v.BinOp.left = left;
	expr->v.BinOp.op = op;
	expr->v.BinOp.right = right;
	expr->lineno = lineno;
	expr->col_offset = col_offset;
	expr->end_lineno = end_lineno;
	expr->end_col_offset = end_col_offset;
	return expr;
}

expr_type CrAST_UnaryOp(unaryop_type op, expr_type operand, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	if (!static_cast<int>(op))
	{
		CrError_SetString(CrExc_ValueError, "field 'op' is required for CrAST_UnaryOp");
		return NULL;
	}
	if (!operand)
	{
		CrError_SetString(CrExc_ValueError, "field 'operand' is required for CrAST_UnaryOp");
		return NULL;
	}
	expr_type expr;
	expr = (expr_type)CrArena_Alloc(arena, sizeof(*expr));
	if (!expr)
		return NULL;
	expr->kind = expr_kind::UnaryOp_kind;
	expr->v.UnaryOp.op = op;
	expr->v.UnaryOp.operand = operand;
	expr->lineno = lineno;
	expr->col_offset = col_offset;
	expr->end_lineno = end_lineno;
	expr->end_col_offset = end_col_offset;
	return expr;
}

expr_type CrAST_NamedExpr(expr_type target, expr_type value, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	expr_type expr;
	if (!target) {
		CrError_SetString(CrExc_ValueError, "field 'target' is required for CrAST_NamedExpr");
		return NULL;
	}
	if (!value) {
		CrError_SetString(CrExc_ValueError, "field 'value' is required for CrAST_NamedExpr");
		return NULL;
	}
	expr = (expr_type)CrArena_Alloc(arena, sizeof(*expr));
	if (!expr)
		return NULL;
	expr->kind = expr_kind::NamedExpr_kind;
	expr->v.NamedExpr.target = target;
	expr->v.NamedExpr.value = value;
	expr->lineno = lineno;
	expr->col_offset = col_offset;
	expr->end_lineno = end_lineno;
	expr->end_col_offset = end_col_offset;
	return expr;
}

expr_type CrAST_Lambda(arguments_type args, expr_type body, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	expr_type expr;
	if (!args) {
		CrError_SetString(CrExc_ValueError, "field 'args' is required for CrAST_Lambda");
		return NULL;
	}
	if (!body) {
		CrError_SetString(CrExc_ValueError, "field 'body' is required for CrAST_Lambda");
		return NULL;
	}
	expr = (expr_type)CrArena_Alloc(arena, sizeof(*expr));
	if (!expr)
		return NULL;
	expr->kind = expr_kind::Lambda_kind;
	expr->v.Lambda.args = args;
	expr->v.Lambda.body = body;
	expr->lineno = lineno;
	expr->col_offset = col_offset;
	expr->end_lineno = end_lineno;
	expr->end_col_offset = end_col_offset;
	return expr;
}

expr_type CrAST_IfExp(expr_type test, expr_type body, expr_type orelse, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	expr_type expr;
	if (!test) {
		CrError_SetString(CrExc_ValueError, "field 'test' is required for CrAST_IfExp");
		return NULL;
	}
	if (!body) {
		CrError_SetString(CrExc_ValueError, "field 'body' is required for CrAST_IfExp");
		return NULL;
	}
	if (!orelse) {
		CrError_SetString(CrExc_ValueError, "field 'orelse' is required for CrAST_IfExp");
		return NULL;
	}
	expr = (expr_type)CrArena_Alloc(arena, sizeof(*expr));
	if (!expr)
		return NULL;
	expr->kind = expr_kind::IfExp_kind;
	expr->v.IfExp.test = test;
	expr->v.IfExp.body = body;
	expr->v.IfExp.orelse = orelse;
	expr->lineno = lineno;
	expr->col_offset = col_offset;
	expr->end_lineno = end_lineno;
	expr->end_col_offset = end_col_offset;
	return expr;
}

expr_type CrAST_Dict(ast_expr_seq* keys, ast_expr_seq* values, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	expr_type expr;
	expr = (expr_type)CrArena_Alloc(arena, sizeof(*expr));
	if (!expr)
		return NULL;
	expr->kind = expr_kind::Dict_kind;
	expr->v.Dict.keys = keys;
	expr->v.Dict.values = values;
	expr->lineno = lineno;
	expr->col_offset = col_offset;
	expr->end_lineno = end_lineno;
	expr->end_col_offset = end_col_offset;
	return expr;
}

expr_type CrAST_Set(ast_expr_seq* elts, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	expr_type expr;
	expr = (expr_type)CrArena_Alloc(arena, sizeof(*expr));
	if (!expr)
		return NULL;
	expr->kind = expr_kind::Set_kind;
	expr->v.Set.elts = elts;
	expr->lineno = lineno;
	expr->col_offset = col_offset;
	expr->end_lineno = end_lineno;
	expr->end_col_offset = end_col_offset;
	return expr;
}

expr_type CrAST_ListComp(expr_type elt, ast_comprehension_seq* generators, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	expr_type expr;
	if (!elt) {
		CrError_SetString(CrExc_ValueError, "field 'elt' is required for CrAST_ListComp");
		return NULL;
	}
	expr = (expr_type)CrArena_Alloc(arena, sizeof(*expr));
	if (!expr)
		return NULL;
	expr->kind = expr_kind::ListComp_kind;
	expr->v.ListComp.elt = elt;
	expr->v.ListComp.generators = generators;
	expr->lineno = lineno;
	expr->col_offset = col_offset;
	expr->end_lineno = end_lineno;
	expr->end_col_offset = end_col_offset;
	return expr;
}

expr_type CrAST_SetComp(expr_type elt, ast_comprehension_seq* generators, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	expr_type expr;
	if (!elt) {
		CrError_SetString(CrExc_ValueError, "field 'elt' is required for CrAST_SetComp");
		return NULL;
	}
	expr = (expr_type)CrArena_Alloc(arena, sizeof(*expr));
	if (!expr)
		return NULL;
	expr->kind = expr_kind::SetComp_kind;
	expr->v.SetComp.elt = elt;
	expr->v.SetComp.generators = generators;
	expr->lineno = lineno;
	expr->col_offset = col_offset;
	expr->end_lineno = end_lineno;
	expr->end_col_offset = end_col_offset;
	return expr;
}

expr_type CrAST_DictComp(expr_type key, expr_type value, ast_comprehension_seq* generators, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	expr_type expr;
	if (!key) {
		CrError_SetString(CrExc_ValueError,
			"field 'key' is required for CrAST_DictComp");
		return NULL;
	}
	if (!value) {
		CrError_SetString(CrExc_ValueError,
			"field 'value' is required for CrAST_DictComp");
		return NULL;
	}
	expr = (expr_type)CrArena_Alloc(arena, sizeof(*expr));
	if (!expr)
		return NULL;
	expr->kind = expr_kind::DictComp_kind;
	expr->v.DictComp.key = key;
	expr->v.DictComp.value = value;
	expr->v.DictComp.generators = generators;
	expr->lineno = lineno;
	expr->col_offset = col_offset;
	expr->end_lineno = end_lineno;
	expr->end_col_offset = end_col_offset;
	return expr;
}

expr_type CrAST_GeneratorExp(expr_type elt, ast_comprehension_seq* generators, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	expr_type expr;
	if (!elt) {
		CrError_SetString(CrExc_ValueError, "field 'elt' is required for CrAST_GeneratorExp");
		return NULL;
	}
	expr = (expr_type)CrArena_Alloc(arena, sizeof(*expr));
	if (!expr)
		return NULL;
	expr->kind = expr_kind::GeneratorExp_kind;
	expr->v.GeneratorExp.elt = elt;
	expr->v.GeneratorExp.generators = generators;
	expr->lineno = lineno;
	expr->col_offset = col_offset;
	expr->end_lineno = end_lineno;
	expr->end_col_offset = end_col_offset;
	return expr;
}

expr_type CrAST_Await(expr_type value, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	expr_type expr;
	if (!value) {
		CrError_SetString(CrExc_ValueError, "field 'value' is required for CrAST_Await");
		return NULL;
	}
	expr = (expr_type)CrArena_Alloc(arena, sizeof(*expr));
	if (!expr)
		return NULL;
	expr->kind = expr_kind::Await_kind;
	expr->v.Await.value = value;
	expr->lineno = lineno;
	expr->col_offset = col_offset;
	expr->end_lineno = end_lineno;
	expr->end_col_offset = end_col_offset;
	return expr;
}

expr_type CrAST_Yield(expr_type value, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	expr_type expr;
	expr = (expr_type)CrArena_Alloc(arena, sizeof(*expr));
	if (!expr)
		return NULL;
	expr->kind = expr_kind::Yield_kind;
	expr->v.Yield.value = value;
	expr->lineno = lineno;
	expr->col_offset = col_offset;
	expr->end_lineno = end_lineno;
	expr->end_col_offset = end_col_offset;
	return expr;
}

expr_type CrAST_YieldFrom(expr_type value, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	expr_type expr;
	if (!value) {
		CrError_SetString(CrExc_ValueError, "field 'value' is required for CrAST_YieldFrom");
		return NULL;
	}
	expr = (expr_type)CrArena_Alloc(arena, sizeof(*expr));
	if (!expr)
		return NULL;
	expr->kind = expr_kind::YieldFrom_kind;
	expr->v.YieldFrom.value = value;
	expr->lineno = lineno;
	expr->col_offset = col_offset;
	expr->end_lineno = end_lineno;
	expr->end_col_offset = end_col_offset;
	return expr;
}

expr_type CrAST_Compare(expr_type left, ast_int_seq* ops, ast_expr_seq* comparators, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	expr_type expr;
	if (!left) {
		CrError_SetString(CrExc_ValueError, "field 'left' is required for CrAST_Compare");
		return NULL;
	}
	expr = (expr_type)CrArena_Alloc(arena, sizeof(*expr));
	if (!expr)
		return NULL;
	expr->kind = expr_kind::Compare_kind;
	expr->v.Compare.left = left;
	expr->v.Compare.ops = ops;
	expr->v.Compare.comparators = comparators;
	expr->lineno = lineno;
	expr->col_offset = col_offset;
	expr->end_lineno = end_lineno;
	expr->end_col_offset = end_col_offset;
	return expr;
}

expr_type CrAST_Call(expr_type func, ast_expr_seq* args, ast_keyword_seq* keywords, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	expr_type expr;
	if (!func) {
		CrError_SetString(CrExc_ValueError, "field 'func' is required for CrAST_Call");
		return NULL;
	}
	expr = (expr_type)CrArena_Alloc(arena, sizeof(*expr));
	if (!expr)
		return NULL;
	expr->kind = expr_kind::Call_kind;
	expr->v.Call.func = func;
	expr->v.Call.args = args;
	expr->v.Call.keywords = keywords;
	expr->lineno = lineno;
	expr->col_offset = col_offset;
	expr->end_lineno = end_lineno;
	expr->end_col_offset = end_col_offset;
	return expr;
}

expr_type CrAST_FormattedValue(expr_type value, int conversion, expr_type format_spec, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	expr_type expr;
	if (!value) {
		CrError_SetString(CrExc_ValueError, "field 'value' is required for CrAST_FormattedValue");
		return NULL;
	}
	expr = (expr_type)CrArena_Alloc(arena, sizeof(*expr));
	if (!expr)
		return NULL;
	expr->kind = expr_kind::FormattedValue_kind;
	expr->v.FormattedValue.value = value;
	expr->v.FormattedValue.conversion = conversion;
	expr->v.FormattedValue.format_spec = format_spec;
	expr->lineno = lineno;
	expr->col_offset = col_offset;
	expr->end_lineno = end_lineno;
	expr->end_col_offset = end_col_offset;
	return expr;
}

expr_type CrAST_JoinedStr(ast_expr_seq* values, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	expr_type expr;
	expr = (expr_type)CrArena_Alloc(arena, sizeof(*expr));
	if (!expr)
		return NULL;
	expr->kind = expr_kind::JoinedStr_kind;
	expr->v.JoinedStr.values = values;
	expr->lineno = lineno;
	expr->col_offset = col_offset;
	expr->end_lineno = end_lineno;
	expr->end_col_offset = end_col_offset;
	return expr;
}

expr_type CrAST_Constant(constant value, string kind, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	if (!value)
	{
		CrError_SetString(CrExc_ValueError, "field 'value' is required for CrAST_Constant");
		return NULL;
	}
	expr_type expr;
	expr = (expr_type)CrArena_Alloc(arena, sizeof(*expr));
	if (!expr)
		return NULL;
	expr->kind = expr_kind::Constant_kind;
	expr->v.Constant.value = value;
	expr->v.Constant.kind = kind;
	expr->lineno = lineno;
	expr->col_offset = col_offset;
	expr->end_lineno = end_lineno;
	expr->end_col_offset = end_col_offset;
	return expr;
}

expr_type CrAST_Attribute(expr_type value, identifier attr, expr_context_type ctx, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	expr_type expr;
	if (!value) {
		CrError_SetString(CrExc_ValueError, "field 'value' is required for CrAST_Attribute");
		return NULL;
	}
	if (!attr) {
		CrError_SetString(CrExc_ValueError, "field 'attr' is required for CrAST_Attribute");
		return NULL;
	}
	if (!ctx) {
		CrError_SetString(CrExc_ValueError, "field 'ctx' is required for CrAST_Attribute");
		return NULL;
	}
	expr = (expr_type)CrArena_Alloc(arena, sizeof(*expr));
	if (!expr)
		return NULL;
	expr->kind = expr_kind::Attribute_kind;
	expr->v.Attribute.value = value;
	expr->v.Attribute.attr = attr;
	expr->v.Attribute.ctx = ctx;
	expr->lineno = lineno;
	expr->col_offset = col_offset;
	expr->end_lineno = end_lineno;
	expr->end_col_offset = end_col_offset;
	return expr;
}

expr_type CrAST_Subscript(expr_type value, expr_type slice, expr_context_type ctx, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	expr_type expr;
	if (!value) {
		CrError_SetString(CrExc_ValueError, "field 'value' is required for CrAST_Subscript");
		return NULL;
	}
	if (!slice) {
		CrError_SetString(CrExc_ValueError, "field 'slice' is required for CrAST_Subscript");
		return NULL;
	}
	if (!ctx) {
		CrError_SetString(CrExc_ValueError, "field 'ctx' is required for CrAST_Subscript");
		return NULL;
	}
	expr = (expr_type)CrArena_Alloc(arena, sizeof(*expr));
	if (!expr)
		return NULL;
	expr->kind = expr_kind::Subscript_kind;
	expr->v.Subscript.value = value;
	expr->v.Subscript.slice = slice;
	expr->v.Subscript.ctx = ctx;
	expr->lineno = lineno;
	expr->col_offset = col_offset;
	expr->end_lineno = end_lineno;
	expr->end_col_offset = end_col_offset;
	return expr;
}

expr_type CrAST_Starred(expr_type value, expr_context_type ctx, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	expr_type expr;
	if (!value) {
		CrError_SetString(CrExc_ValueError, "field 'value' is required for CrAST_Starred");
		return NULL;
	}
	if (!ctx) {
		CrError_SetString(CrExc_ValueError, "field 'ctx' is required for CrAST_Starred");
		return NULL;
	}
	expr = (expr_type)CrArena_Alloc(arena, sizeof(*expr));
	if (!expr)
		return NULL;
	expr->kind = expr_kind::Starred_kind;
	expr->v.Starred.value = value;
	expr->v.Starred.ctx = ctx;
	expr->lineno = lineno;
	expr->col_offset = col_offset;
	expr->end_lineno = end_lineno;
	expr->end_col_offset = end_col_offset;
	return expr;
}

expr_type CrAST_Name(identifier id, expr_context_type ctx, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	expr_type expr;
	if (!id) {
		CrError_SetString(CrExc_ValueError, "field 'id' is required for CrAST_Name");
		return NULL;
	}
	if (!ctx) {
		CrError_SetString(CrExc_ValueError, "field 'ctx' is required for CrAST_Name");
		return NULL;
	}
	expr = (expr_type)CrArena_Alloc(arena, sizeof(*expr));
	if (!expr)
		return NULL;
	expr->kind = expr_kind::Name_kind;
	expr->v.Name.id = id;
	expr->v.Name.ctx = ctx;
	expr->lineno = lineno;
	expr->col_offset = col_offset;
	expr->end_lineno = end_lineno;
	expr->end_col_offset = end_col_offset;
	return expr;
}

expr_type CrAST_List(ast_expr_seq* elts, expr_context_type ctx, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	expr_type expr;
	if (!ctx) {
		CrError_SetString(CrExc_ValueError, "field 'ctx' is required for CrAST_List");
		return NULL;
	}
	expr = (expr_type)CrArena_Alloc(arena, sizeof(*expr));
	if (!expr)
		return NULL;
	expr->kind = expr_kind::List_kind;
	expr->v.List.elts = elts;
	expr->v.List.ctx = ctx;
	expr->lineno = lineno;
	expr->col_offset = col_offset;
	expr->end_lineno = end_lineno;
	expr->end_col_offset = end_col_offset;
	return expr;
}

expr_type CrAST_Tuple(ast_expr_seq* elts, expr_context_type ctx, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	expr_type expr;
	if (!ctx) {
		CrError_SetString(CrExc_ValueError, "field 'ctx' is required for CrAST_Tuple");
		return NULL;
	}
	expr = (expr_type)CrArena_Alloc(arena, sizeof(*expr));
	if (!expr)
		return NULL;
	expr->kind = expr_kind::Tuple_kind;
	expr->v.Tuple.elts = elts;
	expr->v.Tuple.ctx = ctx;
	expr->lineno = lineno;
	expr->col_offset = col_offset;
	expr->end_lineno = end_lineno;
	expr->end_col_offset = end_col_offset;
	return expr;
}

expr_type CrAST_Slice(expr_type lower, expr_type upper, expr_type step, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	expr_type expr;
	expr = (expr_type)CrArena_Alloc(arena, sizeof(*expr));
	if (!expr)
		return NULL;
	expr->kind = expr_kind::Slice_kind;
	expr->v.Slice.lower = lower;
	expr->v.Slice.upper = upper;
	expr->v.Slice.step = step;
	expr->lineno = lineno;
	expr->col_offset = col_offset;
	expr->end_lineno = end_lineno;
	expr->end_col_offset = end_col_offset;
	return expr;
}

comprehension_type CrAST_Comprehension(expr_type target, expr_type iter, ast_expr_seq* ifs, int is_async, CrArena* arena)
{
	comprehension_type comp;
	if (!target) {
		CrError_SetString(CrExc_ValueError, "field 'target' is required for CrAST_Comprehension");
		return NULL;
	}
	if (!iter) {
		CrError_SetString(CrExc_ValueError, "field 'iter' is required for CrAST_Comprehension");
		return NULL;
	}
	comp = (comprehension_type)CrArena_Alloc(arena, sizeof(*comp));
	if (!comp)
		return NULL;
	comp->target = target;
	comp->iter = iter;
	comp->ifs = ifs;
	comp->is_async = is_async;
	return comp;
}

excepthandler_type CrAST_ExceptHandler(expr_type type, identifier name, ast_stmt_seq* body, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	excepthandler_type exc;
	exc = (excepthandler_type)CrArena_Alloc(arena, sizeof(*exc));
	if (!exc)
		return NULL;
	exc->kind = ExceptHandler_kind;
	exc->v.ExceptHandler.type = type;
	exc->v.ExceptHandler.name = name;
	exc->v.ExceptHandler.body = body;
	exc->lineno = lineno;
	exc->col_offset = col_offset;
	exc->end_lineno = end_lineno;
	exc->end_col_offset = end_col_offset;
	return exc;
}

arguments_type CrAST_Arguments(ast_arg_seq* posonlyargs, ast_arg_seq* args, arg_type vararg, ast_arg_seq* kwonlyargs, ast_expr_seq* kw_defaults, arg_type kwarg, ast_expr_seq* defaults, CrArena* arena)
{
	arguments_type arguments;
	arguments = (arguments_type)CrArena_Alloc(arena, sizeof(*arguments));
	if (!arguments)
		return NULL;
	arguments->posonlyargs = posonlyargs;
	arguments->args = args;
	arguments->vararg = vararg;
	arguments->kwonlyargs = kwonlyargs;
	arguments->kw_defaults = kw_defaults;
	arguments->kwarg = kwarg;
	arguments->defaults = defaults;
	return arguments;
}

arg_type CrAST_Arg(identifier arg, expr_type annotation, string type_comment, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	arg_type a;
	if (!arg) {
		CrError_SetString(CrExc_ValueError, "field 'arg' is required for CrAST_Arg");
		return NULL;
	}
	a = (arg_type)CrArena_Alloc(arena, sizeof(*a));
	if (!a)
		return NULL;
	a->arg = arg;
	a->annotation = annotation;
	a->type_comment = type_comment;
	a->lineno = lineno;
	a->col_offset = col_offset;
	a->end_lineno = end_lineno;
	a->end_col_offset = end_col_offset;
	return a;
}

keyword_type CrAST_Keyword(identifier arg, expr_type value, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	keyword_type kwd;
	if (!value) {
		CrError_SetString(CrExc_ValueError, "field 'value' is required for CrAST_Keyword");
		return NULL;
	}
	kwd = (keyword_type)CrArena_Alloc(arena, sizeof(*kwd));
	if (!kwd)
		return NULL;
	kwd->arg = arg;
	kwd->value = value;
	kwd->lineno = lineno;
	kwd->col_offset = col_offset;
	kwd->end_lineno = end_lineno;
	kwd->end_col_offset = end_col_offset;
	return kwd;
}

alias_type CrAST_Alias(identifier name, identifier asname, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	alias_type alias;
	if (!name) {
		CrError_SetString(CrExc_ValueError, "field 'name' is required for CrAST_Alias");
		return NULL;
	}
	alias = (alias_type)CrArena_Alloc(arena, sizeof(*alias));
	if (!alias)
		return NULL;
	alias->name = name;
	alias->asname = asname;
	alias->lineno = lineno;
	alias->col_offset = col_offset;
	alias->end_lineno = end_lineno;
	alias->end_col_offset = end_col_offset;
	return alias;
}

withitem_type CrAST_WithItem(expr_type context_expr, expr_type optional_vars, CrArena* arena)
{
	withitem_type withitem;
	if (!context_expr) {
		CrError_SetString(CrExc_ValueError, "field 'context_expr' is required for CrAST_WithItem");
		return NULL;
	}
	withitem = (withitem_type)CrArena_Alloc(arena, sizeof(*withitem));
	if (!withitem)
		return NULL;
	withitem->context_expr = context_expr;
	withitem->optional_vars = optional_vars;
	return withitem;
}

match_case_type CrAST_MatchCase(pattern_type pattern, expr_type guard, ast_stmt_seq* body, CrArena* arena)
{
	match_case_type matchcase;
	if (!pattern) {
		CrError_SetString(CrExc_ValueError, "field 'pattern' is required for CrAST_MatchCase");
		return NULL;
	}
	matchcase = (match_case_type)CrArena_Alloc(arena, sizeof(*matchcase));
	if (!matchcase)
		return NULL;
	matchcase->pattern = pattern;
	matchcase->guard = guard;
	matchcase->body = body;
	return matchcase;
}

pattern_type CrAST_MatchValue(expr_type value, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	pattern_type pattern;
	if (!value) {
		CrError_SetString(CrExc_ValueError,	"field 'value' is required for CrAST_MatchValue");
		return NULL;
	}
	pattern = (pattern_type)CrArena_Alloc(arena, sizeof(*pattern));
	if (!pattern)
		return NULL;
	pattern->kind = MatchValue_kind;
	pattern->v.MatchValue.value = value;
	pattern->lineno = lineno;
	pattern->col_offset = col_offset;
	pattern->end_lineno = end_lineno;
	pattern->end_col_offset = end_col_offset;
	return pattern;
}

pattern_type CrAST_MatchSingleton(constant value, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	pattern_type pattern;
	if (!value) {
		CrError_SetString(CrExc_ValueError, "field 'value' is required for CrAST_MatchSingleton");
		return NULL;
	}
	pattern = (pattern_type)CrArena_Alloc(arena, sizeof(*pattern));
	if (!pattern)
		return NULL;
	pattern->kind = MatchSingleton_kind;
	pattern->v.MatchSingleton.value = value;
	pattern->lineno = lineno;
	pattern->col_offset = col_offset;
	pattern->end_lineno = end_lineno;
	pattern->end_col_offset = end_col_offset;
	return pattern;
}

pattern_type CrAST_MatchSequence(ast_pattern_seq* patterns, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	pattern_type pattern;
	pattern = (pattern_type)CrArena_Alloc(arena, sizeof(*pattern));
	if (!pattern)
		return NULL;
	pattern->kind = MatchSequence_kind;
	pattern->v.MatchSequence.patterns = patterns;
	pattern->lineno = lineno;
	pattern->col_offset = col_offset;
	pattern->end_lineno = end_lineno;
	pattern->end_col_offset = end_col_offset;
	return pattern;
}

pattern_type CrAST_MatchMapping(ast_expr_seq* keys, ast_pattern_seq* patterns, identifier rest, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	pattern_type pattern;
	pattern = (pattern_type)CrArena_Alloc(arena, sizeof(*pattern));
	if (!pattern)
		return NULL;
	pattern->kind = MatchMapping_kind;
	pattern->v.MatchMapping.keys = keys;
	pattern->v.MatchMapping.patterns = patterns;
	pattern->v.MatchMapping.rest = rest;
	pattern->lineno = lineno;
	pattern->col_offset = col_offset;
	pattern->end_lineno = end_lineno;
	pattern->end_col_offset = end_col_offset;
	return pattern;
}

pattern_type CrAST_MatchClass(expr_type cls, ast_pattern_seq* patterns, ast_identifier_seq* kwd_attrs, ast_pattern_seq* kwd_patterns, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	pattern_type pattern;
	if (!cls) {
		CrError_SetString(CrExc_ValueError, "field 'cls' is required for CrAST_MatchClass");
		return NULL;
	}
	pattern = (pattern_type)CrArena_Alloc(arena, sizeof(*pattern));
	if (!pattern)
		return NULL;
	pattern->kind = MatchClass_kind;
	pattern->v.MatchClass.cls = cls;
	pattern->v.MatchClass.patterns = patterns;
	pattern->v.MatchClass.kwd_attrs = kwd_attrs;
	pattern->v.MatchClass.kwd_patterns = kwd_patterns;
	pattern->lineno = lineno;
	pattern->col_offset = col_offset;
	pattern->end_lineno = end_lineno;
	pattern->end_col_offset = end_col_offset;
	return pattern;
}

pattern_type CrAST_MatchStar(identifier name, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	pattern_type p;
	p = (pattern_type)CrArena_Alloc(arena, sizeof(*p));
	if (!p)
		return NULL;
	p->kind = MatchStar_kind;
	p->v.MatchStar.name = name;
	p->lineno = lineno;
	p->col_offset = col_offset;
	p->end_lineno = end_lineno;
	p->end_col_offset = end_col_offset;
	return p;
}

pattern_type CrAST_MatchAs(pattern_type pattern, identifier name, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	pattern_type p;
	p = (pattern_type)CrArena_Alloc(arena, sizeof(*p));
	if (!p)
		return NULL;
	p->kind = MatchAs_kind;
	p->v.MatchAs.pattern = pattern;
	p->v.MatchAs.name = name;
	p->lineno = lineno;
	p->col_offset = col_offset;
	p->end_lineno = end_lineno;
	p->end_col_offset = end_col_offset;
	return p;
}

pattern_type CrAST_MatchOr(ast_pattern_seq* patterns, int lineno, int col_offset, int end_lineno, int end_col_offset, CrArena* arena)
{
	pattern_type pattern;
	pattern = (pattern_type)CrArena_Alloc(arena, sizeof(*pattern));
	if (!pattern)
		return NULL;
	pattern->kind = MatchOr_kind;
	pattern->v.MatchOr.patterns = patterns;
	pattern->lineno = lineno;
	pattern->col_offset = col_offset;
	pattern->end_lineno = end_lineno;
	pattern->end_col_offset = end_col_offset;
	return pattern;
}

type_ignore_type CrAST_TypeIgnore(int lineno, string tag, CrArena* arena)
{
	type_ignore_type type_ignore;
	if (!tag) {
		CrError_SetString(CrExc_ValueError, "field 'tag' is required for CrAST_TypeIgnore");
		return NULL;
	}
	type_ignore = (type_ignore_type)CrArena_Alloc(arena, sizeof(*type_ignore));
	if (!type_ignore)
		return NULL;
	type_ignore->kind = TypeIgnore_kind;
	type_ignore->v.TypeIgnore.lineno = lineno;
	type_ignore->v.TypeIgnore.tag = tag;
	return type_ignore;
}