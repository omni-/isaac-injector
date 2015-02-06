#include "stdafx.h"
#include "ipc.h"

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		LoadLibrary(L"dsound.dll");
		if (!IPC_Init())
			MessageBoxA(NULL, "ipc init failed", NULL, NULL);
		Hooks_Init();

		break;
	}
	return TRUE;
}