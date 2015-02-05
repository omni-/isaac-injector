#include "stdafx.h"
#include "hooks.h"
#include "ipc.h"
#include "ipc_messages_event.h"
#include "Externs.h"
#include "detours.h"
#include "sigscan.h"
#include "signatures.h"

#include "isaac_api.h"

using namespace std;

/******************************************
************** UseCardEvent ***************
*******************************************/

void* UseCardEvent_Original;

BOOL __cdecl UseCardEvent_Payload(Player* player, int cardID)
{
	UseCardEvent_Notification notification(player, cardID);
	UseCardEvent_Response response(false);
	
	IPC_BeginEvent(&notification, sizeof(UseCardEvent_Notification));
	IPC_ProcessEvent();
	IPC_EndEvent(&response, sizeof(UseCardEvent_Response), IPC_EVENT_DEFAULT_TIMEOUT);

	return response.handled;
}

__declspec(naked) void UseCardEvent_Hook()
{
	// eax = cardID, ebx = player, save edi+ecx
	_asm
	{
		push ebx
		push ecx
		push edi
		push eax
			push eax
			push ebx
				call UseCardEvent_Payload
			add esp, 8
			test eax, eax
			jnz handled
		pop eax
		jmp not_handled
	handled:
		pop eax
		mov eax, 9999
	not_handled:
		pop edi
		pop ecx
		pop ebx
		jmp UseCardEvent_Original
	}
}

/******************************************
************** TakePillEvent **************
*******************************************/

void* TakePillEvent_Original;

bool __fastcall TakePillEvent_Payload(Player* player, int pillID)
{
	TakePillEvent_Response response(false);
	TakePillEvent_Notification notification(player, pillID);

	IPC_BeginEvent(&notification, sizeof(TakePillEvent_Notification));	
	IPC_ProcessEvent();
	IPC_EndEvent(&response, sizeof(TakePillEvent_Response), IPC_EVENT_DEFAULT_TIMEOUT);

	return response.handled;
}

__declspec(naked) void TakePillEvent_Hook()
{
	_asm
	{
		push esi		// save pill
		push edi        // save player

		mov ecx, edi
		mov edx, esi
			call TakePillEvent_Payload
		test al, al
		jnz handled

	not_handled :
		pop edi			// restore player
		pop esi         // restore pill
		jmp TakePillEvent_Original

	handled :
		pop edi			// restore player
		pop esi
		mov esi, 100    // skip pill code (set pillID to something greater than the max pill ID to skip pill effect handler)
		jmp TakePillEvent_Original
	}
}

/******************************************
*********** AddCollectibleEvent ***********
*******************************************/

void* AddCollectibleEvent_Original;

void __cdecl AddCollectibleEvent_Payload(Player* player, int a2, int itemid, int a4)
{
	AddCollectibleEvent_Notification notification(player, a2, itemid, a4);
	AddCollectibleEvent_Response response;
	
	IPC_BeginEvent(&notification, sizeof(AddCollectibleEvent_Notification));
	IPC_ProcessEvent();
	IPC_EndEvent(&response, sizeof(AddCollectibleEvent_Response), IPC_EVENT_DEFAULT_TIMEOUT);
}

__declspec(naked) void AddCollectibleEvent_Hook()
{
	_asm
	{
		push ebp
		mov ebp, esp
			push ecx
				push dword ptr[ebp + 0x10]
				push dword ptr[ebp + 0x0C]
				push dword ptr[ebp + 0x08]
				push ecx
					call AddCollectibleEvent_Payload
				add esp, 16
			pop ecx
		pop ebp
		jmp AddCollectibleEvent_Original
	}
}

/******************************************
************* SpawnEntityEvent ************
*******************************************/

void* SpawnEntityEvent_Original;

void __cdecl SpawnEntityEvent_Payload(PointF* velocity, PointF* position, PlayerManager* playerManager, int entityID, int variant, Entity* parent, int subtype, unsigned int seed)
{
	if (entityID <= 1000) //everything 1000+ is just effects. they cause needless lag.
	{
		SpawnEntityEvent_Notification notification(*velocity, *position, entityID, variant, subtype, parent);
		SpawnEntityEvent_Response response;

		IPC_BeginEvent(&notification, sizeof(SpawnEntityEvent_Notification));
		IPC_ProcessEvent();
		IPC_EndEvent(&response, sizeof(SpawnEntityEvent_Response), IPC_EVENT_DEFAULT_TIMEOUT);
	}
}

