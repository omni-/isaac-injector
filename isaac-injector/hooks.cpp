#include "stdafx.h"
#include "hooks.h"
#include "ipc.h"
#include "ipc_messages_event.h"
#include "Externs.h"
#include "detours.h"
#include "sigscan.h"
#include "signatures.h"

#include "isaac_api.h"

using namespace std;

ItemStorageArray* itemStorageArray;
std::map<int, Item*> custom_items;
std::map<int, bool> has_custom_item;

/******************************************
************** UseCardEvent ***************
*******************************************/

void* UseCardEvent_Original;

BOOL __cdecl UseCardEvent_Payload(Player* player, int cardID)
{
	UseCardEvent_Notification notification(player, cardID);
	UseCardEvent_Response response(false);
	
	IPC_BeginEvent(&notification, sizeof(UseCardEvent_Notification));
	IPC_ProcessEvent();
	IPC_EndEvent(&response, sizeof(UseCardEvent_Response), IPC_EVENT_DEFAULT_TIMEOUT);

	return response.handled;
}

__declspec(naked) void UseCardEvent_Hook()
{
	// eax = cardID, ebx = player, save edi+ecx
	_asm
	{
		push ebx
		push ecx
		push edi
		push eax
			push eax
			push ebx
				call UseCardEvent_Payload
			add esp, 8
			test eax, eax
			jnz handled
		pop eax
		jmp not_handled
	handled:
		pop eax
		mov eax, 9999
	not_handled:
		pop edi
		pop ecx
		pop ebx
		jmp UseCardEvent_Original
	}
}

/******************************************
************** TakePillEvent **************
*******************************************/

void* TakePillEvent_Original;

bool __fastcall TakePillEvent_Payload(Player* player, int pillID)
{
	API_SpawnEntity(5,100,-1,3,1,NULL);
	API_AddBlackHearts(player, 3);
	//API_SpawnEntity(5,100,-2,4,1,NULL);

	TakePillEvent_Response response(false);
	TakePillEvent_Notification notification(player, pillID);

	IPC_BeginEvent(&notification, sizeof(TakePillEvent_Notification));	
	IPC_ProcessEvent();
	IPC_EndEvent(&response, sizeof(TakePillEvent_Response), IPC_EVENT_DEFAULT_TIMEOUT);

	return response.handled;
}

__declspec(naked) void TakePillEvent_Hook()
{
	_asm
	{
		push esi		// save pill
		push edi        // save player

		mov ecx, edi
		mov edx, esi
			call TakePillEvent_Payload
		test al, al
		jnz handled

	not_handled :
		pop edi			// restore player
		pop esi         // restore pill
		jmp TakePillEvent_Original

	handled :
		pop edi			// restore player
		pop esi
		mov esi, 100    // skip pill code (set pillID to something greater than the max pill ID to skip pill effect handler)
		jmp TakePillEvent_Original
	}
}

/******************************************
*********** AddCollectibleEvent ***********
*******************************************/

void* AddCollectibleEvent_Original;

void __cdecl AddCollectibleEvent_Payload(Player* player, int itemid, int charges, int a4)
{
	if (itemid == -1)
		API_AddSoulHearts(player, 4);
	if (itemid == -2)
		player->_damage = 30;

	AddCollectibleEvent_Notification notification(player, itemid, charges, a4);
	AddCollectibleEvent_Response response;
	
	IPC_BeginEvent(&notification, sizeof(AddCollectibleEvent_Notification));
	IPC_ProcessEvent();
	IPC_EndEvent(&response, sizeof(AddCollectibleEvent_Response), IPC_EVENT_DEFAULT_TIMEOUT);
}

__declspec(naked) void AddCollectibleEvent_Hook()
{
	_asm
	{
		push ebp
		mov ebp, esp
			push ecx
				push dword ptr[ebp + 0x10]
				push dword ptr[ebp + 0x0C]
				push dword ptr[ebp + 0x08]
				push ecx
					call AddCollectibleEvent_Payload
				add esp, 16
			pop ecx
		pop ebp
		jmp AddCollectibleEvent_Original
	}
}

