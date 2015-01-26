#pragma once
#include "stdafx.h"
#include "Externs.h"

#define TEARTYPE_ENEMY_RED		0
#define TEARTYPE_ENEMY_BONE		1
#define TEARTYPE_ENEMY_FLAME	2
#define TEARTYPE_ENEMY_PUKE		3
#define TEARTYPE_ENEMY_WHITE	4

#define SHOTPATTERN_SINGLE		0;
#define SHOTPATTERN_TRIPLE		2;
#define SHOTPATTERN_QUAD		4;

// player stats
void API_HPUp(Player* player, int amount);
void API_HPDown(Player* player, int amount);
void API_AddSoulHearts(Player* player, int amount);

// player
void API_AddCollectible(Player* player, int itemID);
int API_TeleportPlayer(int roomID);

// spawning
TearInfo* API_InitTear(int value, TearInfo* tear);
void API_ShootTears(PointF* pos, PointF* velocity, int pattern, TearInfo* tear, Entity* source);

Entity* API_SpawnEntity(int entityID, int variant, int subtype, float x, float y, Entity* parent);

// effects
void API_Effect_GoodPill(Player* player);

// misc
char* API_getItemName(int itemID);