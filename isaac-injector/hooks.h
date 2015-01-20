#pragma once
#include "stdafx.h"
#include "detours.h"
#include "sigscan.h"
#include "Externs.h"

using AddCollectibleType = void __fastcall (Player*, int, int, int, int);
extern AddCollectibleType* Player_AddCollectible;
extern CSigScan AddCollectibleSig;

using PillCardActionType = void __fastcall (Player*, int);
extern PillCardActionType* Player_PillCardAction;
extern CSigScan PillCardActionSig;

using SpawnEntityType = char __cdecl (int, int, int, unsigned int);
extern SpawnEntityType* SpawnEntity;
extern CSigScan SpawnEntitySig;

using ModHealthType = int __cdecl (Player*, int);
extern ModHealthType* Player_ModHealth;
extern CSigScan ModHealthSig;

extern HANDLE hPipe;

char* getItemName(int ID);

void hookinit(HANDLE _hpipe);

void __fastcall AddCollectible_Hook(Player* player, int a2, int itemid, int a4, int a5);

void __fastcall PillCardAction_Payload(Player* player, int a);

void PillCardAction_Hook();

void __cdecl SpawnEntity_Payload(int a1, int a2, int a3, unsigned int a4);

char SpawnEntity_Hook();

int __cdecl ModHealth_Payload(Player* plyr, int amt);

int ModHealth_Hook(); // Player* player, int amount