/*
	Created: 05.09.06
	Aurthor: Iain Gilbert
	Modified by ZiGGi
*/


#if defined _weapons_included
	#endinput
#endif

#define _weapons_included
#pragma library weapons


#include "base"

#define MAX_BULLETS				2999
#define MAX_WEAPONS				47

new Ammunations[][CoordInfo] = {
	{291.0004,-84.5168, 1001.5156}, // LVammunation1
	{293.2710,-108.9645, 1001.5156}, // LSAmmunation1
	{290.3177,-38.7184, 1001.5156}, // LSAmmunation2
	{316.4356,-165.1367, 999.6010} // север карты
};

enum WeaponInfo {
	Weapon_ID, // id number of weapon
	Weapon_Name[MAX_NAME], // weapon name
	Weapon_Allowed, // is weapon allowed
	Weapon_MaxBullets, // max bullets
	Weapon_Cost, // cost per bullet
	Weapon_Level // player level required to buy this weapon
}

new Weapons[MAX_WEAPONS][WeaponInfo] = {
//id,	"Name" ,			Allowed, bullets, cost, level
{0, 	"Невооружен" 			, 0 , 1 , 0 , 0},
{1, 	"Кастет" 				, 1 , 1 , 6000 , 3},
{2, 	"Клюшка для гольфа" 	, 1 , 1 , 6000 , 2},
{3, 	"Night Stick" 			, 0 , 1 , 10000 , 18},
{4, 	"Нож" 					, 1 , 1 , 10000 , 4},
{5, 	"Бейсбольная Бита" 		, 1 , 1 , 15000 , 7},
{6, 	"Shovel" 				, 0 , 1 , 1000 , 0},
{7, 	"Кий" 					, 1 , 1 , 1000 , 11},
{8, 	"Меч Катана" 			, 1 , 1 , 20000 , 8},
{9, 	"Бензопила" 			, 1 , 1 , 7500 , 6},
{10, 	"Пурпурный Dildo" 		, 0 , 1 , 5000 , 9},
{11, 	"Вибратор" 				, 1 , 1 , 5000 , 5},
{12, 	"Длинный Белый Dildo" 	, 0 , 1 , 7000 , 15},
{13, 	"Вибратор 2" 			, 1 , 1 , 5000 , 11},
{14, 	"Букет Цветов" 			, 1 , 1 , 1000 , 12},
{15, 	"Трость" 				, 1 , 1 , 1000 , 8},
{16, 	"Граната" 				, 1 , 999 , 67500 , 43},
{17, 	"Газовый Баллон" 		, 1 , 999 , 20000 , 5},
{18, 	"Коктейль Молотова" 	, 1 , 999 , 30000 , 35},

{19, 	"Неизвестное" 			, 0 , 1 , 1000 , 25},
{20, 	"Неизвестное" 			, 0 , 1 , 1000 , 25},
{21, 	"Неизвестное" 			, 0 , 1 , 1000 , 25},

{22, 	"Пистолет" 				, 1 , 999 , 125 , 0},
{23, 	"Пистолет с глушителем" , 1 , 999 , 125 , 10},
{24, 	"Пустынный Орел" 		, 1 , 999 , 1200 , 12},
{25, 	"Дробовик" 				, 1 , 999 , 1100 , 5},
{26, 	"Разрывной Дробовик" 	, 1 , 999 , 2300 , 25},
{27, 	"Боевой Дробовик" 		, 1 , 999 , 3750 , 26},
{28, 	"Микро УЗИ" 			, 1 , 999 , 2100 , 30},
{29, 	"МП5(НК5)" 				, 1 , 999 , 2200 , 32},
{30, 	"АК47" 					, 1 , 999 , 1500 , 35},
{31, 	"M4" 					, 1 , 999 , 4300 , 36},
{32, 	"Tec9" 					, 1 , 999 , 4150 , 29},
{33, 	"Винтовка" 				, 1 , 999 , 11500 , 20},
{34, 	"Снайперская Винтовка" 	, 1 , 999 , 41000 , 38},
{35, 	"Гранатомет РПГ" 		, 1 , 999 , 99000 , 60},
{36, 	"Реактивная Пусковая установка" , 1 , 999 , 99000 , 52},
{37, 	"Огнемет" 				, 1 , 999 , 1000 , 28},
{38, 	"Ручной Пулемет(Миниган)" , 0 , 999 , 5000 , 60},

{39, 	"Заряженный Рюкзак" 	, 0 , 999 , 50000 , 62},
{40, 	"Детонатор к Рюкзаку" 	, 0 , 1 , 100 , 62},

{41, 	"Баллончик с краской" 	, 1 , 999 , 4000 , 5},
{42, 	"Огнетушитель" 			, 1 , 999 , 1000 , 5},
{43, 	"Фотокамера" 			, 1 , 999 , 6000 , 24},
{44, 	"Очки ночного видения"	, 0 , 1 , 1000 , 0},
{45, 	"Тепловые Очки" 		, 0 , 1 , 1000 , 0},
{46, 	"Парашют" 				, 1 , 1 , 7000 , 1}
};