__declspec(naked) char SpawnEntityEvent_Hook()
{
	_asm
	{
		push ebp
		mov ebp, esp
			push eax
			push ebx
				push dword ptr[ebp + 0x1C]
				push dword ptr[ebp + 0x18]
				push dword ptr[ebp + 0x14]
				push dword ptr[ebp + 0x10]
				push dword ptr[ebp + 0x0C]
				push dword ptr[ebp + 0x08]
				push ebx
				push eax
					call SpawnEntityEvent_Payload
				add esp, 32
			pop ebx
			pop eax
		pop ebp
		jmp SpawnEntityEvent_Original
	}
}

/******************************************
**************** HpUpEvent ****************
*******************************************/

void* HpUpEvent_Original;

int __cdecl HpUpEvent_Payload(Player* player, int amount)
{
	if (amount > 0)
	{
		HpUpEvent_Notification notification(player, amount);
		HpUpEvent_Response response(amount);
	
		IPC_BeginEvent(&notification, sizeof(HpUpEvent_Notification));		
		IPC_ProcessEvent();
		IPC_EndEvent(&response, sizeof(HpUpEvent_Response), IPC_EVENT_DEFAULT_TIMEOUT);

		return response.amount;
	}
	else if (amount < 0)
	{
		HpDownEvent_Notification notification(player, -amount);
		HpDownEvent_Response response(-amount);
	
		IPC_BeginEvent(&notification, sizeof(HpDownEvent_Notification));
		IPC_ProcessEvent();
		IPC_EndEvent(&response, sizeof(HpDownEvent_Response), IPC_EVENT_DEFAULT_TIMEOUT);

		return -response.amount;
	}
	else
		return amount;
}

__declspec(naked) int HpUpEvent_Hook() // Player* player, int amount
{
	_asm
	{
		jecxz _original

		push eax // save player
			push ecx // push amount
			push eax // push player
				call HpUpEvent_Payload
			add esp, 8
		mov ecx, eax // ecx = amount from HpUp function
		pop eax      // eax = player

	_original :
		jmp HpUpEvent_Original
	}
}

/******************************************
************ AddSoulHeartsEvent ***********
*******************************************/

void* AddSoulHeartsEvent_Original;

int __fastcall AddSoulHeartsEvent_Payload(Player* player, int amount)
{
	AddSoulHeartsEvent_Notification notification(player, amount);
	AddSoulHeartsEvent_Response response(amount);
	
	IPC_BeginEvent(&notification, sizeof(AddSoulHeartsEvent_Notification));	
	IPC_ProcessEvent();
	IPC_EndEvent(&response, sizeof(AddSoulHeartsEvent_Response), IPC_EVENT_DEFAULT_TIMEOUT);

	return response.amount;
}

__declspec(naked) void AddSoulHeartsEvent_Hook()
{
	_asm
	{
		push eax	// save player
			mov edx, ecx
			mov ecx, eax
				call AddSoulHeartsEvent_Payload
			mov ecx, eax
		pop eax
		jmp AddSoulHeartsEvent_Original
	}
}

/******************************************
**************** VFSLoadFile **************
*******************************************/

void* VFSLoadFile_Original;

char* __cdecl VFSLoadFile_Payload(char* filename)
{
	char* result = filename;

	// Testcode:

/*	FILE* f;
	fopen_s(&f, "C:\\VFSLoadFile_Payload.txt", "a+");
	fprintf_s(f, "Loading: %s, sizeof(Player)= %d\n", filename, sizeof(Player));
	

	string* s = new string(filename);
	if (s->find("itempools.xml") != string::npos)
	{
		result = "resources/mods/rewritten/itempools.xml"; 
		fprintf_s(f, "redirected %s to %s\n", filename, result);
	}

	fclose(f);*/

	return result;
}

__declspec(naked) void VFSLoadFile_Hook()
{
	_asm
	{
		push ebx
			call VFSLoadFile_Payload
		add esp, 4
		mov ebx, eax
		jmp VFSLoadFile_Original
	}
}

/******************************************
************* ShootTearsEvent *************
*******************************************/

void* ShootTearsEvent_Original;

void __cdecl ShootTearsEvent_Payload(PointF* velocity, PointF* position, Entity* sourceEntity, int pattern, TearInfo* tearInfo)
{
	ShootTearsEvent_Notification notification(*velocity, *position, sourceEntity, pattern, *tearInfo);
	ShootTearsEvent_Response response;
	
	IPC_BeginEvent(&notification, sizeof(ShootTearsEvent_Notification));
	IPC_ProcessEvent();
	IPC_EndEvent(&response, sizeof(ShootTearsEvent_Response), IPC_EVENT_DEFAULT_TIMEOUT);
}

