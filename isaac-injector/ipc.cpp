#include "stdafx.h"
#include "ipc.h"
#include "pipe.h"
#include "stdarg.h"
#include "hooks.h"

HANDLE hPipe;
HANDLE hMutex;

//////// The code contains some debug-message boxes because of the pipes not working properly at the moment ////////

bool IPC_Init()
{
	DWORD error;
	hPipe = InitPipe(error);
	Sleep(100);
	hMutex = CreateMutexA(NULL, FALSE, "omlmutex"); //OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "omlmutex");
	if (hMutex == 0)
		MessageBoxA(NULL, "OpenMutexA failed", NULL, NULL);

	return (hMutex != 0) && (hPipe != INVALID_HANDLE_VALUE);
}

bool IPC_SendEvent(int eventID, ...)
{
	bool IPC_Result = false;

	if ((hMutex != 0) && (hPipe != INVALID_HANDLE_VALUE))
	{
		if (WaitForSingleObject(hMutex, 100L) == WAIT_OBJECT_0)
		{
			DWORD cbWritten = 0;

			WriteFile(hPipe, &eventID, sizeof(int), &cbWritten, NULL);

			va_list ap;
			va_start(ap, eventID);
			for (int i = 0; i < strlen(eventMasks[eventID]); i++)
			{
				int intArg = 0;
				char* strArg = NULL;
				float floatArg = 0.0;
				Player* handleArg = NULL;

				char* buff;
				switch (eventMasks[eventID][i])
				{
				case 'p':
					handleArg = va_arg(ap, Player*);
					WriteFile(hPipe, handleArg, sizeof(Player), &cbWritten, NULL);
					break;
				case 'i':
					intArg = va_arg(ap, int);
					WriteFile(hPipe, &intArg, sizeof(int), &cbWritten, NULL);
					break;
				case 'f':
					floatArg = va_arg(ap, float);
					WriteFile(hPipe, &floatArg, sizeof(float), &cbWritten, NULL);
					break;
				case 's':
					strArg = va_arg(ap, char*);
					WriteFile(hPipe, strArg, strlen(strArg), &cbWritten, NULL);
					break;
				}
			}
			va_end(ap);

			ReleaseMutex(hMutex);

			IPC_Result = true;
		}
	}
	else if (hMutex == 0)
		MessageBoxA(NULL, "mutex is zero", NULL, NULL);
	else
		MessageBoxA(NULL, "invalid pipe handle", NULL, NULL);

	return IPC_Result;
}
bool IPC_RecieveEvent(int eventID, ...)
{
	bool IPC_Result = false;

	if ((hMutex != 0) && (hPipe != INVALID_HANDLE_VALUE))
	{
		if (WaitForSingleObject(hMutex, 100L) == WAIT_OBJECT_0)
		{
			DWORD cbWritten = 0;

			ReadFile(hPipe, &eventID, sizeof(int), &cbWritten, NULL);

			va_list ap;
			va_start(ap, eventID);
			for (int i = 0; i < strlen(eventMasks[eventID]); i++)
			{
				int* intArg = NULL;
				char* strArg = NULL;
				float* floatArg = NULL;
				Player* handleArg = NULL;

				char* buff;
				switch (eventMasks[eventID][i])
				{
				case 'p':
					handleArg = va_arg(ap, Player*);
					ReadFile(hPipe, handleArg, sizeof(Player), &cbWritten, NULL);
					break;
				case 'i':
					intArg = va_arg(ap, int*);
					ReadFile(hPipe, intArg, sizeof(int), &cbWritten, NULL);
					break;
				case 'f':
					floatArg = va_arg(ap, float*);
					ReadFile(hPipe, floatArg, sizeof(float), &cbWritten, NULL);
					break;
				case 's':
					strArg = va_arg(ap, char*);
					ReadFile(hPipe, strArg, strlen(strArg), &cbWritten, NULL);
					break;
				}
			}
			va_end(ap);

			ReleaseMutex(hMutex);

			IPC_Result = true;
		}
		else
			MessageBoxA(NULL, "mutex locked", NULL, NULL);
	}
	else if (hMutex == 0)
		MessageBoxA(NULL, "mutex is zero", NULL, NULL);
	else
		MessageBoxA(NULL, "invalid pipe handle", NULL, NULL);

	return IPC_Result;
}