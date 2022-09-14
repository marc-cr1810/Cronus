#pragma once

#include <port.h>

#include <parser/ast.h>
#include <parser/parser.h>

#include <objects/object.h>

/* AST node pairs */

typedef struct _cmpopexprpair {
	cmpop_type cmpop;
	expr_type expr;
} CmpopExprPair;

/* AST node pair functions */

CmpopExprPair* CrGen_CmpopExprPair(Parser* p, cmpop_type cmpop, expr_type expr);
ast_int_seq* CrGen_GetCmpops(Parser* p, ast_seq* seq);
ast_expr_seq* CrGen_GetExprs(Parser* p, ast_seq* seq);

/* Memoization functions */

void CrGen_Memo_ClearStats();
CrObject* CrGen_Memo_GetStats();
int CrGen_IsMemoized(Parser* p, int type, void* pres);

// Here, mark is the start of the node, while p->mark is the end.
// If node==NULL, they should be the same.
int CrGen_Memo_Insert(Parser* p, int mark, int type, void* node);
// Like CrGen_Memo_Insert(), but updates an existing node if found.
int CrGen_Memo_Update(Parser* p, int mark, int type, void* node);

/* Tokenizer functions */

int CrGen_FillToken(Parser* p);
Token* CrGen_ExpectToken(Parser* p, int type);
Token* CrGen_ExpectForcedToken(Parser* p, int type);
int CrGen_LookaheadWithInt(int positive, Token* (func)(Parser*, int), Parser* p, int arg);
int CrGen_Lookahead(int positive, void* (func)(Parser*), Parser* p);
Token* CrGen_GetLastNonWhitespaceToken(Parser* p);

void* CrGen_StringToken(Parser* p);
expr_type CrGen_NumberToken(Parser* p);
expr_type CrGen_NameToken(Parser* p);

/* AST Functions */

ast_seq* CrGen_SingletonSeq(Parser* p, void* a);
ast_seq* CrGen_SeqFlatten(Parser* p, ast_seq* seqs);

// Creates an `expr_ty` equivalent to `expr` but with `ctx` as context
expr_type CrGen_SetExprContext(Parser* p, expr_type expr, expr_context_type ctx);

ast_stmt_seq* CrGen_InteractiveExit(Parser* p);

mod_type CrGen_ASTFromFileObject(std::ifstream* fp, CrObject* filename, int mode, const char* ps1, const char* ps2, int* error_code, CrArena* arena);

/* Misc functions */

CrObject* CrGen_NewIdentifier(Parser* p, const char* n);
CrObject* CrGen_NewTypeComment(Parser* p, Token* tc);
