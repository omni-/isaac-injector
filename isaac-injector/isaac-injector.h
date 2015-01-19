#include "stdafx.h"
#include "detours.h"
#include "sigscan.h"
#include "Externs.h"

using AddCollectibleType = void __fastcall (Player*, int, int, int, int);
AddCollectibleType* Player_AddCollectible;
CSigScan AddCollectibleSig;

using PillCardActionType = void __fastcall (Player*, int);
PillCardActionType* Player_PillCardAction;
CSigScan PillCardActionSig;

using SpawnEntityType = char __cdecl (int, int, int, unsigned int);
SpawnEntityType* SpawnEntity;
CSigScan SpawnEntitySig;

using ModHealthType = int __cdecl (Player*, int);
ModHealthType* Player_ModHealth;
CSigScan ModHealthSig;

void __fastcall AddCollectible_Hook(Player* player, int a2, int itemid, int a4, int a5)
{
	std::ofstream outfile;
	outfile.open("c:/users/cooper/desktop/log.txt", std::ios_base::app);
	outfile << "[info]picked up item: " << getItemName(itemid) << "(" << itemid << "), arg4: " << a4 << ", " << "arg5: " << a5 << std::endl;
	player->_numCoins = 99;
	return Player_AddCollectible(player, a2, itemid, a4, a5);
}

void __fastcall PillCardAction_Payload(Player* player, int a)
{
	std::ofstream outfile;
	outfile.open("c:/users/cooper/desktop/log.txt", std::ios_base::app);
	outfile << "[info]action pill/card triggered" << std::endl;
	player->_keys = 99;
}
__declspec(naked) void PillCardAction_Hook()
{
	_asm
	{
		push esi        // save pill
			push edi        // save player

			mov ecx, edi
			mov edx, esi
			call PillCardAction_Payload
			test eax, eax
			jnz handled

		not_handled :
		pop edi         // restore player
			pop esi         // restore pill
			jmp Player_PillCardAction

		handled :
		pop edi                 // restore player
			pop esi
			mov esi, 100    // skip pill code
			jmp Player_PillCardAction
	}
}
void __cdecl SpawnEntity_Payload(int a1, int a2, int a3, unsigned int a4)
{
	std::ofstream outfile;
	outfile.open("c:/users/cooper/desktop/log.txt", std::ios_base::app);
	outfile << "[info]SpawnEntity with args "<< a1 << ", " << a2 << ", " << a3 << ", " << a4 << std::endl;
}
__declspec(naked) char SpawnEntity_Hook()
{
	_asm
	{
		push ebp
			mov ebp, esp
			push eax
			push eax
			push dword ptr[ebp + 0x0C]
			push dword ptr[ebp + 0x10]
			push dword ptr[ebp + 0x14]
			call SpawnEntity_Payload
			add esp, 16
			pop eax
			pop ebp
			jmp SpawnEntity
	}
}
int __cdecl ModHealth_Payload(Player* plyr, int amt)
{
	std::ofstream outfile;
	outfile.open("c:/users/cooper/desktop/log.txt", std::ios_base::app);
	outfile << "[info]Health changed by: " << amt << std::endl;
	return amt; //* 2;
}
__declspec(naked) int ModHealth_Hook() // Player* player, int amount
{
	_asm
	{
		jecxz _original // HP-Up is also called with amount = 0 for every item isaac picks up

			push eax // save player

			push ecx // push amount
			push eax // push player
			call ModHealth_Payload
			add esp, 8

			mov ecx, eax // ecx = amount from HpUp function
			pop eax      // eax = player

		_original :
			jmp Player_ModHealth
	}
}
