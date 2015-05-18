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


#define MAX_BULLETS				9000

enum WeaponInfo {
	Weapon_ID, // id number of weapon
	Weapon_Name[MAX_NAME], // weapon name
	Float:Weapon_Damage,
	Weapon_Allowed, // is weapon allowed
	Weapon_IsFirearm, // is weapon is firearm
	Weapon_Cost, // cost per bullet
	Weapon_Level // player level required to buy this weapon
}

new Weapons[][WeaponInfo] = {
	{0,  "Невооружен",                    1.0,   0, 0, 0,     0},
	{1,  "Кастет",                        1.0,   1, 0, 600,   4},
	{2,  "Клюшка для гольфа",             1.0,   0, 0, 600,   2},
	{3,  "Дубинка",                       1.0,   0, 0, 1000,  8},
	{4,  "Нож",                           1.0,   1, 0, 1000,  10},
	{5,  "Бейсбольная бита",              1.0,   1, 0, 1500,  7},
	{6,  "Лопата",                        1.0,   0, 0, 100,   0},
	{7,  "Кий",                           1.0,   1, 0, 100,   2},
	{8,  "Катана",                        1.0,   1, 0, 2000,  8},
	{9,  "Бензопила",                     1.0,   1, 0, 750,   9},
	{10, "Пурпурный Dildo",               1.0,   0, 0, 500,   6},
	{11, "Дилдо",                         1.0,   1, 0, 500,   5},
	{12, "Вибратор",                      1.0,   0, 0, 700,   8},
	{13, "Серебряный вибратор",           1.0,   0, 0, 500,   4},
	{14, "Букет цветов",                  1.0,   1, 0, 100,   3},
	{15, "Трость",                        1.0,   0, 0, 100,   1},

	{16, "Граната",                       82.5,  1, 1, 800,   27},
	{17, "Газовый баллон",                0.0,   0, 1, 400,   5},
	{18, "Коктейль молотова",             1.0,   0, 1, 550,   30},
	{19, "Неизвестное",                   0.0,   0, 0, 10,    30},
	{20, "Неизвестное",                   0.0,   0, 0, 10,    30},
	{21, "Неизвестное",                   0.0,   0, 0, 10,    30},
	{22, "Пистолет 9мм",                  8.25,  1, 1, 10,    0},
	{23, "Пистолет 9мм с глушителем",     13.2,  1, 1, 10,    5},
	{24, "Пустынный орёл",                46.2,  1, 1, 20,    10},
	{25, "Дробовик",                      3.3,   1, 1, 15,    5},
	{26, "Разрывной дробовик",            3.3,   1, 1, 25,    15},
	{27, "Боевой дробовик",               4.95,  1, 1, 40,    10},
	{28, "Узи",                           6.6,   1, 1, 20,    20},
	{29, "MP5",                           8.25,  1, 1, 25,    20},
	{30, "AK-47",                         9.9,   1, 1, 40,    23},
	{31, "M4",                            9.9,   1, 1, 40,    23},
	{32, "Tec-9",                         6.6,   1, 1, 30,    20},
	{33, "Винтовка",                      24.75, 1, 1, 150,   22},
	{34, "Снайперская винтовка",          41.25, 1, 1, 400,   26},
	{35, "РПГ",                           82.5,  1, 1, 1000,  30},
	{36, "Реактивная пусковая установка", 82.5,  0, 1, 1000,  29},
	{37, "Огнемёт",                       1.0,   0, 1, 200,   28},
	{38, "Ручной пулемёт(Миниган)",       46.2,  0, 1, 100,   30},
	{39, "Заряженный рюкзак",             82.5,  0, 1, 500,   30},
	{40, "Детонатор к рюкзаку",           0.0,   0, 0, 1,     30},

	{41, "Баллончик с краской",           0.33,  0, 1, 40,    5},
	{42, "Огнетушитель",                  0.33,  0, 1, 10,    5},
	{43, "Фотокамера",                    0.0,   0, 1, 60,    5},
	{44, "Очки ночного видения",          0.0,   0, 0, 10,    0},
	{45, "Тепловые очки",                 0.0,   0, 0, 10,    0},
	{46, "Парашют",                       0.0,   1, 0, 500,   1},
	{47, "Броня",                         0.0,   1, 0, 200,   10}
};

stock weapon_LoadConfig(file_config)
{
	ini_getString(file_config, "Weapon_DB", db_weapon);
}

stock weapon_SaveConfig(file_config)
{
	ini_setString(file_config, "Weapon_DB", db_weapon);
}

stock weapon_LoadAll()
{
	new file_weapons,
		db_weaponname[MAX_STRING];
	
	for (new i = 0; i < sizeof(Weapons); i++) {
		format(db_weaponname, sizeof(db_weaponname), "%sid_%d"DATA_FILES_FORMAT, db_weapon, Weapons[i][Weapon_ID]);
		if (!ini_fileExist(db_weaponname)) {
			continue;
		}

		file_weapons = ini_openFile(db_weaponname);

		ini_getString(file_weapons, "Name", Weapons[i][Weapon_Name], MAX_NAME);
		ini_getFloat(file_weapons, "Damage", Weapons[i][Weapon_Damage]);
		ini_getInteger(file_weapons, "IsAllowed", Weapons[i][Weapon_Allowed]);
		ini_getInteger(file_weapons, "Level", Weapons[i][Weapon_Level]);
		ini_getInteger(file_weapons, "Price", Weapons[i][Weapon_Cost]);

		ini_closeFile(file_weapons);
	}
	return 1;
}

stock weapon_SaveAll()
{
	new file_weapons,
		db_weaponname[MAX_STRING];

	for (new i = 0; i < sizeof(Weapons); i++) {
		format(db_weaponname, sizeof(db_weaponname), "%sid_%d"DATA_FILES_FORMAT, db_weapon, Weapons[i][Weapon_ID]);
		file_weapons = (!ini_fileExist(db_weaponname)) ? ini_createFile(db_weaponname) : ini_openFile(db_weaponname);

		ini_setString(file_weapons, "Name", Weapons[i][Weapon_Name]);
		ini_setFloat(file_weapons, "Damage", Weapons[i][Weapon_Damage]);
		ini_setInteger(file_weapons, "IsAllowed", Weapons[i][Weapon_Allowed]);
		ini_setInteger(file_weapons, "Level", Weapons[i][Weapon_Level]);
		ini_setInteger(file_weapons, "Price", Weapons[i][Weapon_Cost]);

		ini_closeFile(file_weapons);
	}
	return 1;
}

stock weapon_OnGameModeInit()
{
	weapon_LoadAll();
	Log_Game("SERVER: Weapon module init");
}

stock ReturnWeaponName(weaponid)
{
	new name[MAX_NAME],
		lookupid = GetWeaponLookupID(weaponid);
	
	if (lookupid == -1) {
		return name;
	}

	strmid(name, Weapons[lookupid][Weapon_Name], 0, strlen(Weapons[lookupid][Weapon_Name]));
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

stock Float:GetWeaponDamage(weaponid)
{
	new lookupid = GetWeaponLookupID(weaponid);
	if (lookupid == -1) {
		return 0;
	}
	return Weapons[lookupid][Weapon_Damage];
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
			format(string, sizeof(string), lang_texts[9][25], ReturnWeaponName(weaponid), GetWeaponCost(weaponid));
			SendClientMessage(playerid, COLOUR_MISC, string);
		}
	}
	return 1;
}
//
