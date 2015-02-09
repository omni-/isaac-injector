#pragma once
#include "stdafx.h"

#define ITEMTYPE_PASSIVE	   0x01
#define ITEMTYPE_TRINKET	   0x02
#define ITEMTYPE_ACTIVE		   0x03
#define ITEMTYPE_FAMILIAR	   0x04

// struct for easily printing values behind an unknown pointer
#pragma pack(1)
struct DebugStruct
{
	int unknown[40];
};

#pragma pack(1)
struct PointF
{
	float x;
	float y;
};

// Entity + 0xB9C	charName
#pragma pack(1)
struct Entity 
{
char _unk0000[0xC];
	int _id;
	unsigned int _variant;
	int _subtype;
	int _parentID;
	unsigned int _parentVariant;
char _unk0018[0x54];
	int _tearType;
	int _realItemID; // used as item ID for custom items
char _unk0020[0x238];
	float _scaleX;
	float _scaleY;
char _unk000C[0x138];
	int _statusEffectDuration;
char _unk000D[0x1C];
	PointF position;
char _unk0[0x350];
};

#pragma pack(1)
struct Player : Entity
{
	char _unk76C[0x3E4]; // 0xB50
	int _maxHearts;
	int _hearts;
	int _eternalHearts;
	int _soulHearts;
	int _blackHeartMask;
	int _jarHearts; // 0xB98
	int _keys;
	int _hasGoldenKey;
	int _numBombs;
	int _numCoins;
	char _unkB78[0x20];
	int _charID;
	char* _charname;
	char _unk900[0x44];
	float _shotspeed;
	int _firerate;
	char _unkA00[0x04];
	float _damage;
	float _range;
	char _unkB00[0xBC];// 89
	float _speed;  
	float _luck;
	char _unkB01[0x10];
	int _charges;
	char _unkC02[0x4];
	int _liftedItem;
	char _unkC01[0x8C];
	int _trinketID1;
	int _trinketID2;
	char _unkD01[0x04];
	BOOL _items[0x15A];
	char _unkD02[0x1CBC];
	int _pillCardID1;
	BOOL _isCard1;
	int _pillCardID2;
	BOOL _isCard2;
	char _unkE00[0xBC];
};

#pragma pack(1)
struct Item
{
	int _type; // 1 = passive(?), trinket = 2, active = 3, 4 = familiar
	int _id;
	char* _name;

	int unknown1;
	int unknown2;
	int unknown3;
	int unknown4;
	int unknown5;
	int unknown6;
	int _desc;
	int unknown8;
	int unknown9;
	int unknown10;
	int unknown11;
	int unknown12;
	int unknown13;

	char* _imageResourcePath;

	int unknown14;
	int unknown15;
	int unknown16;
	int unknown17;
	int unknown18;
	int _gfx;
	int _achievement;
	int _cache;
	int _maxhearts;
	int _hearts;
	int _soulhearts;
	int _blackhearts;
	int _bombs;
	int _keys;
	int _coins;
	int _maxcharges;
	int _cooldown;
	int _special;
	int _devilprice;
	int unknown33;

	char* _animationResourcePath;

	int unknown34;
	int unknown35;
	int unknown36;
	int unknown37;
	int unknown38;
	int unknown39;
	int unknown40;
	int unknown41;
	int unknown42;
	int unknown43;
	int unknown44;
	int unknown45;
};

struct ItemStorageArray
{
	Item* items[347];
};

#pragma pack(1)
struct TearInfo
{
	float unknown0;
	float unknown4;
	float shotheight;
	float shotspeed_strange;
	float shotspeed;
	float damage;
	float unknown24;
	float unknown28;
	float unknown32;
	float unknown36;
	float tearcolor_red;
	float tearcolor_green;
	float tearcolor_blue;
	float tearcolor_alpha;
	float unknown56[0x0F];
	int type;
	float unknown64[0x11];
};

#pragma pack(1)
struct RoomManager
{
	char unknown[0x5D9C];
	DWORD currRoomIndex;
	DWORD previousRoomIndex;
};

struct RoomEntityInfo
{
	int entityID;
	int unknown1;
	void* unknown2;
	int unknown3;
	void* unknown4;
	int unknown5;
	//int _test[30];
};

struct RoomEntity
{
	int unknown1;
	RoomEntityInfo* info;
	float unknown2;
	int unknown3;
};

struct RoomEntities
{
	RoomEntity entity[20];
};

#pragma pack(1)
struct RoomInfo
{
	int flags; // 101 (5) = Map + Compass, 111 = map + compass + blue map ?
	int type;
	int variant;
	char name[16];
	int unknown2;
	int unknown3;
	int unknown4;
	int difficulty;
	// pointer
	int unknown5;
	int unknown6;
	//
	int unknown7;
	// pointer
	RoomEntities* Entities;
	int unknown9;
	// 
	int unknown10;
	int unknown11;
};

#pragma pack(1)
struct Room
{
	RoomInfo* info;
	// ?
	int unknown1;
	// rooms
	int LeftRoomID;
	int TopRoomID;
	int RightRoomID;
	int BottomRoomID;
	int BottomLeftRoomID;
	int TopRightRoomID;
	int BottomRightRoomID;
	int BottomRightRoomID2;
	//	?
	int* unknown2;
	// zero (mostly)
	int unknown3;
	int unknown4;
	int unknown5;
	// pointers
	DebugStruct* unknown6;
	DebugStruct* unknown7;
	DebugStruct* unknown8;
	DebugStruct* unknown9;
	DebugStruct* unknown10;
	DebugStruct* unknown11;
	// zero
	int unknown12;
	// pointers
	DebugStruct* unknown13;
	DebugStruct* unknown14;
	DebugStruct* unknown15;
	// zero
	int unknown16;
	int unknown17;
	// pointers
	DebugStruct* unknown18; // something
	// zero
	int unknown19;
	int unknown20;
	// pointers
	DebugStruct* unknown21;
	DebugStruct* unknown22;
	DebugStruct* unknown23;
	// zero
	int unknown24;
	int unknown25;
	int unknown26;
	int unknown27;
	int unknown28;
	int unknown29;
	int unknown30;
	// -1 x 3
	int unknown31;
	int unknown32;
	int unknown33;
	// index
	int index;
};

// d0061D214 this is the current level structure

#pragma pack(1)
struct PlayerManager
{
	int _floorNo;
	bool _alternateFloor;
	char unknown1[0x03];
	int _curses;
	char unknown2[0x01];
	bool _seeForever;
	char unknown3[0x0A];
	Room _rooms[41]; // unknown size.. 41
	char unknown31[0x2D]; 
	char unknown4[0x4197];
	int _roomCount; 
	char unknown5[0x1074];
	Item* _items[346];
	// f23936 = floor Seed
	// f23920 = current seed
};
