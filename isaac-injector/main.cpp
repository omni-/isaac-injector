#include "stdafx.h"
#include "isaac-injector.h"

BOOL APIENTRY DllMain(HANDLE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		std::ofstream outfile;
		outfile.open("c:/users/cooper/desktop/log.txt", std::ios_base::app);
		outfile << "======================================" << std::endl;
		AddCollectibleSig.Init((unsigned char*)
			"\x55\x8B\xEC\x83\xE4\xF8\x6A\xFF\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x83\xEC\x78\xA1\x00\x00\x00\x00\x33\xC4\x89\x44\x24\x70\x53\x56\x57\xA1"
			"\x00\x00\x00\x00\x33\xC4\x50\x8D\x84\x24\x88\x00\x00\x00\x64\xA3\x00\x00\x00\x00\x8B\x75\x08\x8B\xD9\x89\x5C\x24\x1C\x89\x74\x24\x24\x81\xFE"

			,
			"xxxxxxxxx????xx????xxxxx????xxxxxxxxxx????xxxxxxxxxx????xxxxxxxxxxxx"
			,
			68);
		PillCardActionSig.Init((unsigned char*)"\x55\x8B\xEC\x6A\xFF\x68\x00\x00\x00\x00\x64\xA1\x00\x00\x00\x00\x50\x81\xEC\x00\x00\x00\x00", 
			"xxxxxx????xxxxxxxxx????", 23);

		Player_AddCollectible = (AddCollectibleType*)(DetourFunction(PBYTE(AddCollectibleSig.sig_addr), PBYTE(AddCollectible_Hook)));
		Player_PillCardAction = (PillCardActionType*)(DetourFunction(PBYTE(PillCardActionSig.sig_addr), PBYTE(PillCardAction_Hook)));

		break;
	}
	return TRUE;
}
