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

				case APICALL_GETKEYS:
					if (bl + sizeof(int) == sizeof(API_GetKeysCall))
					{
						API_GetKeysCall request;
						ReadFile(hCallPipe, &request, sizeof(API_GetKeysCall), &br, NULL);

						API_GetKeysResult response;
						response.amount = request.player->_keys;
						WriteFile(hCallPipe, &response, sizeof(API_GetKeysResult), &br, NULL);
					}
					break;

				case APICALL_SETKEYS:
					if (bl + sizeof(int) == sizeof(API_SetKeysCall))
					{
						API_SetKeysCall request;
						ReadFile(hCallPipe, &request, sizeof(API_SetKeysCall), &br, NULL);

						request.player->_keys = request.amount;

						API_SetKeysResult response;
						WriteFile(hCallPipe, &response, sizeof(API_SetKeysResult), &br, NULL);
					}
					break;

				case APICALL_GETBOMBS:
					if (bl + sizeof(int) == sizeof(API_GetBombsCall))
					{
						API_GetBombsCall request;
						ReadFile(hCallPipe, &request, sizeof(API_GetBombsCall), &br, NULL);

						API_GetKeysResult response;
						response.amount = request.player->_numBombs;
						WriteFile(hCallPipe, &response, sizeof(API_GetBombsResult), &br, NULL);
					}
					break;

				case APICALL_SETBOMBS:
					if (bl + sizeof(int) == sizeof(API_SetBombsCall))
					{
						API_SetBombsCall request;
						ReadFile(hCallPipe, &request, sizeof(API_SetBombsCall), &br, NULL);

						request.player->_numBombs = request.amount;

						API_SetBombsResult response;
						WriteFile(hCallPipe, &response, sizeof(API_SetBombsResult), &br, NULL);
					}
					break;
				case APICALL_GETSTAT:
					if (bl + sizeof(int) == sizeof(API_GetStatCall))
					{
						API_GetStatCall request;
						ReadFile(hCallPipe, &request, sizeof(API_GetStatCall), &br, NULL);

						API_GetStatResult response;

						switch (request.stat)
						{
						case PLAYERSTAT_SPEED:
							response.amount = request.player->_speed;
							break;
						case PLAYERSTAT_RANGE:
							response.amount = request.player->_range;
							break;
						case PLAYERSTAT_FIRERATE:
							response.amount = request.player->_firerate;
							break;
						case PLAYERSTAT_SHOTSPEED:
							response.amount = request.player->_shotspeed;
							break;
						case PLAYERSTAT_DAMAGE:
							response.amount = request.player->_damage;
							break;
						case PLAYERSTAT_LUCK:
							response.amount = request.player->_luck;
							break;
						}
						WriteFile(hCallPipe, &response, sizeof(API_GetStatResult), &br, NULL);
					}
					break;
				case APICALL_SETSTAT:
					if (bl + sizeof(int) == sizeof(API_SetStatCall))
					{
						API_SetStatCall request;
						ReadFile(hCallPipe, &request, sizeof(API_SetStatCall), &br, NULL);

						switch (request.stat)
						{
						case PLAYERSTAT_SPEED:
							request.player->_speed = request.amount;
							break;
						case PLAYERSTAT_RANGE:
							request.player->_range = request.amount;
							break;
						case PLAYERSTAT_FIRERATE:
							request.amount = request.player->_firerate;
							break;
						case PLAYERSTAT_SHOTSPEED:
							request.player->_shotspeed = request.amount; 
							break;
						case PLAYERSTAT_DAMAGE:
							request.player->_damage = request.amount;
							break;
						case PLAYERSTAT_LUCK:
							request.player->_luck = request.amount;
							break;
						}
						API_SetStatResult response;
						WriteFile(hCallPipe, &response, sizeof(API_SetStatResult), &br, NULL);
					}
					break;
				case APICALL_SPAWNENTITY:
					if (bl + sizeof(int) == sizeof(API_SpawnEntityCall))
					{
						API_SpawnEntityCall request;
						ReadFile(hCallPipe, &request, sizeof(API_SpawnEntityCall), &br, NULL);

						Entity* entity = API_SpawnEntity(request.entityID, request.variant, request.subtype, request.x, request.y, request.parent);

						API_SpawnEntityResult response;
						response.entity = entity;
						WriteFile(hCallPipe, &response, sizeof(API_SpawnEntityResult), &br, NULL);
					}
					break;
				case APICALL_TELEPORT:
					if (bl + sizeof(int) == sizeof(API_TeleportCall))
					{
						API_TeleportCall request;
						ReadFile(hCallPipe, &request, sizeof(API_TeleportCall), &br, NULL);

						API_TeleportPlayer(request.roomid);

						API_TeleportResult response;
						WriteFile(hCallPipe, &response, sizeof(API_TeleportResult), &br, NULL);
					}
					break;
				default:
					MessageBoxA(NULL, "unknown bytecode", NULL, NULL);
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
