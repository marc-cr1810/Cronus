#include "Cronus.h"

static void CrMain_Free()
{
	CrRuntime_Finalize();
}

static void CrMain_Header()
{
	printf("Cronus %s on %s\n", Cr_GetVersion(), PLATFORM);
}

static CrStatus CrMain_Init()
{
	CrStatus status;
	status = Cr_Initialize();

	if (CrStatus_Exception(status))
	{
		std::cout << "Error: " << status.error_msg << std::endl;
		return status;
	}

	return status;
}

// Run Cronus in interactive mode
static int Cr_RunStdin()
{
	std::ifstream file("<stdin>"); // make a blank file for stdin
	int run = CrRun_FileObject(&file, "<stdin>", false);
	return (run != 0);
}

static int Cr_RunCronus(int *exitcode)
{
	CrInterpreterState* interp = CrInterpreterState_Get();

	CrMain_Header();

	*exitcode = Cr_RunStdin();

	return 0;
}

int Cr_RunMain()
{
	CrStatus status = CrMain_Init();

	if (CrStatus_Exception(status))
	{
		CrMain_Free();
		std::cout << "Error: " << status.error_msg << std::endl;
		return status.exit_code;
	}

	int exitcode = 0;

	Cr_RunCronus(&exitcode);
	CrMain_Free();

	return exitcode;
}

int main(int argc, char** argv)
{
	return Cr_RunMain();
}