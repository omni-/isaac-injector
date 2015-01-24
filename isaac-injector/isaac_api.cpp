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

Entity* API_SpawnEntity(int entityID, int variant, int subtype, float x, float y)
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
			push 0
			push variant
			push entityID
			push playerMan
			mov ebx, pos
			mov eax, velocity
			call SpawnEntityEvent_Hook
		}
	}
}

//4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 84, 96, 100, 104, 108, 112, 116(Type)
struct TearType
{
	float unknown[0x1D];
	/*
	float _stuff0;
	float _stuff4;
	float _stuff8;
	float _stuff12;
	float _stuff16;
	float _stuff20;
	float _stuff24;
	float _stuff28;
	float _stuff32;
	float _stuff36;
	float _stuff40;
	char unknown[0x48];*/
	int _type;
};

void API_ShootTears(PointF* pos, PointF* velocity, int pattern, Entity* source)
{
	//(PointF* direction, PointF* startpos, Entity* mob, int typ, float a5)
	TearType* tears = new TearType();
	
	for (int i=0; i < sizeof(tears->unknown) / 4; i++)
	{
		tears->unknown[i] = 0.3f;
	}
	tears->_type = 5;

	_asm
	{
		push tears
		push pattern
		push source
		mov edx, pos
		mov ecx, velocity
		call ShootTearsEvent_Hook
	}
}