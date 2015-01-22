#pragma once
#include "stdafx.h"
#define BUFSIZE 512

HANDLE InitPipe(DWORD &error);

HANDLE ConnectMutex();