/*

	About: weapons system
	Author: ziggi

*/

#if defined _weapons_included
	#endinput
#endif

#define _weapons_included

/*
	Defines
*/

#define MAX_WEAPONS     47
#define MAX_WEAPON_NAME 32
#define MAX_WEAPON_AMMO 9000

#define WEAPON_ARMOUR   19

/*
	Enums
*/

enum e_Weapon_Info {
	e_wName[MAX_WEAPON_NAME char],
	bool:e_wIsAllowed,
	bool:e_wIsFirearm,
	e_wCost,
	e_wLevel,
}

/*
	Vars
*/

static gWeapons[MAX_WEAPONS][e_Weapon_Info] = {
	// hand to hand
	{!"Unarmed",           false, false, 0,     0},
	{!"Brass Knuckles",    true,  false, 600,   4},
	{!"Golf Club",         false, false, 600,   2},
	{!"Nightstick",        false, false, 1000,  8},
	{!"Knife",             true,  false, 1000,  10},
	{!"Baseball Bat",      true,  false, 1500,  7},
	{!"Shovel",            false, false, 100,   0},
	{!"Pool Cue",          true,  false, 100,   2},
	{!"Katana",            true,  false, 2000,  8},
	{!"Chainsaw",          true,  false, 750,   9},
	{!"Purple Dildo",      false, false, 500,   6},
	{!"Dildo",             true,  false, 500,   5},
	{!"Vibrator",          false, false, 700,   8},
	{!"Silver Vibrator",   false, false, 500,   4},
	{!"Flowers",           true,  false, 100,   3},
	{!"Cane",              false, false, 100,   1},
	// firearm
	{!"Grenade",           true,  true,  800,   27},
	{!"Tear Gas",          false, true,  400,   5},
	{!"Molotov Cocktail",  false, true,  550,   30},
	{!"Armour",            true,  false, 200,   10}, // 19 (WEAPON_ARMOUR) added
	{!"Unknown",           false, false, 10,    30},
	{!"Unknown",           false, false, 10,    30},
	{!"9mm",               true,  true,  10,    0},
	{!"Silenced 9mm	",     true,  true,  10,    5},
	{!"Desert Eagle",      true,  true,  20,    10},
	{!"Shotgun",           true,  true,  15,    5},
	{!"Sawnoff Shotgun",   true,  true,  25,    15},
	{!"Combat Shotgun",    true,  true,  40,    10},
	{!"Micro SMG/Uzi",     true,  true,  30,    20},
	{!"MP5",               true,  true,  25,    20},
	{!"AK-47",             true,  true,  40,    23},
	{!"M4",                true,  true,  40,    23},
	{!"Tec-9",             true,  true,  30,    20},
	{!"Country Rifle",     true,  true,  150,   22},
	{!"Sniper Rifle",      true,  true,  400,   26},
	{!"RPG",               true,  true,  1000,  30},
	{!"HS Rocket",         false, true,  1000,  29},
	{!"Flamethrower",      false, true,  200,   28},
	{!"Minigun",           false, true,  100,   30},
	{!"Satchel Charge",    false, true,  500,   30},
	{!"Detonator",         false, false, 1,     30},
	// other
	{!"Spraycan",          false, true,  40,    5},
	{!"Fire Extinguisher", false, true,  10,    5},
	{!"Camera",            false, true,  60,    5},
	{!"Night Vis Goggles", false, false, 10,    0},
	{!"Thermal Goggles",   false, false, 10,    0},
	{!"Parachute",         true,  false, 500,   1}
};

/*
	Config
*/

Weapon_LoadConfig(file_config)
{
	ini_getString(file_config, "Weapon_DB", db_weapon);
}

Weapon_SaveConfig(file_config)
{
	ini_setString(file_config, "Weapon_DB", db_weapon);
}

/*
	Weapon_Load
*/

Weapon_Load()
{
	new
		file_handler,
		temp,
		weapon_config[MAX_STRING];

	for (new i = 0; i < MAX_WEAPONS; i++) {
		format(weapon_config, sizeof(weapon_config), "%sid_%d"DATA_FILES_FORMAT, db_weapon, i);

		file_handler = ini_openFile(weapon_config);

		if (file_handler < 0) {
			continue;
		}

		ini_getInteger(file_handler, "IsAllowed", temp);
		SetWeaponAllowedStatus(i, bool:temp);

		ini_getInteger(file_handler, "Cost", temp);
		SetWeaponCost(i, temp);

		ini_getInteger(file_handler, "Level", temp);
		SetWeaponLevel(i, temp);

		ini_closeFile(file_handler);
	}
	return 1;
}

/*
	Weapon_Save
*/

