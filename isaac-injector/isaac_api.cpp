#include "stdafx.h"
#include "Externs.h"
#include "hooks.h"
#include "isaac_api.h"

PlayerManager* API_GetPlayerManager()
{
	return Hooks_GetPlayerManager();
}

void API_GotoFloor(unsigned int floorNo)
{
	PlayerManager* pman = API_GetPlayerManager();
	pman->_floorNo = floorNo & (~FLOOR_ALTERNATE);
	pman->_alternateFloor = (floorNo & FLOOR_ALTERNATE) > 0;
	_asm
	{
		mov eax, 0                   // 1 = forgetMeNow animation
		mov ecx, 1                   // 0 = +1 floor further?!
		mov di, 0					 // ??
		call GotoFloorEvent_Original
	}
}

void API_Effect_GoodPill(Player* player)
{
	GoodPillEffectFunc(player);
}

void API_HPUp(Player* player, int amount)
{
	_asm
	{
		mov eax, player
		mov ecx, amount
		call HpUpEvent_Original
	}
}

void API_AddSoulHearts(Player* player, int amount)
{
	_asm
	{
		mov eax, player
		mov ecx, amount
		call AddSoulHeartsEvent_Original
	}
}

void API_AddBlackHearts(Player* player, int amount)
{
	_asm
	{
		mov ebx, player
		push amount
		call AddBlackHeartsEvent_Original
	}
}

void API_FreezeEntity(Entity* entity, int duration)
{
	_asm
	{
		mov ecx, entity
		mov edx, duration
		call Game_FreezeEntityFunc
	}
}

void API_FearEntity(Entity* entity, int duration)
{
	_asm
	{
		mov ecx, entity
		mov edx, duration
		call Game_FearEntityFunc
	}
}

void API_CharmEntity(Entity* entity, int duration)
{
	_asm
	{
		mov ecx, entity
		push duration
		call Game_CharmEntityFunc
	}
}

void API_ConfuseEntity(Entity* entity, int duration)
{
	_asm
	{
		mov ecx, entity
		mov edx, duration
		call Game_ConfuseEntityFunc
	}
}

Entity* API_SpawnEntity(int entityID, int variant, int subtype, float x, float y, Entity* parent)
{
	// zero
	PointF* velocity = new PointF();
	velocity->x = 0;
	velocity->y = 0;
	// position
	PointF* pos = new PointF();
	pos->x = (x * 40) + 80;
	pos->y = (y * 40) + 160;
	// manager
	PlayerManager* playerMan = Hooks_GetPlayerManager();

	unsigned int seed = IsaacRandomFunc();

	if (playerMan)
	{
		_asm
		{
			push seed
			push subtype
			push parent
			push variant
			push entityID
			push playerMan
			mov ebx, pos
			mov eax, velocity
			call SpawnEntityEvent_Original
		}
	}
	else
		return NULL;
}

int API_TeleportPlayer(int roomID)
{
	PlayerManager* playerMan = Hooks_GetPlayerManager();
	void* roomManager = (void*)((int)playerMan+33088);  // I actually don't know this yet

	_asm
	{
		mov eax, 3
		mov edx, roomID
		mov esi, roomManager
		push -1
		call Player_TeleportFunc
	}
}

TearInfo* API_InitTear(int value, TearInfo* tear)
{
	_asm
	{
		mov ecx, value
		mov esi, tear
		call InitTearFunc
	}
}

void API_AddCollectible(Player* player, int itemID)
{
	_asm
	{
		mov ecx, player
		push 0
		push 0
		push itemID
		call AddCollectibleEvent_Original
	}
}

void API_ShootTears(PointF* pos, PointF* velocity, int pattern, TearInfo* tear, Entity* source)
{
	_asm
	{
		push tear
		push pattern
		push source
		mov edx, pos
		mov ecx, velocity
		call ShootTearsEvent_Original
	}
}

