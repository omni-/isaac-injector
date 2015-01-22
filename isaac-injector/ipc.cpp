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
	/*if (error != 0)
	{
		char *s = (char*)malloc(10);
		sprintf(s, "err: %d", error);
		MessageBoxA(NULL, s, NULL, NULL);
		free(s);
	}*/
	return (hMutex != 0) && (hPipe != INVALID_HANDLE_VALUE);
}

bool IPC_SendEvent(int eventID, ...)
{
	bool IPC_Result = false;

	FILE* f;
	fopen_s(&f, "C:\\IPC_SendEvent.txt", "a+");

	fprintf(f, "Sending Event: %d...\n", eventID);

	if ((hMutex != 0) && (hPipe != INVALID_HANDLE_VALUE))
	{
		fprintf(f, "Pipe & Mutex OK\n");
		if (WaitForSingleObject(hMutex, 100L) == WAIT_OBJECT_0)
		{
			fprintf(f, "Access to Pipe successfull\n");

			char* buff;
			DWORD cbWritten = 0;
			WriteFile(hPipe, &eventID, sizeof(int), &cbWritten, NULL);
			fprintf(f, "eventID written (%d)\n", cbWritten);

			fprintf(f, "EventMaskSize: (%d)\n", strlen(eventMasks[eventID]));

			va_list ap;
			va_start(ap, eventID);
			for (int i = 0; i < strlen(eventMasks[eventID]); i++)
			{
				int intArg = 0;
				char* strArg = NULL;
				float floatArg = 0.0;
				Player* handleArg = NULL;

				switch (eventMasks[eventID][i])
				{
				case 'p':
					handleArg = va_arg(ap, Player*);
					fprintf(f, "%d: p-argument = %p\n", i, handleArg);

					buff = Player_serialize(handleArg, cbWritten);
					fprintf(f, "Serialized: %d Bytes\n", cbWritten);

					WriteFile(hPipe, buff, cbWritten, &cbWritten, NULL);
					fprintf(f, "Done (%d), (%d)\n", cbWritten, GetLastError());
					break;
				case 'i':
					intArg = va_arg(ap, int);
					fprintf(f, "%d: i-argument = %d\n", i, intArg);
					WriteFile(hPipe, &intArg, sizeof(int), &cbWritten, NULL);
					fprintf(f, "Done (%d), (%d)\n", cbWritten, GetLastError());
					break;
				case 'f':
					floatArg = va_arg(ap, float);
					fprintf(f, "%d: f-argument = %f\n", i, floatArg);
					WriteFile(hPipe, &floatArg, sizeof(float), &cbWritten, NULL);
					fprintf(f, "Done (%d), (%d)\n", cbWritten, GetLastError());
					break;
				case 's':
					strArg = va_arg(ap, char*);
					fprintf(f, "%d: s-argument = %s\n", i, strArg);
					WriteFile(hPipe, strArg, strlen(strArg), &cbWritten, NULL);
					fprintf(f, "Done (%d), (%d)\n", cbWritten, GetLastError());
					break;
				}
			}
			va_end(ap);

			ReleaseMutex(hMutex);

			fprintf(f, "Access released\n");

			IPC_Result = true;
		}
	}

	fprintf(f, "SendEvent finished\n");

	fclose(f);

	return IPC_Result;
}