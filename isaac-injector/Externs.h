#include "stdafx.h"


struct Entity
{
	char _unk0[0x76C];
};

struct Player : Entity
{
	char _unk76C[0x3E4];
	int _maxHearts;
	int _hearts;
	int _eternalHearts;
	int _soulHearts;
	int _blackHeartMask;
	int _jarHearts;
	int _keys;
	int _hasGoldenKey;
	int _numBombs;
	int _numCoins;
	char _unkB78[0x24E8];
};