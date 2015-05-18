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
	Weapon_Name[MAX_NAME], // weapon name
	Float:Weapon_Damage,
	Weapon_Allowed, // is weapon allowed
	Weapon_IsFirearm, // is weapon is firearm
	Weapon_Cost, // cost per bullet
	Weapon_Level // player level required to buy this weapon
}

new Weapons[][WeaponInfo] = {
	{"Невооружен",                    1.0,   0, 0, 0,     0},
	{"Кастет",                        1.0,   1, 0, 600,   4},
	{"Клюшка для гольфа",             1.0,   0, 0, 600,   2},
	{"Дубинка",                       1.0,   0, 0, 1000,  8},
	{"Нож",                           1.0,   1, 0, 1000,  10},
	{"Бейсбольная бита",              1.0,   1, 0, 1500,  7},
	{"Лопата",                        1.0,   0, 0, 100,   0},
	{"Кий",                           1.0,   1, 0, 100,   2},
	{"Катана",                        1.0,   1, 0, 2000,  8},
	{"Бензопила",                     1.0,   1, 0, 750,   9},
	{"Пурпурный Dildo",               1.0,   0, 0, 500,   6},
	{"Дилдо",                         1.0,   1, 0, 500,   5},
	{"Вибратор",                      1.0,   0, 0, 700,   8},
	{"Серебряный вибратор",           1.0,   0, 0, 500,   4},
	{"Букет цветов",                  1.0,   1, 0, 100,   3},
	{"Трость",                        1.0,   0, 0, 100,   1},

	{"Граната",                       82.5,  1, 1, 800,   27},
	{"Газовый баллон",                0.0,   0, 1, 400,   5},
	{"Коктейль молотова",             1.0,   0, 1, 550,   30},
	{"Неизвестное",                   0.0,   0, 0, 10,    30},
	{"Неизвестное",                   0.0,   0, 0, 10,    30},
	{"Неизвестное",                   0.0,   0, 0, 10,    30},
	{"Пистолет 9мм",                  8.25,  1, 1, 10,    0},
	{"Пистолет 9мм с глушителем",     13.2,  1, 1, 10,    5},
	{"Пустынный орёл",                46.2,  1, 1, 20,    10},
	{"Дробовик",                      3.3,   1, 1, 15,    5},
	{"Разрывной дробовик",            3.3,   1, 1, 25,    15},
	{"Боевой дробовик",               4.95,  1, 1, 40,    10},
	{"Узи",                           6.6,   1, 1, 20,    20},
	{"MP5",                           8.25,  1, 1, 25,    20},
	{"AK-47",                         9.9,   1, 1, 40,    23},
	{"M4",                            9.9,   1, 1, 40,    23},
	{"Tec-9",                         6.6,   1, 1, 30,    20},
	{"Винтовка",                      24.75, 1, 1, 150,   22},
	{"Снайперская винтовка",          41.25, 1, 1, 400,   26},
	{"РПГ",                           82.5,  1, 1, 1000,  30},
	{"Реактивная пусковая установка", 82.5,  0, 1, 1000,  29},
	{"Огнемёт",                       1.0,   0, 1, 200,   28},
	{"Ручной пулемёт(Миниган)",       46.2,  0, 1, 100,   30},
	{"Заряженный рюкзак",             82.5,  0, 1, 500,   30},
	{"Детонатор к рюкзаку",           0.0,   0, 0, 1,     30},

	{"Баллончик с краской",           0.33,  0, 1, 40,    5},
	{"Огнетушитель",                  0.33,  0, 1, 10,    5},
	{"Фотокамера",                    0.0,   0, 1, 60,    5},
	{"Очки ночного видения",          0.0,   0, 0, 10,    0},
	{"Тепловые очки",                 0.0,   0, 0, 10,    0},
	{"Парашют",                       0.0,   1, 0, 500,   1},
	{"Броня",                         0.0,   1, 0, 200,   10}
};

forward Float:GetWeaponDamage(weaponid);


weapon_LoadConfig(file_config)
{
	ini_getString(file_config, "Weapon_DB", db_weapon);
}

weapon_SaveConfig(file_config)
{
	ini_setString(file_config, "Weapon_DB", db_weapon);
}

