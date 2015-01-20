#include "stdafx.h"
#include "pipe.h"

HANDLE initPipe(DWORD &error)
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

unsigned char* awaitServerResponse(HANDLE hPipe)
{
	unsigned char* buffer;
	bool fSuccess;
	DWORD dwRead;
	do
	{
		// Read from the pipe. 
		fSuccess = ReadFile(
			hPipe,    // pipe handle 
			buffer,    // buffer to receive reply 
			BUFSIZE*sizeof(TCHAR),  // size of buffer 
			&dwRead,  // number of bytes read 
			NULL);    // not overlapped 

		if (!fSuccess && GetLastError() != ERROR_MORE_DATA)
			break;
	} while (!fSuccess);  // repeat loop if ERROR_MORE_DATA 
	return buffer;
}