#include "stdafx.h"
#include "ipc.h"

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		std::ofstream outfile;
		outfile.open("c:\\ExeModdingLog.txt", std::ios_base::app);
		outfile << "======================================" << std::endl;

		IPC_Init();
		Hooks_Init();

		break;
	}
	return TRUE;
}