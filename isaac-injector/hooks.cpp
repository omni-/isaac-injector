#include "stdafx.h"
#include "hooks.h"
#include "ipc.h"
#include "Externs.h"
#include "detours.h"
#include "sigscan.h"
#include "signatures.h"

#include "isaac_api.h"

char* eventMasks[6];
DWORD** PlayerManager;

/******************************************
************** TakePillEvent **************
*******************************************/

void* TakePillEvent_Original;

bool __fastcall TakePillEvent_Payload(Player* player, int pillID)
{
	// Testcode
	API_Effect_GoodPill(player);
	Entity* pooter = API_SpawnEntity(14, 0, 0, 3, 5);
	/*API_HPDown(player, 2);
	API_AddSoulHearts(player, 4);
	API_SpawnEntity(rand() % 7 + 46, 0, 0, 3, 5);
	API_SpawnEntity(5, 100, 169, 7, 5);*/

	API_SpawnEntity(pooter->_id, 0, 0, 7, 5);

	pooter->_tearType = 5;

	PointF pos;
	pos.x = 120;
	pos.y = 200;
	PointF velocity;
	velocity.x = 0.1;
	velocity.y = 0;
	API_ShootTears(&pos, &velocity, 4, pooter);
	//

	// Event Handling
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
		pop edi				// restore player
			pop esi         // restore pill
			jmp TakePillEvent_Original

		handled :
		pop edi				// restore player
			pop esi
			mov esi, 100    // skip pill code
			jmp TakePillEvent_Original
	}
}

/******************************************
************** TakePillEvent **************
*******************************************/

void* AddCollectibleEvent_Original;

void __cdecl AddCollectibleEvent_Payload(Player* player, int a2, int itemid, int a4)
{
	// Testcode
	player->_numCoins = 99;
	//

	// Event Handling
	IPC_SendEvent(PLAYER_EVENT_ADDCOLLECTIBLE, player, a2, itemid, a4);
	IPC_RecieveEvent(PLAYER_EVENT_ADDCOLLECTIBLE, player, &a2, &itemid, &a4);
	//std::ofstream outfile;
	//outfile.open("c:\\users\\cooper\\desktop\\log.txt");
	//outfile << "AddCollectible event sent and recieved." << std::endl;
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

void __cdecl SpawnEntityEvent_Payload(PointF* velocity, PointF* position, int gameManager, signed int EntityID, int Variant, int unknown_ptr, int subtype, unsigned int seed)
{
	// test code
	//FILE* f;

	//fopen_s(&f, "C:\\SpawnEntityEvent_Payload.txt", "a+");
	//fprintf(f, "a1.1: %f, a1.2: %f, a2.1: %f, a2.2: %f, manager: %p, EID: %d, Var: %d, unknown: %d, subtype: %d, seed: %p\n",
	//	zero->x, zero->y,
	//	position->x, position->y,
	//	gameManager,
	//	EntityID, Variant, unknown_ptr, subtype, seed);
	//fclose(f);
	//

	// Event Handling
	//IPC_SendEvent(GAME_EVENT_SPAWNENTITY, zero, position, gameManager, EntityID, Variant, unknown_ptr, subtype, seed);
	//IPC_RecieveEvent(GAME_EVENT_SPAWNENTITY, zero, position, gameManager, EntityID, Variant, unknown_ptr, subtype, seed);
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
	// Event handling
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

void __cdecl ShootTearsEvent_Payload(PointF* direction, PointF* startpos, Entity* mob, int typ, float a5)
{
	FILE* f;
	fopen_s(&f, "C:\\SpawnEntityEvent_Payload.txt", "a+");
		fprintf(f, "dir = %f / %f, pos = %f / %f, mob: %d/%d/%d, typ: %d, a5: %f\n", direction->x, direction->y, startpos->x, startpos->y, mob->_id, mob->_variant, mob->_subtype, typ, a5);
	fclose(f);
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
************* GoodPillEffect **************
*******************************************/

GoodPillEffectFuncType* GoodPillEffectFunc;

/******************************************
************** Isaac random ***************
*******************************************/

IsaacRandomFuncType* IsaacRandomFunc;

/******************************************
************** Initialization *************
*******************************************/

void Hooks_InitEventMasks()
{
	eventMasks[PLAYER_EVENT_TAKEPILL] = "pi";
	eventMasks[PLAYER_EVENT_ADDCOLLECTIBLE] = "piii";
	eventMasks[PLAYER_EVENT_HPUP] = "pi";
	eventMasks[PLAYER_EVENT_HPDOWN] = "pi";
	eventMasks[PLAYER_EVENT_ADDSOULHEARTS] = "pi";
	eventMasks[GAME_EVENT_SPAWNENTITY] = "pppiipii";
}

void Hooks_HookEvents()
{
	FILE* f;
	fopen_s(&f, "C:\Hooks_HookEvents.txt", "a+");
	
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

	//fprintf(f, "ShootTearsEvent_SigPtr= %p\n", ShootTearsEvent_SigPtr);

	fclose(f);
}

void Hooks_GetFunctions()
{
	IsaacRandomFunc = (IsaacRandomFuncType*)SigScan_FindSignature(&Signature_IsaacRandomFunc);
	GoodPillEffectFunc = (GoodPillEffectFuncType*)SigScan_FindSignature(&Signature_GoodPillEffectFunc);
}

void Hooks_GetPlayerManagerPtr()
{
	PlayerManager = (DWORD**)SigScan_FindSignature(&Signature_PlayerManager);
	PlayerManager = (DWORD**)((DWORD)PlayerManager + 2);
}

DWORD Hooks_GetPlayerManager()
{
	if (PlayerManager)
		return **PlayerManager;
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