/******************************************
************* SpawnEntityEvent ************
*******************************************/

void* SpawnEntityEvent_Original;

// returns subtype (for item spawning)
int __cdecl SpawnEntityEvent_Payload(PointF* velocity, PointF* position, PlayerManager* playerManager, int entityID, int variant, Entity* parent, int subtype, unsigned int seed)
{
	if (entityID <= 1000) //everything 1000+ is just effects. they cause needless lag.
	{
		SpawnEntityEvent_Notification notification(*velocity, *position, entityID, variant, subtype, parent);
		SpawnEntityEvent_Response response;

		IPC_BeginEvent(&notification, sizeof(SpawnEntityEvent_Notification));
		IPC_ProcessEvent();
		IPC_EndEvent(&response, sizeof(SpawnEntityEvent_Response), IPC_EVENT_DEFAULT_TIMEOUT);

		// custom item handling
		if ((entityID == 5) && (variant == 100) && (subtype < 0))
		{
			ofstream stream = ofstream("log.txt", ios_base::app);
			for each (auto item in custom_items)
			{
				stream << item.second->_name << ", " << subtype << endl;
			}
			itemStorageArray->items[235] = custom_items[subtype];
			return 235;
		}
		else
		{
			return subtype;
		}
	}
	else
		return subtype;
}

// not pretty.. should be changed somehow in the future
void* _retAddr = NULL;
int _tmpEntityID;
int _tmpVariant;
int _tmpSubType;

void __cdecl AfterSpawnEntityEvent_Payload(Entity* entitySpawned)
{
	if ((_tmpEntityID == 5) && (_tmpVariant == 100) && (_tmpSubType < 0))
	{
		entitySpawned->_realItemID = _tmpSubType;
	}
}

__declspec(naked) char SpawnEntityEvent_Hook()
{
	_asm
	{
		push ebp
		mov ebp, esp
			push eax
			push ebx
				// save tmp values
				push eax
					mov eax, dword ptr[ebp + 0x0C]
					mov _tmpEntityID, eax
					mov eax, dword ptr[ebp + 0x10]
					mov _tmpVariant, eax
					mov eax, dword ptr[ebp + 0x18]
					mov _tmpSubType, eax
				pop eax
				//
				push dword ptr[ebp + 0x1C]	// seed
				push dword ptr[ebp + 0x18]	// subtype
				push dword ptr[ebp + 0x14]	// parent
				push dword ptr[ebp + 0x10]	// variant
				push dword ptr[ebp + 0x0C]	// entityID
				push dword ptr[ebp + 0x08]	// playerManager
				push ebx					// position
				push eax					// velocity
					call SpawnEntityEvent_Payload
				add esp, 32
				mov dword ptr[ebp + 0x18], eax
			pop ebx
			pop eax
		pop ebp
		pop _retAddr
		call SpawnEntityEvent_Original

		// After SpawnEntity event
		push eax
			push eax
				call AfterSpawnEntityEvent_Payload
			add esp, 4
		pop eax
		push _retAddr
		ret
	}
}

/******************************************
**************** HpUpEvent ****************
*******************************************/

void* HpUpEvent_Original;

int __cdecl HpUpEvent_Payload(Player* player, int amount)
{
	if (amount > 0)
	{
		HpUpEvent_Notification notification(player, amount);
		HpUpEvent_Response response(amount);
	
		IPC_BeginEvent(&notification, sizeof(HpUpEvent_Notification));		
		IPC_ProcessEvent();
		IPC_EndEvent(&response, sizeof(HpUpEvent_Response), IPC_EVENT_DEFAULT_TIMEOUT);

		return response.amount;
	}
	else if (amount < 0)
	{
		HpDownEvent_Notification notification(player, -amount);
		HpDownEvent_Response response(-amount);
	
		IPC_BeginEvent(&notification, sizeof(HpDownEvent_Notification));
		IPC_ProcessEvent();
		IPC_EndEvent(&response, sizeof(HpDownEvent_Response), IPC_EVENT_DEFAULT_TIMEOUT);

		return -response.amount;
	}
	else
		return amount;
}

