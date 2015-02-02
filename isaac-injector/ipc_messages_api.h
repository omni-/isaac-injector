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