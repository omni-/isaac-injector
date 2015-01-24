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
	amount = amount * 2;
	_asm
	{
		mov eax, player
		mov ecx, amount
		call HpUpEvent_Hook
	}
}

void API_HPDown(Player* player, int amount)
{
	amount = -amount * 2;
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

Entity* API_SpawnEntity(int entityID, int variant, int subtype, float x, float y, Entity* parent)
{
	// zero
	PointF* velocity = new PointF();
	velocity->x = 0;
	velocity->y = 0;
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
			push parent
			push variant
			push entityID
			push playerMan
			mov ebx, pos
			mov eax, velocity
			call SpawnEntityEvent_Hook
		}
	}
	else
		return NULL;
}

TearStruct* API_InitTear(int value, TearStruct* tear)
{
	_asm
	{
		mov ecx, value
		mov esi, tear
		call InitTearFunc
	}
}

void API_ShootTears(PointF* pos, PointF* velocity, int pattern, TearStruct* tear, Entity* source)
{
	//(PointF* direction, PointF* startpos, Entity* mob, int typ, float a5)
/*	TearStruct* tears = new TearStruct();
	
	for (int i=0; i < sizeof(tears->unknown) / 4; i++)
	{
		tears->unknown[i] = 0.3f;
	}
	tears->_type = 5;*/

	_asm
	{
		push tear
		push pattern
		push source
		mov edx, pos
		mov ecx, velocity
		call ShootTearsEvent_Hook
	}
}