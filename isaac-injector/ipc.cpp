#include "stdafx.h"
#include "ipc.h"
#include "pipe.h"
#include "stdarg.h"
#include "hooks.h"

HANDLE hEventPipe;
HANDLE hCallPipe;

bool IPC_Init()
{
	DWORD error;
	hEventPipe = InitPipe(error);

	return (hEventPipe != INVALID_HANDLE_VALUE);
}

bool IPC_BeginEvent(void* eventMsg, size_t msgSize)
{
	bool result = false;
	if ((hEventPipe != INVALID_HANDLE_VALUE) && (eventMsg != NULL) && (msgSize > 0))
	{
		DWORD bw = 0;
		result = WriteFile(hEventPipe, eventMsg, msgSize, &bw, NULL) && (bw == msgSize);
	}

	return result;
}

bool IPC_EndEvent(void* responseMsg, size_t msgSize, DWORD timeout)
{
	bool result = false;
	if ((hEventPipe != INVALID_HANDLE_VALUE) && (responseMsg != NULL) && (msgSize > 0))
	{
		DWORD startTime = GetTickCount();

		DWORD br = 0;
		DWORD ba = 0;
		DWORD bl = 0;
		do
		{
			if (PeekNamedPipe(hEventPipe, NULL, 0, &br, &ba, &bl) && (bl == msgSize))
			{
				result = ReadFile(hEventPipe, responseMsg, msgSize, &br, NULL) && (br == msgSize);
				break;
			}
		}
		while ((GetTickCount() - startTime) < timeout);
	}

	return result;
}