__declspec(naked) void ShootTearsEvent_Hook()
{
	_asm
	{
		push ebp
		mov ebp, esp
			push ecx
			push edx
				push dword ptr[ebp + 0x10]
				push dword ptr[ebp + 0x0C]
				push dword ptr[ebp + 0x08]
				push edx
				push ecx
					call ShootTearsEvent_Payload
				add esp, 20
			pop edx
			pop ecx
		pop ebp
		jmp ShootTearsEvent_Original
	}
}

/******************************************
************* ChangeRoomEvent *************
*******************************************/

void* ChangeRoomEvent_Original;

void __cdecl ChangeRoomEvent_Payload(RoomManager* roomMan, int newRoomIdx)
{
	ChangeRoomEvent_Notification notification(newRoomIdx);
	ChangeRoomEvent_Response response;
	
	IPC_BeginEvent(&notification, sizeof(ChangeRoomEvent_Notification));
	IPC_ProcessEvent();
	IPC_EndEvent(&response, sizeof(ChangeRoomEvent_Response), IPC_EVENT_DEFAULT_TIMEOUT);
}

__declspec(naked) void ChangeRoomEvent_Hook()
{
	_asm
	{
		push ebp
		mov ebp, esp
			push dword ptr[ebp + 0x0C]
			push dword ptr[ebp + 0x08]
				call ChangeRoomEvent_Payload
			add esp, 8
		pop ebp
		jmp ChangeRoomEvent_Original
	}
}

/******************************************
************* GameUpdateEvent *************
*******************************************/

void* GameUpdateEvent_Original;

void __cdecl GameUpdate_Payload()
{
	GameUpdateEvent_Notification notification;
	GameUpdateEvent_Response response;

	IPC_BeginEvent(&notification, sizeof(GameUpdateEvent_Notification));
	IPC_ProcessEvent();
	IPC_EndEvent(&response, sizeof(GameUpdateEvent_Response), IPC_EVENT_DEFAULT_TIMEOUT);
}

__declspec(naked) void GameUpdate_Hook()
{
	_asm
	{
		call GameUpdate_Payload
		jmp GameUpdateEvent_Original
	}
}

/******************************************
************* PlayerUpdateEvent ***********
*******************************************/

void* PlayerUpdateEvent_Original;

void __cdecl PlayerUpdateEvent_Payload(Player* player)
{
	PlayerUpdateEvent_Notification notification(player);
	PlayerUpdateEvent_Response response;

	IPC_BeginEvent(&notification, sizeof(PlayerUpdateEvent_Notification));
	IPC_ProcessEvent();
	IPC_EndEvent(&response, sizeof(PlayerUpdateEvent_Response), IPC_EVENT_DEFAULT_TIMEOUT);
}

__declspec(naked) void PlayerUpdateEvent_Hook()
{
	_asm
	{
		push ecx
			push ecx
				call PlayerUpdateEvent_Payload
			add esp, 4
		pop ecx
		jmp PlayerUpdateEvent_Original
	}
}

/******************************************
************* GotoFloorEvent ***********
*******************************************/

void* GotoFloorEvent_Original;

void __cdecl GotoFloorEvent_Payload(BOOL showForgetMeNow, BOOL skipOneFloor, int a3)
{
	PlayerManager* pman = API_GetPlayerManager();
	int nextFloorNo = pman->_floorNo;
	if (skipOneFloor)
		nextFloorNo++;

	GotoFloorEvent_Notification notification(nextFloorNo);
	GotoFloorEvent_Response response;

	IPC_BeginEvent(&notification, sizeof(GotoFloorEvent_Notification));
	IPC_ProcessEvent();
	IPC_EndEvent(&response, sizeof(GotoFloorEvent_Response), IPC_EVENT_DEFAULT_TIMEOUT);
}

__declspec(naked) void GotoFloorEvent_Hook()
{
	_asm
	{
		push eax
		push ecx
		push edi
			push edi
			push ecx
			push eax
				call GotoFloorEvent_Payload
			add esp, 12
		pop edi
		pop ecx
		pop eax
		jmp GotoFloorEvent_Original
	}
}


/******************************************
*************** Functions *****************
*******************************************/

GoodPillEffectFuncType* GoodPillEffectFunc;
IsaacRandomFuncType* IsaacRandomFunc;
InitTearFuncType* InitTearFunc;
Player_TeleportFuncType* Player_TeleportFunc;

/******************************************
************** Initialization *************
*******************************************/