stock WeaponLoadAll()
{
	if (ini_Exist(ConfigDB))
	{
		new file_weapons_cfg = ini_Open(ConfigDB);
		ini_Get(file_weapons_cfg, "Weapon_DB", WeaponDB);
		ini_Close(file_weapons_cfg);
	}
	new file_weapons, weapondbname[MAX_STRING];
	for (new i = 0; i < MAX_WEAPONS; i++)
	{
		format(weapondbname, sizeof(weapondbname), "%sid_%d"GTO_FILES_FORMAT, WeaponDB, Weapons[i][Weapon_ID]);
		if (!ini_Exist(weapondbname))
		{
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

stock WeaponSaveAll()
{
	new file_weapons_cfg = (!ini_Exist(ConfigDB)) ? ini_Create(ConfigDB) : ini_Open(ConfigDB);
	ini_Set(file_weapons_cfg, "Weapon_DB", WeaponDB);
	ini_Close(file_weapons_cfg);
	new file_weapons, weapondbname[MAX_STRING];
	for (new i = 0; i < MAX_WEAPONS; i++)
	{
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

stock weapons_OnGameModeInit()
{
	for (new id = 0; id < sizeof(Ammunations); id++)
	{
		CreatePickup(1247, 1, Ammunations[id][Coord_X], Ammunations[id][Coord_Y], Ammunations[id][Coord_Z]);
	}
	GameMSG("SERVER: Weapons module init");
}

stock weapons_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	#pragma unused oldkeys, newkeys
	weapons_ShowDialog_Select(playerid);
	return 1;
}

stock IsPlayerAtAmmunation(playerid)
{
	for (new id = 0; id < sizeof(Ammunations); id++)
	{
		if (IsPlayerInRangeOfPoint(playerid, 2, Ammunations[id][Coord_X], Ammunations[id][Coord_Y], Ammunations[id][Coord_Z]))
		{
			return id;
		}
	}
	return -1;
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
	return Weapons[lookupid][Weapon_MaxBullets];
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
	new weaponlookupid = GetWeaponLookupID(weaponid);
	if (weaponlookupid == -1)
	{
		return 0;
	}
	if (Weapons[weaponlookupid][Weapon_MaxBullets] == 1)
	{
		return 1;
	}
	return 0;
}

stock GetWeaponCost(weaponid)
{
	new weaponlookupid = GetWeaponLookupID(weaponid);
	if (weaponlookupid == -1)
	{
		return 0;
	}
	return Weapons[weaponlookupid][Weapon_Cost];
}

stock GetWeaponLookupID(weaponid)
{
	for (new i = 0; i < MAX_WEAPONS;i++)
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
	switch(weaponid)
	{
		case 0..46: return 1;
	}
	return 0;
}

stock BuyWeapon(playerid, weaponid, bullets)
{
	new string[MAX_STRING];
	if (weaponid < 1 || weaponid > 46)
	{
		SendClientMessage(playerid, COLOUR_RED, lang_texts[16][7]);
		return 0;
	}
	if (bullets < 1 || bullets > MAX_BULLETS)
	{
		SendClientMessage(playerid, COLOUR_RED, lang_texts[16][8]);
		return 0;
	}
	if (IsPlayerAtAmmunation(playerid) == -1)
	{
		SendClientMessage(playerid, COLOUR_RED, lang_texts[16][9]);
		return 0;
	}
	if (!IsPlayerAllowedWeapon(playerid, weaponid))
	{
		format(string, sizeof(string), lang_texts[16][10], oGetWeaponName(weaponid));
		SendClientMessage(playerid, COLOUR_RED, string);
		return 0;
	}
	new purchasecost = GetWeaponCost(weaponid) * bullets;
	if (purchasecost < 1)
	{
		format(string, sizeof(string), lang_texts[16][11], oGetWeaponName(weaponid));
		SendClientMessage(playerid, COLOUR_RED, string);
		return 0;
	}
	if (oGetPlayerMoney(playerid) < purchasecost)
	{
		format(string, sizeof(string), lang_texts[16][12], purchasecost);
		SendClientMessage(playerid, COLOUR_RED, string);
		return 0;
	}
	oGivePlayerMoney(playerid,-purchasecost, 1);
	GivePlayerOwnedWeapon(playerid, weaponid, (weaponid == 37) ? bullets*10 : bullets); // фикс для количества патрон огнемёта(37)
	if(bullets > 1)
	{
		format(string, sizeof(string), lang_texts[16][13], bullets, oGetWeaponName(weaponid), purchasecost);
	}
	else
	{
		format(string, sizeof(string), lang_texts[16][14], oGetWeaponName(weaponid), purchasecost);
	}
	SendClientMessage(playerid, COLOUR_RED, string);
	SendClientMessage(playerid, COLOUR_RED, lang_texts[16][15]);
	return 1;
}

stock weapons_OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch (dialogid)
	{
		case weapons_Select_DialogID: // Выбераем оружие из списка
		{
			if (!response)
			{
				return 0;
			}
			new weaponid = GetSelectedWeaponID(playerid, listitem);
			SetPVarInt(playerid, "Buy_Weapon_ID", weaponid);
			weapons_ShowDialog_Buy(playerid, weaponid);
			return 1;
		}
		case weapons_Buy_DialogID: // Покупаем, вводим патроны
		{
			if (!response)
			{
				return weapons_ShowDialog_Select(playerid);
			}
			BuyWeapon(playerid, GetPVarInt(playerid, "Buy_Weapon_ID"), strval(inputtext));
			weapons_ShowDialog_Select(playerid);
			return 1;
		}
	}
	return 1;
}

stock weapons_ShowDialog_Select(playerid)
{
	new string[1024]; // 1024 будет достаточно
	for (new weaponid = 1; weaponid < MAX_WEAPONS; weaponid++)
	{
		if (!IsPlayerAllowedWeapon(playerid, weaponid))
		{
			continue;
		}
		format(string, sizeof(string), "%s%s, цена $%d\n", string, oGetWeaponName(weaponid), GetWeaponCost(weaponid));
	}
	return ShowPlayerDialog(playerid, weapons_Select_DialogID, DIALOG_STYLE_LIST,
		"Меню оружия",
		string,
		"Выбрать", "Выход"
	);
}

stock weapons_ShowDialog_Buy(playerid, weaponid)
{
	new string[MAX_STRING];
	format(string, sizeof(string),
		"Оружие: %s\nЦена за 1 патрон: $%d\n\nВведите количество патрон:",
		oGetWeaponName(weaponid), GetWeaponCost(weaponid)
	);
	return ShowPlayerDialog(playerid, weapons_Buy_DialogID, DIALOG_STYLE_INPUT,
		"Покупка оружия",
		string,
		"Купить", "Назад"
	);
}

// узнаёт ид оружия по нажатию в диалоге
stock GetSelectedWeaponID(playerid, listitem)
{
	new k = 0;
	for (new weaponid = 1; weaponid < MAX_WEAPONS; weaponid++)
	{
		if (IsPlayerAllowedWeapon(playerid, weaponid))
		{
			k++;
			if (listitem+1 == k)
			{
				return weaponid;
			}
		}
	}
	return 0;
}
//

// показывает доступное оружие на уровне
stock ShowWeaponsOnLevel(playerid, level)
{
	new string[MAX_STRING],
		wepfound;
	
	for (new weaponid = 1; weaponid < MAX_WEAPONS; weaponid++)
	{
		new lookupid = GetWeaponLookupID(weaponid);
		if (lookupid == -1 || Weapons[lookupid][Weapon_Allowed] == 0)
		{
			continue;
		}
		if (Weapons[lookupid][Weapon_Level] == level)
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