__declspec(naked) int HpUpEvent_Hook() // Player* player, int amount
{
	_asm
	{
		jecxz _original

		push eax // save player
			push ecx // push amount
			push eax // push player
				call HpUpEvent_Payload
			add esp, 8
		mov ecx, eax // ecx = amount from HpUp function
		pop eax      // eax = player

	_original :
		jmp HpUpEvent_Original
	}
}

/******************************************
************ AddSoulHeartsEvent ***********
*******************************************/

void* AddSoulHeartsEvent_Original;

int __fastcall AddSoulHeartsEvent_Payload(Player* player, int amount)
{
	if (amount != 0)
	{
		AddSoulHeartsEvent_Notification notification(player, amount);
		AddSoulHeartsEvent_Response response(amount);
	
		IPC_BeginEvent(&notification, sizeof(AddSoulHeartsEvent_Notification));	
		IPC_ProcessEvent();
		IPC_EndEvent(&response, sizeof(AddSoulHeartsEvent_Response), IPC_EVENT_DEFAULT_TIMEOUT);

		return response.amount;
	}
	else
		return amount;
}

__declspec(naked) void AddSoulHeartsEvent_Hook()
{
	_asm
	{
		push eax	// save player
			mov edx, ecx
			mov ecx, eax
				call AddSoulHeartsEvent_Payload
			mov ecx, eax
		pop eax
		jmp AddSoulHeartsEvent_Original
	}
}

/******************************************
************* PickupItemEvent *************
*******************************************/

void* CollideWithEntityEvent_Original;

void __cdecl CollideWithEntityEvent_Payload(Entity* collisionEntity, Player* player, int something)
{
	//if (pedestralToID.find(collisionEntity) != pedestralToID.end())
	if (collisionEntity->_realItemID < 0)
	{
		if (collisionEntity->_subtype == 235)
		{
			Item* tmpItem = custom_items[collisionEntity->_realItemID];
			itemStorageArray->items[235] = tmpItem;
		}
	}
}

__declspec(naked) void CollideWithEntityEvent_Hook()
{
	_asm
	{
		push ebp
		mov ebp, esp
			push ecx
				push dword ptr[ebp+0x0C]
				push dword ptr[ebp+0x08]
				push ecx
					call CollideWithEntityEvent_Payload
				add esp, 12
			pop ecx
		pop ebp
		jmp CollideWithEntityEvent_Original
	}
}

/******************************************
**************** VFSLoadFile **************
*******************************************/

void* VFSLoadFile_Original;

char* __cdecl VFSLoadFile_Payload(char* filename)
{
	//char* result = filename;

	//STOP USING PRINTF FOR THE LOVE OF GOD
	//if (filename == "resources/config/itempools.xml")
	//{
	//	filename = "resources/mods/config/itempools.xml";
	//}

	//if (filename == "resources/config/items.xml")
	//{
	//	result = "resources/mods/config/items.xml"; 
	//}
	return filename;
}

__declspec(naked) void VFSLoadFile_Hook()
{
	_asm
	{
		push ebx
			call VFSLoadFile_Payload
		add esp, 4
		mov ebx, eax
		jmp VFSLoadFile_Original
	}
}

/******************************************
************* ShootTearsEvent *************
*******************************************/

void* ShootTearsEvent_Original;

