#pragma once
#include "stdafx.h"
#include "Externs.h"

#define TEARTYPE_ENEMY_RED		0
#define TEARTYPE_ENEMY_BONE		1
#define TEARTYPE_ENEMY_FLAME	2
#define TEARTYPE_ENEMY_PUKE		3
#define TEARTYPE_ENEMY_WHITE	4

void API_HPUp(Player* player, int amount);
void API_HPDown(Player* player, int amount);
void API_AddSoulHearts(Player* player, int amount);

TearStruct* API_InitTear(int value, TearStruct* tear);
void API_ShootTears(PointF* pos, PointF* velocity, int pattern, TearStruct* tear, Entity* source);

Entity* API_SpawnEntity(int entityID, int variant, int subtype, float x, float y, Entity* parent);

void API_Effect_GoodPill(Player* player);

void API_AddCollectible(Player* player, int itemID);

int API_TeleportPlayer(int a1, int a2, void* a3, int a4);