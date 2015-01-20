#include "stdafx.h"
#define BUFSIZE 512

HANDLE initPipe(DWORD &error);

unsigned char* awaitServerResponse(HANDLE hPipe);