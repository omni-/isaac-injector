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

#pragma pack(1)
struct Entity
{
char _unk0000[0xC];
	int _id;
	unsigned int _variant;
	int _subtype;
char _unk0018[0x10];
	void* Paralysis;
char _unk0021[0x48];
	int _tearType;
char _unk0020[0x23C];
	float _scaleX;
	float _scaleY;
char _unk000D[0x88];
	float dmg;  // no
	float dmg2; // no
char _unk000C[0xC8];
	PointF position;
char _unk0[0x350];
};

#pragma pack(1)
struct Player : Entity
{
	char _unk76C[0x3E4];
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
	char _unk900[0x6C];
	float _shotspeed;
	char _unkA00[0x08];
	float _damage;
	float _range;
	char _unkB00[0xBC];
	float _speed;
	float _luck;
	char _unkC00[0xB4];
	BOOL _items[0x15A];
	char _unkB78[0x1D88];
};

#pragma pack(1)
struct TearInfo
{
	float _stuff0;
	float _stuff4;
	float _shotheight;
	float _shotspeed_strange;
	float _shotspeed;
	float _damage;
	float _stuff24;
	float _stuff28;
	float _stuff32;
	float _stuff36;
	float _tearcolor_red;
	float _tearcolor_green;
	float _tearcolor_blue;
	float _tearcolor_alpha;
	float unknown[0x0F];
	int _type;
	float unknown2[0x11];
};

#pragma pack(1)
struct RoomManager
{
	char* unknown[0x5D9C];
	DWORD currRoomIndex;
	DWORD previousRoomIndex;
};

#pragma pack(1)
struct RoomInfo
{
	int unknown1;
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
	int unknown8;
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

#pragma pack(1)
struct PlayerManager
{
	char unknown[0x18];
	Room rooms[50]; // unknown size.. 50 for now
	char unknown2[0x3BB8];
	int RoomCount;
};
