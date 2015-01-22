#include "stdafx.h"

struct FuncSignature
{
	char* signature;
	char* mask;
	size_t length;
};

void* SigScan_FindSignature(FuncSignature* sig);
BOOL SigScan_GetImageInfo();