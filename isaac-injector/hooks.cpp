#include "stdafx.h"
#include "hooks.h"
#include "ipc.h"
#include "Externs.h"
#include "detours.h"
#include "sigscan.h"
#include "signatures.h"

#include "isaac_api.h"

char* eventMasks[8];

/******************************************
************** TakePillEvent **************
*******************************************/

void* TakePillEvent_Original;

bool __fastcall TakePillEvent_Payload(Player* player, int pillID)
{
//Event Handling
	//IPC_SendEvent(PLAYER_EVENT_TAKEPILL, player, pillID);
	//IPC_RecieveEvent(PLAYER_EVENT_TAKEPILL, player, pillID);

	return true;
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
		test eax, eax
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
************** TakePillEvent **************
*******************************************/

void* AddCollectibleEvent_Original;

void __cdecl AddCollectibleEvent_Payload(Player* player, int a2, int itemid, int a4)
{
// Event Handling
	//IPC_SendEvent(PLAYER_EVENT_ADDCOLLECTIBLE, player, a2, itemid, a4);
	//IPC_RecieveEvent(PLAYER_EVENT_ADDCOLLECTIBLE, player, &a2, &itemid, &a4);
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

void __cdecl SpawnEntityEvent_Payload(PointF* velocity, PointF* position, int gameManager, signed int EntityID, int Variant, Entity* parent, int subtype, unsigned int seed)
{
// Event Handling
	//IPC_SendEvent(GAME_EVENT_SPAWNENTITY, zero, position, gameManager, EntityID, Variant, parent, subtype, seed);
	//IPC_RecieveEvent(GAME_EVENT_SPAWNENTITY, zero, position, gameManager, EntityID, Variant, parent, subtype, seed);
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
		//IPC_SendEvent(PLAYER_EVENT_HPUP, player, amount);
		//IPC_RecieveEvent(PLAYER_EVENT_HPUP, player, amount);
	}
	else if (amount < 0)
	{
		//IPC_SendEvent(PLAYER_EVENT_HPDOWN, player, amount);
		//IPC_RecieveEvent(PLAYER_EVENT_HPDOWN, player, amount);
	}

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
// Event handling
	//IPC_SendEvent(PLAYER_EVENT_ADDSOULHEARTS, player, amount);
	//IPC_RecieveEvent(PLAYER_EVENT_ADDSOULHEARTS, player, amount);

	return amount;
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
************* ShootTearsEvent *************
*******************************************/

void* ShootTearsEvent_Original;

void __cdecl ShootTearsEvent_Payload(PointF* direction, PointF* startpos, Entity* mob, int type, TearInfo* tearInfo)
{
// Event handling
	//IPC_SendEvent(ENEMY_EVENT_SHOOTTEARS, direction, startpos, mob, type, tearInfo);
	//IPC_RecieveEvent(ENEMY_EVENT_SHOOTTEARS, direction, startpos, mob, type, tearInfo);
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
// Event handling
	//IPC_SendEvent(GAME_EVENT_CHANGEROOM, roomMan, newRoomIdx);
	//IPC_RecieveEvent(GAME_EVENT_CHANGEROOM, roomMan, newRoomIdx);
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
*************** Functions *****************
*******************************************/

GoodPillEffectFuncType* GoodPillEffectFunc;
IsaacRandomFuncType* IsaacRandomFunc;
InitTearFuncType* InitTearFunc;
Player_TeleportFuncType* Player_TeleportFunc;

/******************************************
************** Initialization *************
*******************************************/

// p = player, but at the moment every p is just a pointer
void Hooks_InitEventMasks()
{
	eventMasks[PLAYER_EVENT_TAKEPILL] = "pi";
	eventMasks[PLAYER_EVENT_ADDCOLLECTIBLE] = "piii";
	eventMasks[PLAYER_EVENT_HPUP] = "pi";
	eventMasks[PLAYER_EVENT_HPDOWN] = "pi";
	eventMasks[PLAYER_EVENT_ADDSOULHEARTS] = "pi";
	eventMasks[GAME_EVENT_SPAWNENTITY] = "pppiipii";
	eventMasks[ENEMY_EVENT_SHOOTTEARS] = "pppip";
	eventMasks[GAME_EVENT_CHANGEROOM] = "pi";
}

void Hooks_HookEvents()
{
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
		Hooks_InitEventMasks();

		Hooks_GetPlayerManagerPtr();

		Hooks_GetFunctions();

		Hooks_HookEvents();

		return true;
	}
	else
		return false;
}