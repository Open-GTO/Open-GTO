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
	{!"Unarmed",           false, false, 0,       0},
	{!"Brass Knuckles",    true,  false, 10000,   1},
	{!"Golf Club",         true,  false, 5000,    7},
	{!"Nightstick",        false, false, 7500,    9},
	{!"Knife",             true,  false, 100000,  15},
	{!"Baseball Bat",      true,  false, 5000,    6},
	{!"Shovel",            true,  false, 4000,    4},
	{!"Pool Cue",          false, false, 2000,    3},
	{!"Katana",            true,  false, 20000,   11},
	{!"Chainsaw",          true,  false, 1000000, 22},
	{!"Purple Dildo",      true,  false, 30000,   17},
	{!"Dildo",             false, false, 15000,   14},
	{!"Vibrator",          false, false, 25000,   16},
	{!"Silver Vibrator",   false, false, 15000,   13},
	{!"Flowers",           true,  false, 3000,    2},
	{!"Cane",              false, false, 8000,    12},
	// firearm
	{!"Grenade",           true,  true,  2500,    44},
	{!"Tear Gas",          false, true,  250,     10},
	{!"Molotov Cocktail",  false, true,  2000,    33},
	{!"Armour",            true,  false, 1000,    1}, // 19 (WEAPON_ARMOUR) added
	{!"Unknown",           false, false, 0,       0},
	{!"Unknown",           false, false, 0,       0},
	{!"9mm",               true,  true,  25,      0},
	{!"Silenced 9mm	",     true,  true,  10,      5},
	{!"Desert Eagle",      true,  true,  200,     21},
	{!"Shotgun",           true,  true,  30,      8},
	{!"Sawnoff Shotgun",   true,  true,  150,     25},
	{!"Combat Shotgun",    true,  true,  80,      18},
	{!"Micro SMG/Uzi",     true,  true,  65,      24},
	{!"MP5",               true,  true,  50,      20},
	{!"AK-47",             true,  true,  100,     35},
	{!"M4",                true,  true,  150,     41},
	{!"Tec-9",             true,  true,  80,      30},
	{!"Country Rifle",     true,  true,  200,     19},
	{!"Sniper Rifle",      true,  true,  500,     38},
	{!"RPG",               true,  true,  20000,   58},
	{!"HS Rocket",         false, true,  25000,   55},
	{!"Flamethrower",      false, true,  1000,    27},
	{!"Minigun",           false, true,  0,       0},
	{!"Satchel Charge",    false, true,  5000,    52},
	{!"Detonator",         false, false, 5000,    52},
	// other
	{!"Spraycan",          false, true,  20,      15},
	{!"Fire Extinguisher", false, true,  30,      23},
	{!"Camera",            true,  true,  5,       5},
	{!"Night Vis Goggles", false, false, 10,      5},
	{!"Thermal Goggles",   false, false, 10,      5},
	{!"Parachute",         true,  false, 2000,    1}
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
