#pragma once
#include "stdafx.h"
#include "Windows.h"
#include "hooks.h"

#define IPC_EVENT_DEFAULT_TIMEOUT 100
#define IPC_API_DEFAULT_TIMEOUT 10

bool IPC_Init();

bool IPC_BeginEvent(void* eventMsg, size_t msgSize);
bool IPC_EndEvent(void* responseMsg, size_t msgSize, DWORD timeout);

unsigned int IPC_HandleAPICall(DWORD timeout);
void IPC_ProcessEvent();