bool API_PlayerHasItem(Player* player, int itemID)
{
	return player->_items[itemID];
}

char* API_getItemName(int itemID)
{
	std::map<int, char*> items;
	items[1] = "The Sad Onion";
	items[2] = "The Inner Eye";
	items[3] = "Spoon Bender";
	items[4] = "Cricket's Head";
	items[5] = "My Reflection";
	items[6] = "Number One";
	items[7] = "Blood of the Martyr";
	items[8] = "Brother Bobby";
	items[9] = "Skatole";
	items[10] = "Halo of Flies";
	items[11] = "1up!";
	items[12] = "Magic Mushroom";
	items[13] = "The Virus";
	items[14] = "Roid Rage";
	items[15] = "<3";
	items[16] = "Raw Liver";
	items[17] = "Skeleton Key";
	items[18] = "A Dollar";
	items[19] = "Boom!";
	items[20] = "Transcendence";
	items[21] = "The Compass";
	items[22] = "Lunch";
	items[23] = "Dinner";
	items[24] = "Dessert";
	items[25] = "Breakfast";
	items[26] = "Rotten Meat";
	items[27] = "Wooden Spoon";
	items[28] = "The Belt";
	items[29] = "Mom's Underwear";
	items[30] = "Mom's Heels";
	items[31] = "Mom's Lipstick";
	items[32] = "Wire Coat Hanger";
	items[33] = "The Bible";
	items[34] = "The Book of Belial";
	items[35] = "The Necronomicon";
	items[36] = "The Poop";
	items[37] = "Mr. Boom";
	items[38] = "Tammy's Head";
	items[39] = "Mom's Bra";
	items[40] = "Kamikaze!";
	items[41] = "Mom's Pad";
	items[42] = "Bob's Rotten Head";
	items[43] = "Pills here!";
	items[44] = "Teleport!";
	items[45] = "Yum Heart";
	items[46] = "Lucky Foot";
	items[47] = "Doctor's Remote";
	items[48] = "Cupid's Arrow";
	items[49] = "Shoop da Whoop!";
	items[50] = "Steven";
	items[51] = "Pentagram";
	items[52] = "Dr. Fetus";
	items[53] = "Magneto";
	items[54] = "Treasure Map";
	items[55] = "Mom's Eye";
	items[56] = "Lemon Mishap";
	items[57] = "Distant Admiration";
	items[58] = "Book of Shadows";
	items[60] = "The Ladder";
	items[61] = "Tarot Card";
	items[62] = "Charm of the Vampire";
	items[63] = "The Battery";
	items[64] = "Steam Sale";
	items[65] = "Anarchist Cookbook";
	items[66] = "The Hourglass";
	items[67] = "Sister Maggy";
	items[68] = "Technology";
	items[69] = "Chocolate Milk";
	items[70] = "Growth Hormones";
	items[71] = "Mini Mush";
	items[72] = "Rosary";
	items[73] = "Cube of Meat";
	items[74] = "A Quarter";
	items[75] = "PHD";
	items[76] = "X-Ray Vision";
	items[77] = "My Little Unicorn";
	items[78] = "Book of Revelations";
	items[79] = "The Mark";
	items[80] = "The Pact";
	items[81] = "Dead Cat";
	items[82] = "Lord of the Pit";
	items[83] = "The Nail";
	items[84] = "We Need To Go Deeper!";
	items[85] = "Deck of Cards";
	items[86] = "Monstro's Tooth";
	items[87] = "Loki's Horns";
	items[88] = "Little Chubby";
	items[89] = "Spider Bite";
	items[90] = "The Small Rock";
	items[91] = "Spelunker Hat";
	items[92] = "Super Bandage";
	items[93] = "The Gamekid";
	items[94] = "Sack of Pennies";
	items[95] = "Robo-Baby";
	items[96] = "Little C.H.A.D.";
	items[97] = "The Book of Sin";
	items[98] = "The Relic";
	items[99] = "Little Gish";
	items[100] = "Little Steven";
	items[101] = "The Halo";
	items[102] = "Mom's Bottle of Pills";
	items[103] = "The Common Cold";
	items[104] = "The Parasite";
	items[105] = "The D6";
	items[106] = "Mr. Mega";
	items[107] = "The Pinking Shears";
	items[108] = "The Wafer";
	items[109] = "Money = Power";
	items[110] = "Mom's Contacts";
	items[111] = "The Bean";
	items[112] = "Guardian Angel";
	items[113] = "Demon Baby";
	items[114] = "Mom's Knife";
	items[115] = "Ouija Board";
	items[116] = "9 Volt";
	items[117] = "Dead Bird";
	items[118] = "Brimstone";
	items[119] = "Blood Bag";
	items[120] = "Odd Mushroom";
	items[121] = "Odd Mushroom";
	items[122] = "Whore of Babylon";
	items[123] = "Monster Manual";
	items[124] = "Dead Sea Scrolls";
	items[125] = "Bobby-Bomb";
	items[126] = "Razor Blade";
	items[127] = "Forget Me Now";
	items[128] = "Forever alone";
	items[129] = "Bucket of Lard";
	items[130] = "A Pony";
	items[131] = "Bomb Bag";
	items[132] = "A Lump of Coal";
	items[133] = "Guppy's Paw";
	items[134] = "Guppy's Tail";
	items[135] = "IV Bag";
	items[136] = "Best Friend";
	items[137] = "Remote Detonator";
	items[138] = "Stigmata";
	items[139] = "Mom's Purse";
	items[140] = "Bobs Curse";
	items[141] = "Pageant Boy";
	items[142] = "Scapular";
	items[143] = "Speed Ball";
	items[144] = "Bum Friend";
	items[145] = "Guppy's Head";
	items[146] = "Prayer Card";
	items[147] = "Notched Axe";
	items[148] = "Infestation";
	items[149] = "Ipecac";
	items[150] = "Tough Love";
	items[151] = "The Mulligan";
	items[152] = "Technology 2";
	items[153] = "Mutant Spider";
	items[154] = "Chemical Peel";
	items[155] = "The Peeper";
	items[156] = "Habit";
	items[157] = "Bloody Lust";
	items[158] = "Crystal Ball";
	items[159] = "Spirit of the Night";
	items[160] = "Crack the Sky";
	items[161] = "Ankh";
	items[162] = "Celtic Cross";
	items[163] = "Ghost Baby";
	items[164] = "The Candle";
	items[165] = "Cat-o-nine-tails";
	items[166] = "D20";
	items[167] = "Harlequin Baby";
	items[168] = "Epic Fetus";
	items[169] = "Polyphemus";
	items[170] = "Daddy Longlegs";
	items[171] = "Spider Butt";
	items[172] = "Sacrificial Dagger";
	items[173] = "Mitre";
	items[174] = "Rainbow Baby";
	items[175] = "Dad's Key";
	items[176] = "Stem Cells";
	items[177] = "Portable Slot";
	items[178] = "Holy Water";
	items[179] = "Fate";
	items[180] = "The Black Bean";
	items[181] = "White Pony";
	items[182] = "Sacred Heart";
	items[183] = "Tooth Picks";
	items[184] = "Holy Grail";
	items[185] = "Dead Dove";
	items[186] = "Blood Rights";
	items[187] = "Guppy's Hairball";
	items[188] = "Abel";
	items[189] = "SMB Super Fan";
	items[190] = "Pyro";
	items[191] = "3 Dollar Bill";
	items[192] = "Telepathy For Dummies";
	items[193] = "MEAT!";
	items[194] = "Magic 8 Ball";
	items[195] = "Mom's Coin Purse";
	items[196] = "Squeezy";
	items[197] = "Jesus Juice";
	items[198] = "Box";
	items[199] = "Mom's Key";
	items[200] = "Mom's Eyeshadow";
	items[201] = "Iron Bar";
	items[202] = "Midas' Touch";
	items[203] = "Humbleing Bundle";
	items[204] = "Fanny Pack";
	items[205] = "Sharp Plug";
	items[206] = "Guillotine";
	items[207] = "Ball of Bandages";
	items[208] = "Champion Belt";
	items[209] = "Butt Bombs";
	items[210] = "Gnawed Leaf";
	items[211] = "Spiderbaby";
	items[212] = "Guppy's Collar";
	items[213] = "Lost Contact";
	items[214] = "Anemic";
	items[215] = "Goat Head";
	items[216] = "Ceremonial Robes";
	items[217] = "Mom's Wig";
	items[218] = "Placenta";
	items[219] = "Old Bandage";
	items[220] = "Sad Bombs";
	items[221] = "Rubber Cement";
	items[222] = "Anti-Gravity";
	items[223] = "Pyromaniac";
	items[224] = "Cricket's Body";
	items[225] = "Gimpy";
	items[226] = "Black Lotus";
	items[227] = "Piggy Bank";
	items[228] = "Mom's Perfume";
	items[229] = "Monstro's Lung";
	items[230] = "Abaddon";
	items[231] = "Ball of Tar";
	items[232] = "Stop Watch";
	items[233] = "Tiny Planet";
	items[234] = "Infestation 2";
	items[236] = "E. Coli";
	items[237] = "Death's Touch";
	items[238] = "Key Piece 1";
	items[239] = "Key Piece 2";
	items[240] = "Experimental Treatment";
	items[241] = "Contract from Below";
	items[242] = "Infamy";
	items[243] = "Trinity Shield";
	items[244] = "Tech.5";
	items[245] = "20/20";
	items[246] = "Blue Map";
	items[247] = "BFFS!";
	items[248] = "Hive Mind";
	items[249] = "There's Options";
	items[250] = "BOGO Bombs";
	items[251] = "Starter Deck";
	items[252] = "Little Baggy";
	items[253] = "Magic Scab";
	items[254] = "Blood Clot";
	items[255] = "Screw";
	items[256] = "Hot Bombs";
	items[257] = "Fire Mind";
	items[258] = "Missing No.";
	items[259] = "Dark Matter";
	items[260] = "Black Candle";
	items[261] = "Proptosis";
	items[262] = "Missing Page 2";
	items[264] = "Smart Fly";
	items[265] = "Dry Baby";
	items[266] = "Juicy Sack";
	items[267] = "Robo-Baby 2.0";
	items[268] = "Rotten Baby";
	items[269] = "Headless Baby";
	items[270] = "Leech";
	items[271] = "Mystery Sack";
	items[272] = "BBF";
	items[273] = "Bob's Brain";
	items[274] = "Best Bud";
	items[275] = "Lil' Brimstone";
	items[276] = "Isaac's Heart";
	items[277] = "Lil' Haunt";
	items[278] = "Dark Bum";
	items[279] = "Big Fan";
	items[280] = "Sissy Longlegs";
	items[281] = "Punching Bag";
	items[282] = "How to Jump";
	items[283] = "D100";
	items[284] = "D4";
	items[285] = "D10";
	items[286] = "Blank Card";
	items[287] = "Book of Secrets";
	items[288] = "Box of Spiders";
	items[289] = "Red Candle";
	items[290] = "The Jar";
	items[291] = "Flush!";
	items[292] = "Satanic Bible";
	items[293] = "Head of Krampus";
	items[294] = "Butter Bean";
	items[295] = "Magic Fingers";
	items[296] = "Converter";
	items[297] = "Pandora's Box";
	items[298] = "Unicorn Stump";
	items[299] = "Taurus";
	items[300] = "Aries";
	items[301] = "Cancer";
	items[302] = "Leo";
	items[303] = "Virgo";
	items[304] = "Libra";
	items[305] = "Scorpio";
	items[306] = "Sagittarius";
	items[307] = "Capricorn";
	items[308] = "Aquarius";
	items[309] = "Pisces";
	items[310] = "Eve's Mascara";
	items[311] = "Judas' Shadow";
	items[312] = "Maggy's Bow";
	items[313] = "Holy Mantle";
	items[314] = "Thunder Thighs";
	items[315] = "Strange Attractor";
	items[316] = "Cursed Eye";
	items[317] = "Mysterious Liquid";
	items[318] = "Gemini";
	items[319] = "Cain's Other Eye";
	items[320] = "???'s Only Friend";
	items[321] = "Samson's Chains";
	items[322] = "Mongo Baby";
	items[323] = "Isaac's Tears";
	items[324] = "Undefined";
	items[325] = "Scissors";
	items[326] = "Breath of Life";
	items[327] = "The Polaroid";
	items[328] = "The Negative";
	items[329] = "The Ludovico Technique";
	items[330] = "Soy Milk";
	items[331] = "Godhead";
	items[332] = "Lazarus' Rags";
	items[333] = "The Mind";
	items[334] = "The Body";
	items[335] = "The Soul";
	items[336] = "Dead Onion";
	items[337] = "Broken Watch";
	items[338] = "The Boomerang";
	items[339] = "Safety Pin";
	items[340] = "Caffeine Pill";
	items[341] = "Torn Photo";
	items[342] = "Blue Cap";
	items[343] = "Latch Key";
	items[344] = "Match Book";
	items[345] = "Synthoil";
	items[346] = "A Snack";
	return items[itemID];
} 

