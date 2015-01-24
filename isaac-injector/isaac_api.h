#pragma once
#include "stdafx.h"
#include "Externs.h"

void API_HPUp(Player* player, int amount);
void API_HPDown(Player* player, int amount);
void API_AddSoulHearts(Player* player, int amount);

void API_ShootTears(PointF* pos, PointF* velocity, int pattern, Entity* source);
Entity* API_SpawnEntity(int entityID, int variant, int subtype, float x, float y);

void API_Effect_GoodPill(Player* player);