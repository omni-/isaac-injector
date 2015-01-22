#include "stdafx.h"
#include "Externs.h"

char* Player_serialize(Player* player, DWORD &resultSize)
{
	resultSize = sizeof(Player);
	char* result = new char[resultSize];
	memmove(result, player, resultSize);

	return result;
}