void __cdecl ShootTearsEvent_Payload(PointF* velocity, PointF* position, Entity* sourceEntity, int pattern, TearInfo* tearInfo)
{
	ShootTearsEvent_Notification notification(*velocity, *position, sourceEntity, pattern, *tearInfo);
	ShootTearsEvent_Response response;
	
	IPC_BeginEvent(&notification, sizeof(ShootTearsEvent_Notification));
	IPC_ProcessEvent();
	IPC_EndEvent(&response, sizeof(ShootTearsEvent_Response), IPC_EVENT_DEFAULT_TIMEOUT);
}

__declspec(naked) void ShootTearsEvent_Hook()
{
	_asm
	{
		push ebp
		mov ebp, esp
			push ecx
			push edx
				push dword ptr[ebp + 0x10]
				push dword ptr[ebp + 0x0C]
				push dword ptr[ebp + 0x08]
				push edx
				push ecx
					call ShootTearsEvent_Payload
				add esp, 20
			pop edx
			pop ecx
		pop ebp
		jmp ShootTearsEvent_Original
	}
}

/******************************************
************* ChangeRoomEvent *************
*******************************************/

void* ChangeRoomEvent_Original;

void __cdecl ChangeRoomEvent_Payload(RoomManager* roomMan, int newRoomIdx)
{
	FILE* f;
	fopen_s(&f, "C:\\ChangeRoomEvent_Payload.txt", "a+");
	PlayerManager* pman = API_GetPlayerManager();
	for (int i=0; i < pman->_roomCount; i++)
	{ 
		fprintf(f, "RoomID: %d, type: %d, variant: %d\n", pman->_rooms[i].LeftRoomID+1, pman->_rooms[i].info->type, pman->_rooms[i].info->variant);
		
	//	fprintf(f, "2=%d, 3=%d, 4=%d\n", pman->_rooms[i].info->unknown2, pman->_rooms[i].info->unknown3, pman->_rooms[i].info->unknown4);
		fprintf(f, "Roo: %d\n", ((int*)(&pman->_rooms[i].info->Entities->entity[0].info->entities))[-1]);
		for (int j=0; j < 20; j++)
		{
			//if (pman->_rooms[i].info->unknown6)
			fprintf(f, "%d, %d, %d, %d\n", pman->_rooms[i].info->Entities->entity[0].unknown1, pman->_rooms[i].info->Entities->entity[0].unknown2, pman->_rooms[i].info->Entities->entity[0].unknown3);
			fprintf(f, "entity[%d].ID/var/sub: %d/%d/%d, pos= %d, %d, %d, %d\n", j, pman->_rooms[i].info->Entities->entity[0].info->entities[j].entityID, pman->_rooms[i].info->Entities->entity[0].info->entities[j].variant, 
				pman->_rooms[i].info->Entities->entity[0].info->entities[j].subtype, pman->_rooms[i].info->Entities->entity[0].info->entities[j].x1, pman->_rooms[i].info->Entities->entity[0].info->entities[j].x2,
				pman->_rooms[i].info->Entities->entity[0].info->entities[j].x3, pman->_rooms[i].info->Entities->entity[0].info->entities[j].x4);
		//	fprintf(f, "bla[%d]= %p\n", j, ((int*)(&pman->_rooms[i].info->flags))[j]);
		}
	}

	fclose(f);

	ChangeRoomEvent_Notification notification(newRoomIdx);
	ChangeRoomEvent_Response response;
	
	IPC_BeginEvent(&notification, sizeof(ChangeRoomEvent_Notification));
	IPC_ProcessEvent();
	IPC_EndEvent(&response, sizeof(ChangeRoomEvent_Response), IPC_EVENT_DEFAULT_TIMEOUT);
}

__declspec(naked) void ChangeRoomEvent_Hook()
{
	_asm
	{
		push ebp
		mov ebp, esp
			push dword ptr[ebp + 0x0C]
			push dword ptr[ebp + 0x08]
				call ChangeRoomEvent_Payload
			add esp, 8
		pop ebp
		jmp ChangeRoomEvent_Original
	}
}

