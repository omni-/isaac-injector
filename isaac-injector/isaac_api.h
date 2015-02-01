#pragma once
#include "stdafx.h"
#include "Externs.h"

#define TEARTYPE_ENEMY_RED		0
#define TEARTYPE_ENEMY_BONE		1
#define TEARTYPE_ENEMY_FLAME	2
#define TEARTYPE_ENEMY_PUKE		3
#define TEARTYPE_ENEMY_WHITE	4

#define SHOTPATTERN_SINGLE		0
#define SHOTPATTERN_DOUBLE		1
#define SHOTPATTERN_TRIPLE		2
#define SHOTPATTERN_TRIPLE2		3
#define SHOTPATTERN_QUAD		4
#define SHOTPATTERN_QUINTUPLE	5
#define SHOTPATTERN_PLUS		6
#define SHOTPATTERN_CROSS		7
#define SHOTPATTERN_CIRCLE		8

#define PLAYERSTAT_SPEED		0
#define PLAYERSTAT_RANGE		1
#define PLAYERSTAT_FIRERATE		2
#define	PLAYERSTAT_SHOTSPEED	3
#define PLAYERSTAT_DAMAGE		4
#define PLAYERSTAT_LUCK			5


PlayerManager* API_GetPlayerManager();

// player stats
void API_HPUp(Player* player, int amount);
void API_AddSoulHearts(Player* player, int amount);

// player
void API_AddCollectible(Player* player, int itemID);
int API_TeleportPlayer(int roomID);
bool API_PlayerHasItem(Player* player, int itemID);

// spawning
TearInfo* API_InitTear(int value, TearInfo* tear);
void API_ShootTears(PointF* pos, PointF* velocity, int pattern, TearInfo* tear, Entity* source);

Entity* API_SpawnEntity(int entityID, int variant, int subtype, float x, float y, Entity* parent);

// effects
void API_Effect_GoodPill(Player* player);

// misc
char* API_getItemName(int itemID);