#include "stdafx.h"
#include "Externs.h"
#include "hooks.h"
#include "isaac_api.h"

void API_Effect_GoodPill(Player* player)
{
	GoodPillEffectFunc(player);
}

void API_HPUp(Player* player, int amount)
{
	//amount = amount * 2;
	_asm
	{
		mov eax, player
			mov ecx, amount
			call HpUpEvent_Hook
	}
}

void API_HPDown(Player* player, int amount)
{
	//amount = -amount * 2;
	_asm
	{
		mov eax, player
			mov ecx, amount
			call HpUpEvent_Hook
	}
}

void API_AddSoulHearts(Player* player, int amount)
{
	_asm
	{
		mov eax, player
			mov ecx, amount
			call AddSoulHeartsEvent_Hook
	}
}

void API_SpawnEntity(int entityID, int variant, int subtype, float x, float y)
{
	// zero
	PointF* zero = new PointF();
	zero->x = 0;
	zero->y = 0;
	// position
	PointF* pos = new PointF();
	pos->x = (x * 40) + 80;
	pos->y = (y * 40) + 160;
	// manager
	DWORD playerMan = Hooks_GetPlayerManager();

	unsigned int seed = IsaacRandomFunc();

	if (playerMan)
	{
		_asm
		{
			push seed
				push subtype
				push 0
				push variant
				push entityID
				push playerMan
				mov ebx, pos
				mov eax, zero
				call SpawnEntityEvent_Hook
		}
	}
}