int API_getItemID(char* name)
{
	std::map<char*, int> items;
	items["The Sad Onion"] = 1;
	items["The Inner Eye"] = 2;
	items["Spoon Bender"] = 3;
	items["Cricket's Head"] = 4;
	items["My Reflection"] = 5;
	items["Number One"] = 6;
	items["Blood of the Martyr"] = 7;
	items["Brother Bobby"] = 8;
	items["Skatole"] = 9;
	items["Halo of Flies"] = 10;
	items["1up!"] = 11;
	items["Magic Mushroom"] = 12;
	items["The Virus"] = 13;
	items["Roid Rage"] = 14;
	items["<3"] = 15;
	items["Raw Liver"] = 16;
	items["Skeleton Key"] = 17;
	items["A Dollar"] = 18;
	items["Boom!"] = 19;
	items["Transcendence"] = 20;
	items["The Compass"] = 21;
	items["Lunch"] = 22;
	items["Dinner"] = 23;
	items["Dessert"] = 24;
	items["Breakfast"] = 25;
	items["Rotten Meat"] = 26;
	items["Wooden Spoon"] = 27;
	items["The Belt"] = 28;
	items["Mom's Underwear"] = 29;
	items["Mom's Heels"] = 30;
	items["Mom's Lipstick"] = 31;
	items["Wire Coat Hanger"] = 32;
	items["The Bible"] = 33;
	items["The Book of Belial"] = 34;
	items["The Necronomicon"] = 35;
	items["The Poop"] = 36;
	items["Mr. Boom"] = 37;
	items["Tammy's Head"] = 38;
	items["Mom's Bra"] = 39;
	items["Kamikaze!"] = 40;
	items["Mom's Pad"] = 41;
	items["Bob's Rotten Head"] = 42;
	items["Pills here!"] = 43;
	items["Teleport!"] = 44;
	items["Yum Heart"] = 45;
	items["Lucky Foot"] = 46;
	items["Doctor's Remote"] = 47;
	items["Cupid's Arrow"] = 48;
	items["Shoop da Whoop!"] = 49;
	items["Steven"] = 50;
	items["Pentagram"] = 51;
	items["Dr. Fetus"] = 52;
	items["Magneto"] = 53;
	items["Treasure Map"] = 54;
	items["Mom's Eye"] = 55;
	items["Lemon Mishap"] = 56;
	items["Distant Admiration"] = 57;
	items["Book of Shadows"] = 58;
	items["The Ladder"] = 60;
	items["Tarot Card"] = 61;
	items["Charm of the Vampire"] = 62;
	items["The Battery"] = 63;
	items["Steam Sale"] = 64;
	items["Anarchist Cookbook"] = 65;
	items["The Hourglass"] = 66;
	items["Sister Maggy"] = 67;
	items["Technology"] = 68;
	items["Chocolate Milk"] = 69;
	items["Growth Hormones"] = 70;
	items["Mini Mush"] = 71;
	items["Rosary"] = 72;
	items["Cube of Meat"] = 73;
	items["A Quarter"] = 74;
	items["PHD"] = 75;
	items["X-Ray Vision"] = 76;
	items["My Little Unicorn"] = 77;
	items["Book of Revelations"] = 78;
	items["The Mark"] = 79;
	items["The Pact"] = 80;
	items["Dead Cat"] = 81;
	items["Lord of the Pit"] = 82;
	items["The Nail"] = 83;
	items["We Need To Go Deeper!"] = 84;
	items["Deck of Cards"] = 85;
	items["Monstro's Tooth"] = 86;
	items["Loki's Horns"] = 87;
	items["Little Chubby"] = 88;
	items["Spider Bite"] = 89;
	items["The Small Rock"] = 90;
	items["Spelunker Hat"] = 91;
	items["Super Bandage"] = 92;
	items["The Gamekid"] = 93;
	items["Sack of Pennies"] = 94;
	items["Robo-Baby"] = 95;
	items["Little C.H.A.D."] = 96;
	items["The Book of Sin"] = 97;
	items["The Relic"] = 98;
	items["Little Gish"] = 99;
	items["Little Steven"] = 100;
	items["The Halo"] = 101;
	items["Mom's Bottle of Pills"] = 102;
	items["The Common Cold"] = 103;
	items["The Parasite"] = 104;
	items["The D6"] = 105;
	items["Mr. Mega"] = 106;
	items["The Pinking Shears"] = 107;
	items["The Wafer"] = 108;
	items["Money = Power"] = 109;
	items["Mom's Contacts"] = 110;
	items["The Bean"] = 111;
	items["Guardian Angel"] = 112;
	items["Demon Baby"] = 113;
	items["Mom's Knife"] = 114;
	items["Ouija Board"] = 115;
	items["9 Volt"] = 116;
	items["Dead Bird"] = 117;
	items["Brimstone"] = 118;
	items["Blood Bag"] = 119;
	items["Odd Mushroom"] = 120;
	items["Odd Mushroom"] = 121;
	items["Whore of Babylon"] = 122;
	items["Monster Manual"] = 123;
	items["Dead Sea Scrolls"] = 124;
	items["Bobby-Bomb"] = 125;
	items["Razor Blade"] = 126;
	items["Forget Me Now"] = 127;
	items["Forever alone"] = 128;
	items["Bucket of Lard"] = 129;
	items["A Pony"] = 130;
	items["Bomb Bag"] = 131;
	items["A Lump of Coal"] = 132;
	items["Guppy's Paw"] = 133;
	items["Guppy's Tail"] = 134;
	items["IV Bag"] = 135;
	items["Best Friend"] = 136;
	items["Remote Detonator"] = 137;
	items["Stigmata"] = 138;
	items["Mom's Purse"] = 139;
	items["Bobs Curse"] = 140;
	items["Pageant Boy"] = 141;
	items["Scapular"] = 142;
	items["Speed Ball"] = 143;
	items["Bum Friend"] = 144;
	items["Guppy's Head"] = 145;
	items["Prayer Card"] = 146;
	items["Notched Axe"] = 147;
	items["Infestation"] = 148;
	items["Ipecac"] = 149;
	items["Tough Love"] = 150;
	items["The Mulligan"] = 151;
	items["Technology 2"] = 152;
	items["Mutant Spider"] = 153;
	items["Chemical Peel"] = 154;
	items["The Peeper"] = 155;
	items["Habit"] = 156;
	items["Bloody Lust"] = 157;
	items["Crystal Ball"] = 158;
	items["Spirit of the Night"] = 159;
	items["Crack the Sky"] = 160;
	items["Ankh"] = 161;
	items["Celtic Cross"] = 162;
	items["Ghost Baby"] = 163;
	items["The Candle"] = 164;
	items["Cat-o-nine-tails"] = 165;
	items["D20"] = 166;
	items["Harlequin Baby"] = 167;
	items["Epic Fetus"] = 168;
	items["Polyphemus"] = 169;
	items["Daddy Longlegs"] = 170;
	items["Spider Butt"] = 171;
	items["Sacrificial Dagger"] = 172;
	items["Mitre"] = 173;
	items["Rainbow Baby"] = 174;
	items["Dad's Key"] = 175;
	items["Stem Cells"] = 176;
	items["Portable Slot"] = 177;
	items["Holy Water"] = 178;
	items["Fate"] = 179;
	items["The Black Bean"] = 180;
	items["White Pony"] = 181;
	items["Sacred Heart"] = 182;
	items["Tooth Picks"] = 183;
	items["Holy Grail"] = 184;
	items["Dead Dove"] = 185;
	items["Blood Rights"] = 186;
	items["Guppy's Hairball"] = 187;
	items["Abel"] = 188;
	items["SMB Super Fan"] = 189;
	items["Pyro"] = 190;
	items["3 Dollar Bill"] = 191;
	items["Telepathy For Dummies"] = 192;
	items["MEAT!"] = 193;
	items["Magic 8 Ball"] = 194;
	items["Mom's Coin Purse"] = 195;
	items["Squeezy"] = 196;
	items["Jesus Juice"] = 197;
	items["Box"] = 198;
	items["Mom's Key"] = 199;
	items["Mom's Eyeshadow"] = 200;
	items["Iron Bar"] = 201;
	items["Midas' Touch"] = 202;
	items["Humbleing Bundle"] = 203;
	items["Fanny Pack"] = 204;
	items["Sharp Plug"] = 205;
	items["Guillotine"] = 206;
	items["Ball of Bandages"] = 207;
	items["Champion Belt"] = 208;
	items["Butt Bombs"] = 209;
	items["Gnawed Leaf"] = 210;
	items["Spiderbaby"] = 211;
	items["Guppy's Collar"] = 212;
	items["Lost Contact"] = 213;
	items["Anemic"] = 214;
	items["Goat Head"] = 215;
	items["Ceremonial Robes"] = 216;
	items["Mom's Wig"] = 217;
	items["Placenta"] = 218;
	items["Old Bandage"] = 219;
	items["Sad Bombs"] = 220;
	items["Rubber Cement"] = 221;
	items["Anti-Gravity"] = 222;
	items["Pyromaniac"] = 223;
	items["Cricket's Body"] = 224;
	items["Gimpy"] = 225;
	items["Black Lotus"] = 226;
	items["Piggy Bank"] = 227;
	items["Mom's Perfume"] = 228;
	items["Monstro's Lung"] = 229;
	items["Abaddon"] = 230;
	items["Ball of Tar"] = 231;
	items["Stop Watch"] = 232;
	items["Tiny Planet"] = 233;
	items["Infestation 2"] = 234;
	items["E. Coli"] = 236;
	items["Death's Touch"] = 237;
	items["Key Piece 1"] = 238;
	items["Key Piece 2"] = 239;
	items["Experimental Treatment"] = 240;
	items["Contract from Below"] = 241;
	items["Infamy"] = 242;
	items["Trinity Shield"] = 243;
	items["Tech.5"] = 244;
	items["20/20"] = 245;
	items["Blue Map"] = 246;
	items["BFFS!"] = 247;
	items["Hive Mind"] = 248;
	items["There's Options"] = 249;
	items["BOGO Bombs"] = 250;
	items["Starter Deck"] = 251;
	items["Little Baggy"] = 252;
	items["Magic Scab"] = 253;
	items["Blood Clot"] = 254;
	items["Screw"] = 255;
	items["Hot Bombs"] = 256;
	items["Fire Mind"] = 257;
	items["Missing No."] = 258;
	items["Dark Matter"] = 259;
	items["Black Candle"] = 260;
	items["Proptosis"] = 261;
	items["Missing Page 2"] = 262;
	items["Smart Fly"] = 264;
	items["Dry Baby"] = 265;
	items["Juicy Sack"] = 266;
	items["Robo-Baby 2.0"] = 267;
	items["Rotten Baby"] = 268;
	items["Headless Baby"] = 269;
	items["Leech"] = 270;
	items["Mystery Sack"] = 271;
	items["BBF"] = 272;
	items["Bob's Brain"] = 273;
	items["Best Bud"] = 274;
	items["Lil' Brimstone"] = 275;
	items["Isaac's Heart"] = 276;
	items["Lil' Haunt"] = 277;
	items["Dark Bum"] = 278;
	items["Big Fan"] = 279;
	items["Sissy Longlegs"] = 280;
	items["Punching Bag"] = 281;
	items["How to Jump"] = 282;
	items["D100"] = 283;
	items["D4"] = 284;
	items["D10"] = 285;
	items["Blank Card"] = 286;
	items["Book of Secrets"] = 287;
	items["Box of Spiders"] = 288;
	items["Red Candle"] = 289;
	items["The Jar"] = 290;
	items["Flush!"] = 291;
	items["Satanic Bible"] = 292;
	items["Head of Krampus"] = 293;
	items["Butter Bean"] = 294;
	items["Magic Fingers"] = 295;
	items["Converter"] = 296;
	items["Pandora's Box"] = 297;
	items["Unicorn Stump"] = 298;
	items["Taurus"] = 299;
	items["Aries"] = 300;
	items["Cancer"] = 301;
	items["Leo"] = 302;
	items["Virgo"] = 303;
	items["Libra"] = 304;
	items["Scorpio"] = 305;
	items["Sagittarius"] = 306;
	items["Capricorn"] = 307;
	items["Aquarius"] = 308;
	items["Pisces"] = 309;
	items["Eve's Mascara"] = 310;
	items["Judas' Shadow"] = 311;
	items["Maggy's Bow"] = 312;
	items["Holy Mantle"] = 313;
	items["Thunder Thighs"] = 314;
	items["Strange Attractor"] = 315;
	items["Cursed Eye"] = 316;
	items["Mysterious Liquid"] = 317;
	items["Gemini"] = 318;
	items["Cain's Other Eye"] = 319;
	items["???'s Only Friend"] = 320;
	items["Samson's Chains"] = 321;
	items["Mongo Baby"] = 322;
	items["Isaac's Tears"] = 323;
	items["Undefined"] = 324;
	items["Scissors"] = 325;
	items["Breath of Life"] = 326;
	items["The Polaroid"] = 327;
	items["The Negative"] = 328;
	items["The Ludovico Technique"] = 329;
	items["Soy Milk"] = 330;
	items["Godhead"] = 331;
	items["Lazarus' Rags"] = 332;
	items["The Mind"] = 333;
	items["The Body"] = 334;
	items["The Soul"] = 335;
	items["Dead Onion"] = 336;
	items["Broken Watch"] = 337;
	items["The Boomerang"] = 338;
	items["Safety Pin"] = 339;
	items["Caffeine Pill"] = 340;
	items["Torn Photo"] = 341;
	items["Blue Cap"] = 342;
	items["Latch Key"] = 343;
	items["Match Book"] = 344;
	items["Synthoil"] = 345;
	items["A Snack"] = 346;
	return items[name];
}