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
					buff = Player_serialize(handleArg, cbWritten);
					WriteFile(hPipe, buff, cbWritten, &cbWritten, NULL);
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

	return IPC_Result;
}