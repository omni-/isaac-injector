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

#define FLOOR_ALTERNATE		0x80000000

#define FLOOR_BASEMENT1		0x01
//#define FLOOR_CELLAR1		0x01 | FLOOR_ALTERNATE
#define FLOOR_BASEMENT2		0x02
//#define FLOOR_CELAR2		0x02 | FLOOR_ALTERNATE
#define FLOOR_CAVES1		0x03
//#define FLOOR_CATACOMBS1	0x03 | FLOOR_ALTERNATE
#define FLOOR_CAVES2		0x04 
//#define FLOOR_CATACOMBS2	0x04 | FLOOR_ALTERNATE
#define FLOOR_DEPTHS1		0x05
//#define FLOOR_NECROPOLIS1	0x05 | FLOOR_ALTERNATE
#define FLOOR_DEPTHS2		0x06
//#define FLOOR_NECROPOLIS2	0x06 | FLOOR_ALTERNATE
#define FLOOR_WOMB1			0x07
//#define FLOOR_UTERO1        0x07 | FLOOR_ALTERNATE
#define FLOOR_WOMB2			0x08 
//#define FLOOR_UTERO2		0x08 | FLOOR_ALTERNATE
#define FLOOR_SHEOL			0x09
#define FLOOR_CATHEDRAL		0x09 | FLOOR_ALTERNATE
#define FLOOR_DARKROOM		0x0B
#define FLOOR_CHEST			0x0B | FLOOR_ALTERNATE


PlayerManager* API_GetPlayerManager();

// player stats
void API_HPUp(Player* player, int amount);
void API_AddSoulHearts(Player* player, int amount);
void API_AddBlackHearts(Player* player, int amount);
void API_GiveEternalHeart(Player* player);

// player
void API_AddCollectible(Player* player, int itemID);
int API_TeleportPlayer(int roomID);
bool API_PlayerHasItem(Player* player, int itemID);
void API_AddCostume(Player* player, Item* item);

// game
void API_GotoFloor(unsigned int floorNo);
void API_UpdateRoom();

void API_ChangePickupEntity(Entity* entity, int newID, int newVariant, int newSubtype, BOOL stuff);

// spawning
TearInfo* API_InitTear(int value, TearInfo* tear);
void API_ShootTears(PointF* pos, PointF* velocity, int pattern, TearInfo* tear, Entity* source);
void API_PoisonCloud(Entity* source, PointF* position, float damage);
void API_SpawnBlueFlies(Entity* owner, PointF* position, int amount);

Entity* API_SpawnEntity(int entityID, int variant, int subtype, float x, float y, Entity* parent);

// Status effects
void API_FreezeEntity(Entity* entity, int duration);
void API_FearEntity(Entity* entity, int duration);
void API_ConfuseEntity(Entity* entity, int duration);
void API_CharmEntity(Entity* entity, int duration);

// effects
void API_Effect_GoodPill(Player* player);
void API_Effect_BadPill(Player* player);

// misc
char* API_getItemName(int itemID);
bool API_IsEnemy(Entity* entity);
Item* API_GetItem(int itemID);