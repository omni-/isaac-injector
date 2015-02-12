#pragma once
#include "hooks.h"

#define APICALL_NULL           0x00

#define APICALL_HPUP           0x01
#define APICALL_ADDSOULHEARTS  0x02
#define APICALL_GETKEYS        0x03
#define APICALL_SETKEYS        0x04
#define APICALL_GETBOMBS       0x05
#define APICALL_SETBOMBS       0x06
#define APICALL_GETCOINS       0x07
#define APICALL_SETCOINS       0x08
#define APICALL_GETTRINKET     0x09
#define APICALL_SETTRINKET     0x0A
#define APICALL_ADDCOLLECTIBLE 0x0B
#define APICALL_HASITEM        0x0C
#define APICALL_GETPOSITION    0x0D
#define APICALL_SETPOSITION    0x0E
#define APICALL_TELEPORT       0x0F
#define APICALL_GETSTAT		   0x10
#define APICALL_SETSTAT		   0x11
#define APICALL_SPAWNENTITY	   0x12
#define APICALL_GOTOFLOOR	   0x13
#define APICALL_GETCUSTOMITEMS 0x14
#define APICALL_ADDCUSTOMITEM  0x15

#define APICALL_END            0xFFFFFFFF

#pragma pack(1)
struct API_EndCall
{
	unsigned int id = APICALL_END;
};

#pragma pack(1)
struct API_EndResult
{
	unsigned int id = APICALL_END;
};

#pragma pack(1)
struct API_HpUpCall
{
	unsigned int id = APICALL_HPUP;
	Player* player;
	int amount;
};

#pragma pack(1)
struct API_HpUpResult
{
	unsigned int id = APICALL_HPUP;
};

#pragma pack(1)
struct API_GetKeysCall
{
	unsigned int id = APICALL_GETKEYS;
	Player* player;
};

#pragma pack(1)
struct API_GetKeysResult
{
	unsigned int id = APICALL_GETKEYS;
	int amount;
};

#pragma pack(1)
struct API_SetKeysCall
{
	unsigned int id = APICALL_SETKEYS;
	Player* player;
	int amount;
};

#pragma pack(1)
struct API_SetKeysResult
{
	unsigned int id = APICALL_SETKEYS;
};

#pragma pack(1)
struct API_GetBombsCall
{
	unsigned int id = APICALL_GETBOMBS;
	Player* player;
};

#pragma pack(1)
struct API_GetBombsResult
{
	unsigned int id = APICALL_GETBOMBS;
	int amount;
};

#pragma pack(1)
struct API_SetBombsCall
{
	unsigned int id = APICALL_SETBOMBS;
	Player* player;
	int amount;
};

#pragma pack(1)
struct API_SetBombsResult
{
	unsigned int id = APICALL_SETBOMBS;
};

#pragma pack(1)
struct API_GetCoinsCall
{
	unsigned int id = APICALL_GETCOINS;
	Player* player;
};

#pragma pack(1)
struct API_GetCoinsResult
{
	unsigned int id = APICALL_GETCOINS;
	int amount;
};

#pragma pack(1)
struct API_SetCoinsCall
{
	unsigned int id = APICALL_SETCOINS;
	Player* player;
	int amount;
};

#pragma pack(1)
struct API_SetCoinsResult
{
	unsigned int id = APICALL_SETCOINS;
};


#pragma pack (1)
struct API_GetStatCall
{
	unsigned int id = APICALL_GETSTAT;
	Player* player;
	int stat;
};

#pragma pack (1)
struct API_GetStatResult
{
	unsigned int id = APICALL_GETSTAT;
	int amount;
};

#pragma pack (1)
struct API_SetStatCall
{
	unsigned int id = APICALL_SETSTAT;
	Player* player;
	int amount;
	int stat;
};

#pragma pack (1)
struct API_SetStatResult
{
	unsigned int id = APICALL_SETSTAT;
};

#pragma pack (1)
struct API_SpawnEntityCall
{
	unsigned int id = APICALL_SPAWNENTITY;
	int entityID, variant, subtype;
	float x, y;
	Entity* parent;
};

#pragma pack (1)
struct API_SpawnEntityResult
{
	unsigned int id = APICALL_SPAWNENTITY;
	Entity* entity;
};

#pragma pack(1)
struct API_GotoFloorCall
{
	unsigned int id = APICALL_GOTOFLOOR;
	unsigned floorNo;
};

#pragma pack(1)
struct API_GotoFloorResult
{
	unsigned int id = APICALL_GOTOFLOOR;
};

#pragma pack (1)
struct API_TeleportCall
{
	unsigned int id = APICALL_TELEPORT;
	int roomid;
};

#pragma pack(1)
struct API_TeleportResult
{
	unsigned int id = APICALL_TELEPORT;
};

#pragma pack (1)
struct API_GetPositionCall
{
	unsigned int id = APICALL_GETPOSITION;
	Player* player;
};

#pragma pack (1)
struct API_GetPositionResult
{
	unsigned int id = APICALL_GETPOSITION;
	PointF position;
};

#pragma pack (1)
struct API_SetPositionCall
{
	unsigned int id = APICALL_SETPOSITION;
	Player* player;
	PointF position;
};

#pragma pack (1)
struct API_SetPositionResult
{
	unsigned int id = APICALL_SETPOSITION;
};

#pragma pack (1)
struct API_AddCollectibleCall
{
	unsigned int id = APICALL_ADDCOLLECTIBLE;
	Player* player;
	int itemid;
};

#pragma pack (1)
struct API_AddCollectibleResult
{
	unsigned int id = APICALL_ADDCOLLECTIBLE;
};

#pragma pack (1)
struct API_HasItemCall
{
	unsigned int id = APICALL_HASITEM;
	Player* player;
	int itemid;
};

#pragma pack (1)
struct API_HasItemResult
{
	unsigned int id = APICALL_HASITEM;
	bool hasitem;
};

#pragma pack (1)
struct API_GetItemsCall
{
	unsigned int id = APICALL_GETCUSTOMITEMS;
};

#pragma pack (1)
struct API_GetItemsResult
{
	unsigned int id = APICALL_GETCUSTOMITEMS;
	int* ids;
};

#pragma pack (1)
struct API_AddItemCall
{
	unsigned int id = APICALL_ADDCUSTOMITEM;
	int itemid;
	char name[16];
	int type;
	char resourcename[32];

};

#pragma pack (1)
struct API_AddItemResult
{
	unsigned int id = APICALL_ADDCUSTOMITEM;
};