Weapon_Save()
{
	new
		file_handler,
		weapon_config[MAX_STRING];

	for (new i = 0; i < MAX_WEAPONS; i++) {
		format(weapon_config, sizeof(weapon_config), "%sid_%d"DATA_FILES_FORMAT, db_weapon, i);

		file_handler = ini_createFile(weapon_config);

		if (file_handler < 0) {
			file_handler = ini_openFile(weapon_config);
		}

		if (file_handler < 0) {
			Log(systemlog, DEBUG, "Error <Weapon_Save>: unable to open or create ini file.");
			continue;
		}

		ini_setInteger(file_handler, "IsAllowed", _:IsWeaponAllowed(i));
		ini_setInteger(file_handler, "Cost", GetWeaponCost(i));
		ini_setInteger(file_handler, "Level", GetWeaponLevel(i));

		ini_closeFile(file_handler);
	}
}

/*
	Weapon_OnGameModeInit
*/

Weapon_OnGameModeInit()
{
	Weapon_Load();
	EnableVehicleFriendlyFire();

	Log_Init("system", "Weapon module init.");
}

/*
	Weapon_OnPlayerStateChange
*/

Weapon_OnPlayerStateChange(playerid, newstate, oldstate)
{
	#pragma unused oldstate
	if (newstate == PLAYER_STATE_PASSENGER) {
		new weaponid = GetPlayerWeapon(playerid);

		// check semi automatic
		new
			semi_automatic_id,
			semi_automatic_ammo,
			bool:is_disabled_semi_automatic = false;

		GetPlayerWeaponData(playerid, 4, semi_automatic_id, semi_automatic_ammo);

		switch (semi_automatic_id) {
			case WEAPON_DRIVEBY_DISABLED: {
				is_disabled_semi_automatic = true;
			}
		}

		// set armed weapon
		switch (weaponid) {
			case WEAPON_DRIVEBY_DISABLED: {
				if (!is_disabled_semi_automatic) {
					SetPlayerArmedWeapon(playerid, semi_automatic_id);
				} else {
					SetPlayerArmedWeapon(playerid, 0);
				}
			}
		}
	}

	return 1;
}

/*
	Weapon max ammo
*/

stock GetWeaponMaxAmmo(weaponid)
{
	if (!IsValidWeapon(weaponid)) {
		return 0;
	}

	if (IsWeaponHandToHand(weaponid)) {
		return 1;
	}

	return MAX_WEAPON_AMMO;
}

/*
	Weapon allowed for player
*/

stock IsPlayerAllowedWeapon(playerid, weaponid)
{
	if (!IsValidWeapon(weaponid)) {
		return 0;
	}

	if (!IsWeaponAllowed(weaponid) || GetWeaponLevel(weaponid) > GetPlayerLevel(playerid)) {
		return 0;
	}

	return 1;
}

/*
	Weapon firearm status
*/

stock IsWeaponHandToHand(weaponid)
{
	if (!IsValidWeapon(weaponid)) {
		return 0;
	}

	if (gWeapons[weaponid][e_wIsFirearm]) {
		return 0;
	}

	return 1;
}

/*
	Weapon name
*/

stock ret_GetPlayerWeaponName(playerid, weaponid)
{
	new name[MAX_WEAPON_NAME];
	GetPlayerWeaponName(playerid, weaponid, name, sizeof(name));
	return name;
}

stock GetPlayerWeaponName(playerid, weaponid, weapon[], len)
{
	if (!IsValidWeapon(weaponid)) {
		return 0;
	}

	new
		is_exists,
		string[MAX_WEAPON_NAME];

	format(string, sizeof(string), "WEAPON_NAME_%d", weaponid);
	is_exists = Lang_GetPlayerText(playerid, string, weapon, len);

	if (!is_exists) {
		strunpack(weapon, gWeapons[weaponid][e_wName], len);
	}

	return 1;
}

/*
	Weapon allowed status
*/

stock IsWeaponAllowed(weaponid)
{
	if (!IsValidWeapon(weaponid)) {
		return 0;
	}

	if (!gWeapons[weaponid][e_wIsAllowed]) {
		return 0;
	}

	return 1;
}

stock SetWeaponAllowedStatus(weaponid, bool:status)
{
	if (!IsValidWeapon(weaponid)) {
		return 0;
	}

	gWeapons[weaponid][e_wIsAllowed] = status;
	return 1;
}

/*
	Weapon cost
*/

stock GetWeaponCost(weaponid)
{
	if (!IsValidWeapon(weaponid)) {
		return 0;
	}

	return gWeapons[weaponid][e_wCost];
}

stock SetWeaponCost(weaponid, cost)
{
	if (!IsValidWeapon(weaponid)) {
		return 0;
	}

	gWeapons[weaponid][e_wCost] = cost;
	return 1;
}

/*
	Weapon level
*/

stock GetWeaponLevel(weaponid)
{
	if (!IsValidWeapon(weaponid)) {
		return 0;
	}

	return gWeapons[weaponid][e_wLevel];
}

stock SetWeaponLevel(weaponid, level)
{
	if (!IsValidWeapon(weaponid)) {
		return 0;
	}

	gWeapons[weaponid][e_wLevel] = level;
	return 1;
}

/*
	Weapon validation
*/

stock IsValidWeapon(weaponid)
{
	if (0 <= weaponid < MAX_WEAPONS) {
		return 1;
	}

	return 0;
}
