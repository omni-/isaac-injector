#include "stdafx.h"
#include "windows.h"
#include "TlHelp32.h"
#include "ipc.h"

bool IsRunning(WCHAR* exe1, WCHAR* exe2)
{
	PROCESSENTRY32W proc;
	proc.dwSize = sizeof(PROCESSENTRY32W);

	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snap != INVALID_HANDLE_VALUE)
	{
		if (Process32First(snap, &proc))
		{
			do
			{
				if ((_wcsicmp(&proc.szExeFile[0], exe1)) || (_wcsicmp(&proc.szExeFile[0], exe2)))
					return true;
			}
			while (Process32Next(snap, &proc));
		}
		CloseHandle(snap);
	}

	return false;
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		//if (IsRunning(L"omnis modloader.vshost.exe", L"omnis modloader.exe"))
		{
			LoadLibrary(L"dsound.dll");
			if (IPC_Init())
				Hooks_Init();
		}

		break;
	}
	return TRUE;
}