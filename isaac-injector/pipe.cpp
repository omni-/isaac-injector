#include "stdafx.h"
#include "pipe.h"

HANDLE InitPipe(DWORD &error)
{
	HANDLE hPipe;
	DWORD dwWritten, dwRead;
	TCHAR  chBuf[BUFSIZE];
	BOOL fSuccess;

	hPipe = CreateFile(TEXT("\\\\.\\pipe\\omlpipe"),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	error = GetLastError();
	return hPipe;
}

HANDLE ConnectMutex()
{
	return OpenMutexA(MUTEX_ALL_ACCESS, FALSE, "omlmutex");
}