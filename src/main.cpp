#include "Cronus.h"

int Cr_RunMain()
{
	int exitcode = 0;

	CrIntObject* int_obj = (CrIntObject*)CrIntObject_FromInt(22);
	CrStringObject* str_obj = (CrStringObject*)CrStringObject_FromString("Hello World!");
	CrFloatObject* flt_obj = (CrFloatObject*)CrFloatObject_FromDouble(3.14);

	return exitcode;
}

int main(int argc, char** argv)
{
	return Cr_RunMain();
}