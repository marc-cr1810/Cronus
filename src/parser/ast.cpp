#include "ast.h"

#include <core/mem.h>
#include <core/error.h>

ast_generic_seq* CrAST_NewGenericSeq(Cr_size_t size)
{
	ast_generic_seq* seq = NULL;
	size_t n = 0;

	// Check size is ok
	if (size < 0 ||
		(size && (((size_t)size - 1) > (SIZE_MAX / sizeof(void*)))))
	{
		CrError_NoMemory();
		return NULL;
	}
	n += sizeof(ast_generic_seq);
	seq = (ast_generic_seq*)Mem_Alloc(n);
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

ast_identifier_seq* CrAST_NewIdentifierSeq(Cr_size_t size)
{
	ast_identifier_seq* seq = NULL;
	size_t n = 0;

	// Check size is ok
	if (size < 0 ||
		(size && (((size_t)size - 1) > (SIZE_MAX / sizeof(void*)))))
	{
		CrError_NoMemory();
		return NULL;
	}
	n += sizeof(ast_identifier_seq);
	seq = (ast_identifier_seq*)Mem_Alloc(n);
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

ast_int_seq* CrAST_NewIntSeq(Cr_size_t size)
{
	ast_int_seq* seq = NULL;
	size_t n = 0;

	// Check size is ok
	if (size < 0 ||
		(size && (((size_t)size - 1) > (SIZE_MAX / sizeof(void*)))))
	{
		CrError_NoMemory();
		return NULL;
	}
	n += sizeof(ast_int_seq);
	seq = (ast_int_seq*)Mem_Alloc(n);
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