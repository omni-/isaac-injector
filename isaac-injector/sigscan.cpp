#include "stdafx.h"
#include <stdio.h>
#include <Windows.h>
#include <string>
#include <cstring>
#include <fstream>
#include <Psapi.h>
#include "sigscan.h"

void* SigScan_BasePtr = NULL;
size_t SigScan_BaseLen = 0;

BOOL SigScan_GetImageInfo()
{
	BOOL Success = false;

	HMODULE hMod = GetModuleHandle(L"isaac-ng.exe");
	if (hMod != 0)
	{
		MEMORY_BASIC_INFORMATION memInfo;
		if (VirtualQuery((void*)hMod, &memInfo, sizeof(memInfo)) != 0)
		{
			IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)hMod;
			IMAGE_NT_HEADERS* pe = (IMAGE_NT_HEADERS*)((unsigned long)memInfo.AllocationBase + (unsigned long)dos->e_lfanew);

			if ((dos->e_magic == IMAGE_DOS_SIGNATURE) && (pe->Signature == IMAGE_NT_SIGNATURE))
			{
				Success = true;

				SigScan_BasePtr = (void*)memInfo.AllocationBase;
				SigScan_BaseLen = (size_t)pe->OptionalHeader.SizeOfImage;
			}
			else
				;
			//Log("DOS- or PE-Signatur invalid!\n");
		}
	}

	return Success;
}

void* SigScan_FindSignature(FuncSignature* sig)
{
	FILE* debugFile;

	char* tempPtr = (char*)SigScan_BasePtr;

	int maxLen = 0;
	while (tempPtr < ((char*)SigScan_BasePtr + SigScan_BaseLen - sig->length))
	{
		size_t detectedLen = 0;

		for (int i = 0; i < sig->length; i++)
		{
			if (!((tempPtr[i] == sig->signature[i]) || (sig->mask[i] == '?')))
				break;
			detectedLen++;
		}

		if (detectedLen > maxLen)
			maxLen = detectedLen;

		if (detectedLen == sig->length)
			return tempPtr;

		tempPtr++;
	}

	return NULL;
}