#include "generator.h"

#include <core/error.h>

mod_type CrGen_ASTFromFileObject(std::ifstream* fp, CrObject* filename, int mode, const char* ps1, const char* ps2, int* error_code)
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

	Parser* p = CrParser_New(tok, mode, error_code);
	if (p == NULL)
		goto error;

	result = (mod_type)CrParser_Parse(p);
	CrParser_Free(p);
error:
	CrTokenizer_Free(tok);
	return result;
}
