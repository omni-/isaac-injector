#include "stdafx.h"
#include "ipc.h"
#include "stdarg.h"
#include "hooks.h"
#include "isaac_api.h"
#include "ipc_messages_api.h"

HANDLE hEventPipe;
HANDLE hCallPipe;

HANDLE InitEventPipe()
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

HANDLE InitCallPipe()
{
	HANDLE result = CreateNamedPipe(TEXT("\\\\.\\pipe\\omlCallPipe"), 
		PIPE_ACCESS_DUPLEX | FILE_FLAG_WRITE_THROUGH, 
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE, 
		PIPE_UNLIMITED_INSTANCES, 65536, 65536, 0, NULL);

	if (!ConnectNamedPipe(result, NULL))
		result = INVALID_HANDLE_VALUE;

	return result;
}

bool IPC_Init()
{
	hEventPipe = InitEventPipe();
	hCallPipe = InitCallPipe();

	return (hEventPipe != INVALID_HANDLE_VALUE) && (hCallPipe != INVALID_HANDLE_VALUE);
}

unsigned int IPC_HandleAPICall(DWORD timeout)
{
	unsigned int resultID = APICALL_NULL;

	bool done = false;
	DWORD start = GetTickCount();
	DWORD br, ba, bl = 0;
	do
	{
		if ((PeekNamedPipe(hCallPipe, &resultID, sizeof(unsigned int), &br, &ba, &bl)) && (br > 0))
		{
			done = true;
			switch (resultID)
			{
				case APICALL_END:
					if (bl + sizeof(int) == sizeof(API_EndCall))
					{
						API_EndCall request;
						ReadFile(hCallPipe, &request, sizeof(API_EndCall), &br, NULL);

						API_EndResult response;
						WriteFile(hCallPipe, &response, sizeof(API_EndResult), &br, NULL);
					}
					break;

				case APICALL_HPUP:
					if (bl + sizeof(int) == sizeof(API_HpUpCall))
					{
						API_HpUpCall request;
						ReadFile(hCallPipe, &request, sizeof(API_HpUpCall), &br, NULL);

						API_HPUp(request.player, request.amount);

						API_HpUpResult response;
						WriteFile(hCallPipe, &response, sizeof(API_HpUpResult), &br, NULL);
					}
					break;
			}
		}
	}
	while ((GetTickCount() - start < timeout) && (!done));

	return resultID;
}

void IPC_ProcessEvent()
{
	unsigned int currCallID = APICALL_NULL;
	do
	{
		currCallID = IPC_HandleAPICall(IPC_API_DEFAULT_TIMEOUT);
		Sleep(1);
	}
	while (currCallID != APICALL_END);
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