weapon_LoadAll()
{
	new file_weapons,
		db_weaponname[MAX_STRING];
	
	for (new i = 0; i < sizeof(Weapons); i++) {
		format(db_weaponname, sizeof(db_weaponname), "%sid_%d"DATA_FILES_FORMAT, db_weapon, i);
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

weapon_SaveAll()
{
	new file_weapons,
		db_weaponname[MAX_STRING];

	for (new i = 0; i < sizeof(Weapons); i++) {
		format(db_weaponname, sizeof(db_weaponname), "%sid_%d"DATA_FILES_FORMAT, db_weapon, i);
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

weapon_OnGameModeInit()
{
	weapon_LoadAll();
	EnableVehicleFriendlyFire();
	Log_Game("SERVER: Weapon module init");
}

weapon_OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart)
{
	#pragma unused bodypart

	if (damagedid == INVALID_PLAYER_ID) {
		return 1;
	}

	new
		Float:health,
		Float:armour,
		Float:difference,
		Float:damage = GetWeaponDamage(weaponid);

	if (amount != damage) {
		Admin_SendProtectReport(playerid, _(PROTECTION_WEAPON_DAMAGE), amount, damage);
	}

	if (GetPlayerState(playerid) == PLAYER_STATE_DRIVER) {
		damage /= 2.0;
	}

	GetPlayerHealth(damagedid, health);
	GetPlayerArmour(damagedid, armour);

	if (armour > 0.0) {
		difference = armour - damage;
	}

	if (difference >= 0.0) {
		SetPlayerArmour(damagedid, armour - damage);
	} else {
		SetPlayerArmour(damagedid, 0.0);
		SetPlayerHealth(damagedid, health + difference);
	}
	return 1;
}

weapon_OnPlayerSpawn(playerid)
{
	if (GetPlayerTeam(playerid) != 0xFE) {
		SetPlayerTeam(playerid, 0xFE);
	}
	return 1;
}

weapon_OnPlayerStateChange(playerid, newstate, oldstate)
{
	#pragma unused oldstate
	if (newstate != PLAYER_STATE_PASSENGER) {
		return 1;
	}

	new weaponid = GetPlayerWeapon(playerid);
	switch (weaponid) {
		case WEAPON_COLT45, WEAPON_SILENCED, WEAPON_DEAGLE, WEAPON_SHOTGSPA: {
			SetPlayerArmedWeapon(playerid, 0);
		}
	}

	return 1;
}

weapon_OnPlayerExitVehicle(playerid, vehicleid)
{
	new Float:pos[3];
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);

	if (pos[2] >= 55.0) {
		new model = GetVehicleModel(vehicleid);
		switch (model) {
			case 592,577,511,512,520,593,553,476,519,460,513,548,425,417,487,488,497,563,447,469: {
				GivePlayerWeapon(playerid, 46, 1);
			}
		}
	}
	return 1;
}

stock ReturnWeaponName(weaponid)
{
	new name[MAX_NAME];
	name = "Unknown";

	if (!IS_IN_RANGE(weaponid, 0, sizeof(Weapons))) {
		return name;
	}

	strmid(name, Weapons[weaponid][Weapon_Name], 0, strlen(Weapons[weaponid][Weapon_Name]));
	return name;
}

stock GetWeaponMaxAmmo(weaponid)
{
	if (!IS_IN_RANGE(weaponid, 0, sizeof(Weapons))) {
		return 0;
	}

	if (Weapons[weaponid][Weapon_IsFirearm] == 1) {
		return MAX_BULLETS;
	}

	return 1;
}

stock IsPlayerAllowedWeapon(playerid, weaponid)
{
	if (!IS_IN_RANGE(weaponid, 0, sizeof(Weapons))) {
		return 0;
	}

	if (Weapons[weaponid][Weapon_Allowed] == 0 || Weapons[weaponid][Weapon_Level] > GetPlayerLevel(playerid)) {
		return 0;
	}

	return 1;
}

stock IsWeaponHandToHand(weaponid)
{
	if (!IS_IN_RANGE(weaponid, 0, sizeof(Weapons))) {
		return 0;
	}

	if (Weapons[weaponid][Weapon_IsFirearm] == 0) {
		return 1;
	}

	return 0;
}

stock GetWeaponCost(weaponid)
{
	if (!IS_IN_RANGE(weaponid, 0, sizeof(Weapons))) {
		return 0;
	}

	return Weapons[weaponid][Weapon_Cost];
}

stock Float:GetWeaponDamage(weaponid)
{
	if (!IS_IN_RANGE(weaponid, 0, sizeof(Weapons))) {
		return 0.0;
	}
	return Weapons[weaponid][Weapon_Damage];
}

stock IsWeapon(weaponid)
{
	if (IS_IN_RANGE(weaponid, 0, 46)) {
		return 1;
	}
	return 0;
}

// показывает доступное оружие на уровне
stock ShowWeaponsOnLevel(playerid, newlevel, oldlevel)
{
	new
		string[MAX_STRING],
		wepfound = 0;
	
	for (new weaponid = 1; weaponid < sizeof(Weapons); weaponid++) {
		if (Weapons[weaponid][Weapon_Allowed] == 0) {
			continue;
		}

		if (Weapons[weaponid][Weapon_Level] > oldlevel && Weapons[weaponid][Weapon_Level] <= newlevel) {
			if (wepfound == 0) {
				SendClientMessage(playerid, COLOUR_GREEN, lang_texts[9][24]);
				wepfound = 1;
			}
			format(string, sizeof(string), lang_texts[9][25], ReturnWeaponName(weaponid), GetWeaponCost(weaponid));
			SendClientMessage(playerid, COLOUR_MISC, string);
		}
	}
	return 1;
}
