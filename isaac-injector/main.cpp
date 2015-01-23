#include "stdafx.h"
#include "ipc.h"

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		//std::ofstream outfile;
		//outfile.open("c:\\users\\cooper\\desktop\\log.txt");
		//outfile << "======================================" << std::endl;

		if (!IPC_Init())
			MessageBoxA(NULL, "ipc init failed", NULL, NULL);
		Hooks_Init();

		break;
	}
	return TRUE;
}