/******************************************
************* GameUpdateEvent *************
*******************************************/

void* GameUpdateEvent_Original;

void __cdecl GameUpdate_Payload()
{
	GameUpdateEvent_Notification notification;
	GameUpdateEvent_Response response;

	IPC_BeginEvent(&notification, sizeof(GameUpdateEvent_Notification));
	IPC_ProcessEvent();
	IPC_EndEvent(&response, sizeof(GameUpdateEvent_Response), IPC_EVENT_DEFAULT_TIMEOUT);
}

__declspec(naked) void GameUpdate_Hook()
{
	_asm
	{
		call GameUpdate_Payload
		jmp GameUpdateEvent_Original
	}
}

/******************************************
************* PlayerUpdateEvent ***********
*******************************************/

void* PlayerUpdateEvent_Original;

void __cdecl PlayerUpdateEvent_Payload(Player* player)
{
	PlayerUpdateEvent_Notification notification(player);
	PlayerUpdateEvent_Response response;

	IPC_BeginEvent(&notification, sizeof(PlayerUpdateEvent_Notification));
	IPC_ProcessEvent();
	IPC_EndEvent(&response, sizeof(PlayerUpdateEvent_Response), IPC_EVENT_DEFAULT_TIMEOUT);
}

__declspec(naked) void PlayerUpdateEvent_Hook()
{
	_asm
	{
		push ecx
			push ecx
				call PlayerUpdateEvent_Payload
			add esp, 4
		pop ecx
		jmp PlayerUpdateEvent_Original
	}
}


/******************************************
*********** PlayerHitsEnemyEvent **********
*******************************************/

void* PlayerHitsEnemyEvent_Original;

void __cdecl PlayerHitsEnemyEvent_Payload(Player* player, Entity* enemy, int a3)
{

	ofstream stream = ofstream("log.log", ios_base::app);
	stream << sprintf("playerName: %s, enemyID=%d\n", player->_charname, enemy->_id) << endl;

	/*PlayerUpdateEvent_Notification notification(player);
	PlayerUpdateEvent_Response response;

	IPC_BeginEvent(&notification, sizeof(PlayerUpdateEvent_Notification));
	IPC_ProcessEvent();
	IPC_EndEvent(&response, sizeof(PlayerUpdateEvent_Response), IPC_EVENT_DEFAULT_TIMEOUT);*/
}

__declspec(naked) void PlayerHitsEnemyEvent_Hook()
{
	_asm
	{
		push ebp
		mov ebp, esp
			push ecx
				push dword ptr[ebp+0x0C]
				push dword ptr[ebp+0x08]
				push ecx
					call PlayerHitsEnemyEvent_Payload
				add esp, 12
			pop ecx
		pop ebp
		jmp PlayerHitsEnemyEvent_Original
	}
}

/******************************************
************ AddBlackHeartsEvent **********
*******************************************/

void* AddBlackHeartsEvent_Original;

void __cdecl AddBlackHeartsEvent_Payload(Player* player, int amount)
{
	// TODO

	/*PlayerUpdateEvent_Notification notification(player);
	PlayerUpdateEvent_Response response;

	IPC_BeginEvent(&notification, sizeof(PlayerUpdateEvent_Notification));
	IPC_ProcessEvent();
	IPC_EndEvent(&response, sizeof(PlayerUpdateEvent_Response), IPC_EVENT_DEFAULT_TIMEOUT);*/
}

__declspec(naked) void AddBlackHeartsEvent_Hook()
{
	_asm
	{
		push ebp
		mov ebp, esp
			push ebx
				push dword ptr[ebp+0x08]
				push ebx
					call AddBlackHeartsEvent_Payload
				add esp, 8
			pop ebx
		pop ebp
		jmp AddBlackHeartsEvent_Original
	}
}

/******************************************
************ StoreItemStructEvent *********
*******************************************/

