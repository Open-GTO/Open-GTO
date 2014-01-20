/*
	Created: 05.09.06
	Aurthor: Iain Gilbert
	Modified by ziggi
*/


#if defined _weapons_included
	#endinput
#endif

#define _weapons_included
#pragma library weapons


#include "base"

#define MAX_BULLETS				9000

enum WeaponInfo {
	Weapon_ID, // id number of weapon
	Weapon_Name[MAX_NAME], // weapon name
	Weapon_Allowed, // is weapon allowed
	Weapon_IsFirearm, // is weapon is firearm
	Weapon_Cost, // cost per bullet
	Weapon_Level // player level required to buy this weapon
}

new Weapons[][WeaponInfo] = {
	{0,  "Невооружен" 			, 0, 0, 0,     0},
	{1,  "Кастет" 				, 1, 0, 600,   3},
	{2,  "Клюшка для гольфа" 	, 1, 0, 600,   2},
	{3,  "Night Stick" 			, 0, 0, 1000,  18},
	{4,  "Нож" 					, 1, 0, 1000,  4},
	{5,  "Бейсбольная Бита" 	, 1, 0, 1500,  7},
	{6,  "Shovel" 				, 0, 0, 100,   0},
	{7,  "Кий" 					, 1, 0, 100,   11},
	{8,  "Меч Катана" 			, 1, 0, 2000,  8},
	{9,  "Бензопила" 			, 1, 0, 750,   6},
	{10, "Пурпурный Dildo" 		, 0, 0, 500,   9},
	{11, "Вибратор" 			, 1, 0, 500,   5},
	{12, "Длинный Белый Dildo" 	, 0, 0, 700,   15},
	{13, "Вибратор 2" 			, 1, 0, 500,   11},
	{14, "Букет Цветов" 		, 1, 0, 100,   12},
	{15, "Трость" 				, 1, 0, 100,   8},
	{16, "Граната" 				, 1, 1, 800,   43},
	{17, "Газовый Баллон" 		, 1, 1, 400,   5},
	{18, "Коктейль Молотова" 	, 1, 1, 550,   35},
		 
	{19, "Неизвестное" 			, 0, 0, 10,    25},
	{20, "Неизвестное" 			, 0, 0, 10,    25},
	{21, "Неизвестное" 			, 0, 0, 10,    25},
		 
	{22, "Пистолет" 			, 1, 1, 10,    0},
	{23, "Пистолет с глушителем", 1, 1, 10,    10},
	{24, "Пустынный Орел" 		, 1, 1, 20,    12},
	{25, "Дробовик" 			, 1, 1, 15,    5},
	{26, "Разрывной Дробовик" 	, 1, 1, 25,    25},
	{27, "Боевой Дробовик" 		, 1, 1, 40,    26},
	{28, "Микро УЗИ" 			, 1, 1, 20,    30},
	{29, "МП5(НК5)" 			, 1, 1, 25,    32},
	{30, "АК47" 				, 1, 1, 40,    35},
	{31, "M4" 					, 1, 1, 40,    36},
	{32, "Tec9" 				, 1, 1, 30,    29},
	{33, "Винтовка" 			, 1, 1, 150,   20},
	{34, "Снайперская Винтовка" , 1, 1, 400,   38},
	{35, "Гранатомет РПГ" 		, 1, 1, 1000,  60},
	{36, "Реактивная Пусковая установка", 0, 1, 1000, 52},
	{37, "Огнемет" 				, 1, 1, 200,   28},
	{38, "Ручной Пулемет(Миниган)", 0, 1, 100, 60},
		 
	{39, "Заряженный Рюкзак" 	, 0, 1, 500,   62},
	{40, "Детонатор к Рюкзаку" 	, 0, 0, 1,     62},
		 
	{41, "Баллончик с краской" 	, 1, 1, 40,    5},
	{42, "Огнетушитель" 		, 1, 1, 10,    5},
	{43, "Фотокамера" 			, 1, 1, 60,    24},
	{44, "Очки ночного видения"	, 0, 0, 10,    0},
	{45, "Тепловые Очки" 		, 0, 0, 10,    0},
	{46, "Парашют" 				, 1, 0, 500,   1},
	{47, "Броня" 				, 1, 0, 200,   10}
};

stock weapon_LoadConfig(file_config)
{
	ini_Get(file_config, "Weapon_DB", WeaponDB);
}

stock weapon_SaveConfig(file_config)
{
	ini_Set(file_config, "Weapon_DB", WeaponDB);
}

