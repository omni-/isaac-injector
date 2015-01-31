#include "stdafx.h"
#include "pipe.h"

HANDLE InitPipe(DWORD &error)
{
	HANDLE hPipe = INVALID_HANDLE_VALUE;

	DWORD start = GetTickCount();
	do
	{
		if (WaitNamedPipe(TEXT("\\\\.\\pipe\\omlpipe"), NMPWAIT_WAIT_FOREVER))
		{
			hPipe = CreateFile(TEXT("\\\\.\\pipe\\omlpipe"),
				GENERIC_READ | GENERIC_WRITE,
				FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL,
				OPEN_EXISTING,
				0,
				NULL);
			error = GetLastError();
		}
	}
	while ((hPipe == INVALID_HANDLE_VALUE) && (GetTickCount() - start < 500));

	if (hPipe != INVALID_HANDLE_VALUE)
	{
		DWORD mode = PIPE_READMODE_MESSAGE;
		SetNamedPipeHandleState(hPipe, &mode, NULL, NULL);
	}

	return hPipe;
}