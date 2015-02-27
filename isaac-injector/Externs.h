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

struct DebugStructF
{
	float unknown[40];
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
	char _unk76C[0x04];
	int _freezeframes;
	char _unk774[0x3DC]; // 0xB50
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
	/*[0x0B9C]*/            char* _charname;
	/*[0x0BA0]*/            char _unk0BA0[0x40];
	/*[0x0BE0]*/            int _tearrate;
	/*[0x0BE4]*/            float _shotspeed;
	/*[0x0BE8]*/            char _unk0BE8[0x4];
	/*[0x0BEC]*/            int _ntearsfired;
	/*[0x0BF0]*/            float _damage;
	/*[0x0BF4]*/            float _range;
	/*[0x0BF8]*/            float _tearheight;
	/*[0x0BFC]*/            char _unk0BFC[0xB8];
	/*[0x0CB4]*/            float _speed;
	/*[0x0CB8]*/            float _luck;
	/*[0x0CBC]*/            char _unk0CBC[0x10];
	/*[0x0CCC]*/            int _charges;
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
class Item
{
public:
	/*0x00*/        int _state;
	/*0x04*/        int _id;
	/*0x08*/        union {
		char *_pszName;
		char _szName[0x1C];
	};
	/*0x24*/        union {
		char *_pszDescription;
		char _szDescription[0x1C];
	};
	/*0x40*/        union {
		char *_pszResourcePath;
		char _szResourcePath[0x18];
	};
	/*0x58*/        int _gfx;
	/*0x5C*/        int _achievement;
	/*0x60*/        int _cache; // > 0 if locked?
	/*0x64*/        int _maxhearts;
	/*0x68*/        int _hearts;
	/*0x6C*/        int _soulhearts;
	/*0x70*/        int _blackhearts;
	/*0x74*/        int _bombs;
	/*0x78*/        int _keys;
	/*0x7C*/        int _coins;
	/*0x80*/        int _maxcharges;
	/*0x84*/        int _cooldown;
	/*0x88*/        int _special;
	/*0x8C*/        int _devilprice;
	/*0x90*/        int unknown0x90; // 5a locked ff
	/*0x94*/        union {
		char *_pszAnimationPath;
		char _szAnimationPath[0x1C];
	};
	/*0xB0*/        int unknown0xB0;
	/*0xB0*/        int unknown0xB4;
}; /*0xC8*/

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
	short variant;
	short subtype;
	int x1;
	int x2;
	int x3;
	int x4;
};

struct RoomEntityInfoArray
{
	RoomEntityInfo entities[20];
};

struct RoomEntity
{
	int unknown1;
	RoomEntityInfoArray* info;
	int unknown2;
	int unknown3;
};

struct RoomEntities
{
	RoomEntity entity[20];
};

#pragma pack(1)
struct STB_RoomInfo
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
	DebugStruct* unknown5;
	DebugStruct* unknown6;
	//
	int unknown7;
	// pointer
	RoomEntities* Entities;
	DebugStruct* unknown9;
	// 
	int unknown10;
//	int unknown11;
};

#pragma pack(1)
struct Room
{
	STB_RoomInfo* info;
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
struct CurrRoomInfoEx
{
	int unknown1;
	int unknown2;
	STB_RoomInfo* stb_roomInfo;

	int unknown[30];
};

#pragma pack(1)
struct CurrRoomInfo
{
	int unknown;
	CurrRoomInfoEx* currRoomInfo;
	int roomType;
	int roomWidth;
	int roomHeight;
	float unknown2;
	float unknown3;
	float unknown4;
	float unknown5; 
	int unknown6[941]; // 943
	Entity** entityArray;
	int kram;
	int entityCount;
};

#pragma pack(1)
struct CurrRoom
{
	CurrRoomInfo* info;
	int roomID;
	int nextRoomID; // always 84, teleports to the next room in walking direction or back one room if already at a dead end
	int unknown1;
	int fromDoor;
	int toDoor;
	float width;
	float height;

	int unknown[20];
};

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
	char unknown4[0x3EF3];
	void* _rooms2[169];
	int _roomCount;
	char unknown6[0x2C];
	CurrRoom _currRoom;
	// f23936 = floor Seed
	// f23920 = current seed
};
