#include "stdafx.h";
#include "detours.h"
#include "sigscan.h"
#include "Externs.h"

using AddCollectibleType = void __fastcall (int, int, int, int, int);
AddCollectibleType* Player_AddCollectible;
CSigScan AddCollectibleSig;
using PillCardActionType = void __fastcall (int);
PillCardActionType* Player_PillCardAction;
CSigScan PillCardActionSig;

void __fastcall AddCollectible_Hook(int playerptr, int a2, int itemid, int a4, int a5)
{
	std::ofstream outfile;
	outfile.open("c:/users/cooper/desktop/log.txt", std::ios_base::app);
	outfile << "[info]successful hook! ARGS- pointer to player: " << playerptr << ", item: " << getItemName(itemid) << "(" << itemid << "), arg2, arg4, arg5: " << a2 << ", " << a4 << ", " << a5 << std::endl;
	Player* p = (Player*)playerptr;
	p->_numCoins = 99;
	p->_luck = 5;
	return Player_AddCollectible(playerptr, a2, itemid, a4, a5);
}

void __fastcall PillCardAction_Hook(int playerptr)
{
	std::ofstream outfile;
	outfile.open("c:/users/cooper/desktop/log.txt", std::ios_base::app);
	outfile << "[info]pill/card action activated" << std::endl;
	return Player_PillCardAction(playerptr);
}