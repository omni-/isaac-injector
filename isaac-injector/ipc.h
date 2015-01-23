#pragma once
#include "stdafx.h"
#include "Windows.h"
#include "hooks.h"

extern HANDLE hPipe;
extern HANDLE hMutex;

bool IPC_Init();

bool IPC_SendEvent(int eventID, ...);

bool IPC_RecieveEvent(int eventID, ...);