void Hooks_HookEvents()
{
	// UseCardEvent
	void* UseCardEvent_SigPtr = SigScan_FindSignature(&Signature_UseCardEvent);
	UseCardEvent_Original = DetourFunction(PBYTE(UseCardEvent_SigPtr), PBYTE(UseCardEvent_Hook));

	// TakePillEvent
	void* TakePillEvent_SigPtr = SigScan_FindSignature(&Signature_TakePillEvent);
	TakePillEvent_Original = DetourFunction(PBYTE(TakePillEvent_SigPtr), PBYTE(TakePillEvent_Hook));

	// AddCollectibleEvent
	void* AddCollectibleEvent_SigPtr = SigScan_FindSignature(&Signature_AddCollectibleEvent);
	AddCollectibleEvent_Original = DetourFunction(PBYTE(AddCollectibleEvent_SigPtr), PBYTE(AddCollectibleEvent_Hook));

	// SpawnEntityEvent
	void* SpawnEntityEvent_SigPtr = SigScan_FindSignature(&Signature_SpawnEntityEvent);
	SpawnEntityEvent_Original = DetourFunction(PBYTE(SpawnEntityEvent_SigPtr), PBYTE(SpawnEntityEvent_Hook));

	// HpUpEvent
	void* HpUpEvent_SigPtr = SigScan_FindSignature(&Signature_HpUpEvent);
	HpUpEvent_Original = DetourFunction(PBYTE(HpUpEvent_SigPtr), PBYTE(HpUpEvent_Hook));

	// AddSoulHeartsEvent
	void* AddSoulHeartsEvent_SigPtr = SigScan_FindSignature(&Signature_AddSoulHeartsEvent);
	AddSoulHeartsEvent_Original = DetourFunction(PBYTE(AddSoulHeartsEvent_SigPtr), PBYTE(AddSoulHeartsEvent_Hook));

	// ShootTearsEvent
	void* ShootTearsEvent_SigPtr = SigScan_FindSignature(&Signature_ShootTearsEvent);
	ShootTearsEvent_Original = DetourFunction(PBYTE(ShootTearsEvent_SigPtr), PBYTE(ShootTearsEvent_Hook));

	// ChangeRoomEvent
	void* ChangeRoomEvent_SigPtr = SigScan_FindSignature(&Signature_ChangeRoomEvent);
	ChangeRoomEvent_Original = DetourFunction(PBYTE(ChangeRoomEvent_SigPtr), PBYTE(ChangeRoomEvent_Hook));

	// VFSLoadFile
	void* VFSLoadFile_SigPtr = SigScan_FindSignature(&Signature_VFSLoadFile);
	VFSLoadFile_Original = DetourFunction(PBYTE(VFSLoadFile_SigPtr), PBYTE(VFSLoadFile_Hook));

	//GameUpdateEvent
	void* GameUpdateEvent_SigPtr = SigScan_FindSignature(&Signature_GameUpdate);
	GameUpdateEvent_Original = DetourFunction(PBYTE(GameUpdateEvent_SigPtr), PBYTE(GameUpdate_Hook));

	//PlayerUpdateEvent
	void* PlayerUpdateEvent_SigPtr = SigScan_FindSignature(&Signature_PlayerUpdate);
	PlayerUpdateEvent_Original = DetourFunction(PBYTE(PlayerUpdateEvent_SigPtr), PBYTE(PlayerUpdateEvent_Hook));

	// GotoFloorEvent
	void* GotoFloorEvent_SigPtr = SigScan_FindSignature(&Signature_GotoFloor);
	GotoFloorEvent_Original = DetourFunction(PBYTE(GotoFloorEvent_SigPtr), PBYTE(GotoFloorEvent_Hook));
}

void Hooks_GetFunctions()
{
	// misc
	IsaacRandomFunc = (IsaacRandomFuncType*)SigScan_FindSignature(&Signature_IsaacRandomFunc);
	InitTearFunc = (InitTearFuncType*)SigScan_FindSignature(&Signature_InitTearFunc);

	// effects
	GoodPillEffectFunc = (GoodPillEffectFuncType*)SigScan_FindSignature(&Signature_GoodPillEffectFunc);

	// player functions
	Player_TeleportFunc = (Player_TeleportFuncType*)SigScan_FindSignature(&Signature_PlayerTeleportEvent);  // could/should be hooked, too
}

DWORD** PlayerManagerPtr;

void Hooks_GetPlayerManagerPtr()
{
	PlayerManagerPtr = (DWORD**)SigScan_FindSignature(&Signature_PlayerManager);
	PlayerManagerPtr = (DWORD**)((DWORD)PlayerManagerPtr + 2);
}

PlayerManager* Hooks_GetPlayerManager()
{
	if (PlayerManagerPtr)
		return (PlayerManager*)**PlayerManagerPtr;
	else
		return NULL;
}

bool Hooks_Init()
{
	if (SigScan_GetImageInfo())
	{
		Hooks_GetPlayerManagerPtr();

		Hooks_GetFunctions();

		Hooks_HookEvents();

		return true;
	}
	else
		return false;
}