stock weapon_LoadAll()
{
	new file_weapons,
		weapondbname[MAX_STRING];
	
	for (new i = 0; i < sizeof(Weapons); i++) {
		format(weapondbname, sizeof(weapondbname), "%sid_%d"GTO_FILES_FORMAT, WeaponDB, Weapons[i][Weapon_ID]);
		if (!ini_Exist(weapondbname)) {
			continue;
		}

		file_weapons = ini_Open(weapondbname);

		ini_Get(file_weapons, "Name", Weapons[i][Weapon_Name], MAX_NAME);
		ini_GetInt(file_weapons, "IsAllowed", Weapons[i][Weapon_Allowed]);
		ini_GetInt(file_weapons, "Level", Weapons[i][Weapon_Level]);
		ini_GetInt(file_weapons, "Price", Weapons[i][Weapon_Cost]);

		ini_Close(file_weapons);
	}
	return 1;
}

stock weapon_SaveAll()
{
	new file_weapons,
		weapondbname[MAX_STRING];

	for (new i = 0; i < sizeof(Weapons); i++) {
		format(weapondbname, sizeof(weapondbname), "%sid_%d"GTO_FILES_FORMAT, WeaponDB, Weapons[i][Weapon_ID]);
		file_weapons = (!ini_Exist(weapondbname)) ? ini_Create(weapondbname) : ini_Open(weapondbname);

		ini_Set(file_weapons, "Name", Weapons[i][Weapon_Name]);
		ini_SetInt(file_weapons, "IsAllowed", Weapons[i][Weapon_Allowed]);
		ini_SetInt(file_weapons, "Level", Weapons[i][Weapon_Level]);
		ini_SetInt(file_weapons, "Price", Weapons[i][Weapon_Cost]);

		ini_Close(file_weapons);
	}
	return 1;
}

stock weapon_OnGameModeInit()
{
	weapon_LoadAll();
	GameMSG("SERVER: Weapon module init");
}

stock oGetWeaponName(weaponid)
{
	new name[MAX_NAME],
		lookupid = GetWeaponLookupID(weaponid);
	
	if (lookupid == -1)
	{
		return name;
	}
	set(name, Weapons[lookupid][Weapon_Name]);
	return name;
}

stock GetWeaponMaxAmmo(weaponid)
{
	new lookupid = GetWeaponLookupID(weaponid);
	if (lookupid == -1)
	{
		return 0;
	}
	if (Weapons[lookupid][Weapon_IsFirearm] == 1)
	{
		return MAX_BULLETS;
	}
	return 1;
}

stock IsPlayerAllowedWeapon(playerid, weaponid)
{
	new lookupid = GetWeaponLookupID(weaponid);
	if (lookupid == -1 || Weapons[lookupid][Weapon_Allowed] == 0 || Weapons[lookupid][Weapon_Level] > GetPlayerLevel(playerid))
	{
		return 0;
	}
	return 1;
}

stock IsWeaponHandToHand(weaponid)
{
	new lookupid = GetWeaponLookupID(weaponid);
	if (lookupid == -1)
	{
		return 0;
	}
	if (Weapons[lookupid][Weapon_IsFirearm] == 0)
	{
		return 1;
	}
	return 0;
}

stock GetWeaponCost(weaponid)
{
	new lookupid = GetWeaponLookupID(weaponid);
	if (lookupid == -1)
	{
		return 0;
	}
	return Weapons[lookupid][Weapon_Cost];
}

stock GetWeaponLookupID(weaponid)
{
	for (new i = 0; i < sizeof(Weapons); i++)
	{
		if (weaponid == Weapons[i][Weapon_ID])
		{
			return i;
		}
	}
	return -1;
}

stock IsWeapon(weaponid)
{
	switch (weaponid)
	{
		case 0..46: return 1;
	}
	return 0;
}

// показывает доступное оружие на уровне
stock ShowWeaponsOnLevel(playerid, newlevel, oldlevel)
{
	new string[MAX_STRING],
		wepfound = 0;
	
	for (new weaponid = 1; weaponid < sizeof(Weapons); weaponid++)
	{
		new lookupid = GetWeaponLookupID(weaponid);
		if (lookupid == -1 || Weapons[lookupid][Weapon_Allowed] == 0)
		{
			continue;
		}
		if (Weapons[lookupid][Weapon_Level] > oldlevel && Weapons[lookupid][Weapon_Level] <= newlevel)
		{
			if (wepfound == 0)
			{
				SendClientMessage(playerid, COLOUR_GREEN, lang_texts[9][24]);
				wepfound = 1;
			}
			format(string, sizeof(string), lang_texts[9][25], oGetWeaponName(weaponid), GetWeaponCost(weaponid));
			SendClientMessage(playerid, COLOUR_MISC, string);
		}
	}
	return 1;
}
//
