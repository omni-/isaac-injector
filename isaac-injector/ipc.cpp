#include "stdafx.h"
#include "ipc.h"
#include "pipe.h"
#include "stdarg.h"
#include "hooks.h"

HANDLE hPipe;
HANDLE hMutex;

bool IPC_Init()
{
	DWORD error;
	hPipe = InitPipe(error);
	hMutex = ConnectMutex();
	if (error != 0)
	{
		char *s = (char*)malloc(10);
		sprintf(s, "err: %d", error);
		MessageBoxA(NULL, s, NULL, NULL);
		free(s);
	}
	return (error == 0) && (hMutex != 0) && (hPipe != INVALID_HANDLE_VALUE);
}

bool IPC_SendEvent(int eventID, ...)
{
	if ((hMutex != 0) && (hPipe != INVALID_HANDLE_VALUE))
	{
		if (WaitForSingleObject(hMutex, 100L) == WAIT_OBJECT_0)
		{
			va_list ap;
			va_start(ap, eventID);
			for (int i = 0; i < strlen(eventMasks[eventID]); i++)
			{
				int intArg = 0;
				char* strArg = NULL;
				float floatArg = 0.0;
				void* handleArg = NULL;

				DWORD cbWritten;

				WriteFile(hPipe, &eventID, sizeof(int), &cbWritten, NULL);
				switch (eventMasks[eventID][i])
				{
				case 'p':
					handleArg = va_arg(ap, void*);
					WriteFile(hPipe, &handleArg, sizeof(void*), &cbWritten, NULL);
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
				va_arg(ap, int);
			}
			va_end(ap);

			ReleaseMutex(hMutex);

			return true;
		}
		else
			return false;
	}
	else
		return false;
}