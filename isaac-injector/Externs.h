#pragma once
#include "stdafx.h"

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
char _unk0018[0x5C];
	int _tearType;
char _unk0020[0x23C];
	float _scaleX;
	float _scaleY;
char _unk000D[0x158];
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
	int _jarHearts;
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
	int curses;
	int curses2;
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
	// f8 = curses (uint)  bitmap
	int _floorNo;
	bool _alternateFloor;
	char unknown1[0x08];
	bool _seeForever;
	char unknown2[0x0A];
	Room rooms[50]; // unknown size.. 50 for now
	char unknown3[0x3BB8];
	int RoomCount; 
	char unknown4[0x23D1];
	bool unknown5;
	// f23936 = floor Seed
	// f23920 = current seed
};
