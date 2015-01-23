#include "stdafx.h"
#include "pipe.h"

HANDLE InitPipe(DWORD &error)
{
	HANDLE hPipe = INVALID_HANDLE_VALUE;
	DWORD dwWritten, dwRead;
	TCHAR  chBuf[BUFSIZE];
	BOOL fSuccess;

	DWORD start = GetTickCount();
	do 
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
	while ((hPipe == INVALID_HANDLE_VALUE) && (GetTickCount()-start < 500));

	ConnectNamedPipe(hPipe, NULL);

	return hPipe;
}