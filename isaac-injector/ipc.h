#pragma once
#include "stdafx.h"
#include "Windows.h"
#include "hooks.h"

#define IPC_EVENT_DEFAULT_TIMEOUT 100
#define IPC_API_DEFAULT_TIMEOUT 10

bool IPC_Init();

bool IPC_BeginEvent(void* eventMsg, size_t msgSize);
bool IPC_EndEvent(void* responseMsg, size_t msgSize, DWORD timeout);

// TakePillEvent
#pragma pack(1)
struct TakePillEvent_Notification
{
	int eventID = PLAYER_EVENT_TAKEPILL;
	void* playerHandle;
	int pillID;

	TakePillEvent_Notification(void* _player, int _pillID)
	{
		playerHandle = _player;
		pillID = _pillID;
	}
};

#pragma pack(1)
struct TakePillEvent_Response
{
	int eventID = PLAYER_EVENT_TAKEPILL;
	BOOL handled;

	TakePillEvent_Response(BOOL _handled)
	{
		handled = _handled;
	}
};

// AddCollectibleEvent
#pragma pack(1)
struct AddCollectibleEvent_Notification
{
	int eventID = PLAYER_EVENT_ADDCOLLECTIBLE;
	void* playerHandle;
	int a2;
	int itemID;
	int a4;

	AddCollectibleEvent_Notification(void* _player, int _a2, int _itemID, int _a4)
	{
		playerHandle = _player;
		a2 = _a2;
		itemID = _itemID;
		a4 = _a4;
	}
};

#pragma pack(1)
struct AddCollectibleEvent_Response
{
	int eventID = PLAYER_EVENT_ADDCOLLECTIBLE;
};

// SpawnEntityEvent
#pragma pack(1)
struct SpawnEntityEvent_Notification
{
	int eventID = GAME_EVENT_SPAWNENTITY;
	PointF velocity;
	PointF position;
	int entityID;
	int variant;
	int subtype;
	void* parentHandle;

	SpawnEntityEvent_Notification(PointF _velocity, PointF _position, int _entityID, int _variant, int _subtype, void* _parent)
	{
		velocity = _velocity;
		position = _position;
		entityID = _entityID;
		variant = _variant;
		subtype = _subtype;
		parentHandle = _parent;
	}
};

#pragma pack(1)
struct SpawnEntityEvent_Response
{
	int eventID = PLAYER_EVENT_ADDCOLLECTIBLE;
};

// HpUpEvent
#pragma pack(1)
struct HpUpEvent_Notification
{
	int eventID = PLAYER_EVENT_HPUP;
	void* playerHandle;
	int amount;

	HpUpEvent_Notification(void* _player, int _amount)
	{
		playerHandle = _player;
		amount = _amount;
	}
};

#pragma pack(1)
struct HpUpEvent_Response
{
	int eventID = PLAYER_EVENT_ADDCOLLECTIBLE;
	int amount;

	HpUpEvent_Response(int _amount)
	{
		amount = _amount;
	}
};

// HpDownEvent
#pragma pack(1)
struct HpDownEvent_Notification
{
	int eventID = PLAYER_EVENT_HPDOWN;
	void* playerHandle;
	int amount;

	HpDownEvent_Notification(void* _player, int _amount)
	{
		playerHandle = _player;
		amount = _amount;
	}
};

#pragma pack(1)
struct HpDownEvent_Response
{
	int eventID = PLAYER_EVENT_ADDCOLLECTIBLE;
	int amount;

	HpDownEvent_Response(int _amount)
	{
		amount = _amount;
	}
};

// AddSoulHeartsEvent
#pragma pack(1)
struct AddSoulHeartsEvent_Notification
{
	int eventID = PLAYER_EVENT_ADDSOULHEARTS;
	void* playerHandle;
	int amount;

	AddSoulHeartsEvent_Notification(void* _player, int _amount)
	{
		playerHandle = _player;
		amount = _amount;
	}
};

#pragma pack(1)
struct AddSoulHeartsEvent_Response
{
	int eventID = PLAYER_EVENT_ADDSOULHEARTS;
	int amount;

	AddSoulHeartsEvent_Response(int _amount)
	{
		amount = _amount;
	}
};

// ShootTearsEvent
#pragma pack(1)
struct ShootTearsEvent_Notification
{
	int eventID = ENEMY_EVENT_SHOOTTEARS;
	PointF velocity;
	PointF position;
	void* sourceEntityHandle;
	int pattern;
	TearInfo tearInfo;

	ShootTearsEvent_Notification(PointF _velocity, PointF _position, void* _sourceEntityHandle, int _pattern, TearInfo _tearInfo)
	{
		velocity = _velocity;
		position = _position;
		sourceEntityHandle = _sourceEntityHandle;
		pattern = _pattern;
		tearInfo = _tearInfo;
	}
};

#pragma pack(1)
struct ShootTearsEvent_Response
{
	int eventID = ENEMY_EVENT_SHOOTTEARS;
};

// ChangeRoomEvent
#pragma pack(1)
struct ChangeRoomEvent_Notification
{
	int eventID = PLAYER_EVENT_CHANGEROOM;
	int newRoomIndex;

	ChangeRoomEvent_Notification(int _newRoomIndex)
	{
		newRoomIndex = _newRoomIndex;
	}
};

#pragma pack(1)
struct ChangeRoomEvent_Response
{
	int eventID = PLAYER_EVENT_CHANGEROOM;
};

//GameUpdateEvent
#pragma pack(1)
struct GameUpdateEvent_Notification
{
	int eventID = GAME_EVENT_UPDATE;
};

#pragma pack(1)
struct GameUpdateEvent_Response
{
	int eventID = GAME_EVENT_UPDATE;
};

unsigned int IPC_HandleAPICall(DWORD timeout);
void IPC_ProcessEvent();