void* StoreItemStructEvent_Original;

int __cdecl StoreItemStructEvent_Payload(ItemStorageArray* _itemStorageArray,  int itemID, Item* newitem)
{
	if (itemStorageArray == NULL)
		itemStorageArray = _itemStorageArray;

	// custom items
	if (itemID < 0)
	{
		custom_items[itemID] = newitem;
		return 235;
	}
	else
	{
		return itemID;
	}
}

int _redirectItemID;

__declspec(naked) void StoreItemStructEvent_Hook()
{
	_asm
	{
		push ebp
		push eax
		push ebx
		push ecx
		push edx
		push esi
		push edi
			push edi
			push eax
			push ecx
				call StoreItemStructEvent_Payload
			add esp, 12
			mov [_redirectItemID], eax
		pop edi
		pop esi
		pop edx
		pop ecx
		pop ebx
		pop eax
		pop ebp
		mov eax, [_redirectItemID]
		jmp StoreItemStructEvent_Original
	}
}


/******************************************
************* GotoFloorEvent ***********
*******************************************/

void* GotoFloorEvent_Original;

void __cdecl GotoFloorEvent_Payload(BOOL showForgetMeNow, BOOL skipOneFloor, int a3)
{
	PlayerManager* pman = API_GetPlayerManager();
	int nextFloorNo = pman->_floorNo;
	if (skipOneFloor)
		nextFloorNo++;

	GotoFloorEvent_Notification notification(nextFloorNo);
	GotoFloorEvent_Response response;

	IPC_BeginEvent(&notification, sizeof(GotoFloorEvent_Notification));
	IPC_ProcessEvent();
	IPC_EndEvent(&response, sizeof(GotoFloorEvent_Response), IPC_EVENT_DEFAULT_TIMEOUT);
}

__declspec(naked) void GotoFloorEvent_Hook()
{
	_asm
	{
		push eax
		push ecx
		push edi
			push edi
			push ecx
			push eax
				call GotoFloorEvent_Payload
			add esp, 12
		pop edi
		pop ecx
		pop eax
		jmp GotoFloorEvent_Original
	}
}


/******************************************
*************** Functions *****************
*******************************************/

GoodPillEffectFuncType* GoodPillEffectFunc;
IsaacRandomFuncType* IsaacRandomFunc;
InitTearFuncType* InitTearFunc;
Player_TeleportFuncType* Player_TeleportFunc;
void* Game_FreezeEntityFunc;
void* Game_FearEntityFunc;
void* Game_ConfuseEntityFunc;
void* Game_CharmEntityFunc;


/******************************************
************** Initialization *************
*******************************************/

