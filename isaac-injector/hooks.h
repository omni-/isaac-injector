#pragma once
#include "stdafx.h"
#include "Externs.h"

#define PLAYER_EVENT_TAKEPILL		0x00
#define PLAYER_EVENT_ADDCOLLECTIBLE 0x01
#define GAME_EVENT_SPAWNENTITY		0x02
#define PLAYER_EVENT_HPUP		    0x03
#define PLAYER_EVENT_HPDOWN		    0x04
#define PLAYER_EVENT_ADDSOULHEARTS  0x05

extern char* eventMasks[];

// Player/Game manager
extern DWORD** PlayerManager;

// Hooks
void TakePillEvent_Hook();
void AddCollectibleEvent_Hook();
char SpawnEntityEvent_Hook();
int HpUpEvent_Hook();
void AddSoulHeartsEvent_Hook();
void ShootTearsEvent_Hook();

// functions
using IsaacRandomFuncType = unsigned int __cdecl(void);
extern IsaacRandomFuncType* IsaacRandomFunc;

using GoodPillEffectFuncType = int __stdcall(Player*);
extern GoodPillEffectFuncType* GoodPillEffectFunc;

bool Hooks_Init();
DWORD Hooks_GetPlayerManager();