void Hooks_HookEvents()
{
	// UseCardEvent
	void* UseCardEvent_SigPtr = SigScan_FindSignature(&Signature_UseCardEvent);
	UseCardEvent_Original = DetourFunction(PBYTE(UseCardEvent_SigPtr), PBYTE(UseCardEvent_Hook));

	// TakePillEvent
	void* TakePillEvent_SigPtr = SigScan_FindSignature(&Signature_TakePillEvent);
	TakePillEvent_Original = DetourFunction(PBYTE(TakePillEvent_SigPtr), PBYTE(TakePillEvent_Hook));

	// AddCollectibleEvent
	void* AddCollectibleEvent_SigPtr = SigScan_FindSignature(&Signature_AddCollectibleEvent);
	AddCollectibleEvent_Original = DetourFunction(PBYTE(AddCollectibleEvent_SigPtr), PBYTE(AddCollectibleEvent_Hook));

	// SpawnEntityEvent
	void* SpawnEntityEvent_SigPtr = SigScan_FindSignature(&Signature_SpawnEntityEvent);
	SpawnEntityEvent_Original = DetourFunction(PBYTE(SpawnEntityEvent_SigPtr), PBYTE(SpawnEntityEvent_Hook));

	// HpUpEvent
	void* HpUpEvent_SigPtr = SigScan_FindSignature(&Signature_HpUpEvent);
	HpUpEvent_Original = DetourFunction(PBYTE(HpUpEvent_SigPtr), PBYTE(HpUpEvent_Hook));

	// AddSoulHeartsEvent
	void* AddSoulHeartsEvent_SigPtr = SigScan_FindSignature(&Signature_AddSoulHeartsEvent);
	AddSoulHeartsEvent_Original = DetourFunction(PBYTE(AddSoulHeartsEvent_SigPtr), PBYTE(AddSoulHeartsEvent_Hook));

	// ShootTearsEvent
	void* ShootTearsEvent_SigPtr = SigScan_FindSignature(&Signature_ShootTearsEvent);
	ShootTearsEvent_Original = DetourFunction(PBYTE(ShootTearsEvent_SigPtr), PBYTE(ShootTearsEvent_Hook));

	// ChangeRoomEvent
	void* ChangeRoomEvent_SigPtr = SigScan_FindSignature(&Signature_ChangeRoomEvent);
	ChangeRoomEvent_Original = DetourFunction(PBYTE(ChangeRoomEvent_SigPtr), PBYTE(ChangeRoomEvent_Hook));

	// VFSLoadFile
	void* VFSLoadFile_SigPtr = SigScan_FindSignature(&Signature_VFSLoadFile);
	VFSLoadFile_Original = DetourFunction(PBYTE(VFSLoadFile_SigPtr), PBYTE(VFSLoadFile_Hook));

	//GameUpdateEvent
	void* GameUpdateEvent_SigPtr = SigScan_FindSignature(&Signature_GameUpdate);
	GameUpdateEvent_Original = DetourFunction(PBYTE(GameUpdateEvent_SigPtr), PBYTE(GameUpdate_Hook));

	//PlayerUpdateEvent
	void* PlayerUpdateEvent_SigPtr = SigScan_FindSignature(&Signature_PlayerUpdate);
	PlayerUpdateEvent_Original = DetourFunction(PBYTE(PlayerUpdateEvent_SigPtr), PBYTE(PlayerUpdateEvent_Hook));

	// GotoFloorEvent
	void* GotoFloorEvent_SigPtr = SigScan_FindSignature(&Signature_GotoFloorEvent);
	GotoFloorEvent_Original = DetourFunction(PBYTE(GotoFloorEvent_SigPtr), PBYTE(GotoFloorEvent_Hook));

	// AddBlackHeartsEvent
	void* AddBlackHeartsEvent_SigPtr = SigScan_FindSignature(&Signature_AddBlackHeartsEvent);
	AddBlackHeartsEvent_Original = DetourFunction(PBYTE(AddBlackHeartsEvent_SigPtr), PBYTE(AddBlackHeartsEvent_Hook));

	// PlayerHitsEnemyEvent
	void* PlayerHitsEnemyEvent_SigPtr = SigScan_FindSignature(&Signature_PlayerHitsEnemyEvent);
	PlayerHitsEnemyEvent_Original = DetourFunction(PBYTE(PlayerHitsEnemyEvent_SigPtr), PBYTE(PlayerHitsEnemyEvent_Hook));

	// StoreItemStructEvent
	void* StoreItemStructEvent_SigPtr = SigScan_FindSignature(&Signature_StoreItemStructEvent);
	StoreItemStructEvent_Original = DetourFunction(PBYTE(StoreItemStructEvent_SigPtr), PBYTE(StoreItemStructEvent_Hook));

	// PickupItemEvent
	void* CollideWithEntityEvent_SigPtr = SigScan_FindSignature(&Signature_CollideWithEntityEvent);
	CollideWithEntityEvent_Original = DetourFunction(PBYTE(CollideWithEntityEvent_SigPtr), PBYTE(CollideWithEntityEvent_Hook));


	FILE* f;
	fopen_s(&f, "C:\\Hooks_HookEvents.txt", "a+");
		fprintf(f, "UseCardEvent_SigPtr: %p\n", UseCardEvent_SigPtr);
		fprintf(f, "TakePillEvent_SigPtr: %p\n", TakePillEvent_SigPtr);
		fprintf(f, "AddCollectibleEvent_SigPtr: %p\n", AddCollectibleEvent_SigPtr);
		fprintf(f, "SpawnEntityEvent_SigPtr: %p\n", SpawnEntityEvent_SigPtr);
		fprintf(f, "HpUpEvent_SigPtr: %p\n", HpUpEvent_SigPtr);
		fprintf(f, "AddSoulHeartsEvent_SigPtr: %p\n", AddSoulHeartsEvent_SigPtr);
		fprintf(f, "ShootTearsEvent_SigPtr: %p\n", ShootTearsEvent_SigPtr);
		fprintf(f, "ChangeRoomEvent_SigPtr: %p\n", ChangeRoomEvent_SigPtr);
		fprintf(f, "VFSLoadFile_SigPtr: %p\n", VFSLoadFile_SigPtr);
		fprintf(f, "GameUpdateEvent_SigPtr: %p\n", GameUpdateEvent_SigPtr);
		fprintf(f, "PlayerUpdateEvent_SigPtr: %p\n", PlayerUpdateEvent_SigPtr);
		fprintf(f, "GotoFloorEvent_SigPtr: %p\n", GotoFloorEvent_SigPtr);
		fprintf(f, "AddBlackHeartsEvent_SigPtr: %p\n", AddBlackHeartsEvent_SigPtr);
		fprintf(f, "PlayerHitsEnemyEvent_SigPtr: %p\n", PlayerHitsEnemyEvent_SigPtr);
		fprintf(f, "StoreItemStructEvent_SigPtr: %p\n", StoreItemStructEvent_SigPtr);
		fprintf(f, "CollideWithEntityEvent_SigPtr: %p\n", CollideWithEntityEvent_SigPtr);
	fclose(f);
}

void Hooks_GetFunctions()
{
	// misc
	IsaacRandomFunc = (IsaacRandomFuncType*)SigScan_FindSignature(&Signature_IsaacRandomFunc);
	InitTearFunc = (InitTearFuncType*)SigScan_FindSignature(&Signature_InitTearFunc);

	// effects
	GoodPillEffectFunc = (GoodPillEffectFuncType*)SigScan_FindSignature(&Signature_GoodPillEffectFunc);

	// player functions
	Player_TeleportFunc = (Player_TeleportFuncType*)SigScan_FindSignature(&Signature_PlayerTeleportEvent);  // could/should be hooked, too

	// Status effects
	Game_FreezeEntityFunc = SigScan_FindSignature(&Signature_FreezeEntity); 
	Game_FearEntityFunc = SigScan_FindSignature(&Signature_FearEntity); 
	Game_ConfuseEntityFunc = SigScan_FindSignature(&Signature_ConfuseEntity);
	Game_CharmEntityFunc = SigScan_FindSignature(&Signature_CharmEntity);
}

DWORD** PlayerManagerPtr;

void Hooks_GetPlayerManagerPtr()
{
	PlayerManagerPtr = (DWORD**)SigScan_FindSignature(&Signature_PlayerManager);
	PlayerManagerPtr = (DWORD**)((DWORD)PlayerManagerPtr + 2);
}

PlayerManager* Hooks_GetPlayerManager()
{
	if (PlayerManagerPtr)
		return (PlayerManager*)**PlayerManagerPtr;
	else
		return NULL;
}

bool Hooks_Init()
{
	if (SigScan_GetImageInfo())
	{
		Hooks_GetPlayerManagerPtr();

		Hooks_GetFunctions();

		Hooks_HookEvents();

		return true;
	}
	else
		return false;
}