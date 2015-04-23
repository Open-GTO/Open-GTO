/*

	About: protection
	Author:	ziggi

*/

#if defined _redefine_included
	#endinput
#endif

#define _redefine_included
#pragma library redefine


/*
 * Weapon
 */

native ORIG_GivePlayerWeapon(playerid, weaponid, ammo) = GivePlayerWeapon;
// fixes.inc compatible
#if FIX_HydraSniper
	stock REDEF_GivePlayerWeapon(playerid, weaponid, ammo, bool:allowcheck = false)
	{
		return
			FIXES_GivePlayerWeapon(playerid, weaponid, ammo),
			pl_weapon_GivePlayerWeapon(playerid, weaponid, ammo, allowcheck);
	}
	
	#undef GivePlayerWeapon
	#define GivePlayerWeapon REDEF_GivePlayerWeapon
#else
	#define GivePlayerWeapon pl_weapon_GivePlayerWeapon
#endif

native ORIG_ResetPlayerWeapons(playerid) = ResetPlayerWeapons;
#define ResetPlayerWeapons pl_weapon_ResetPlayerWeapons

/*
 * Money
 */

native ORIG_GivePlayerMoney(playerid, money) = GivePlayerMoney;
#define GivePlayerMoney pl_money_GivePlayerMoney

native ORIG_GetPlayerMoney(playerid) = GetPlayerMoney;
#define GetPlayerMoney pl_money_GetPlayerMoney

/*
 * Health
 */

native ORIG_SetPlayerHealth(playerid, Float:health) = SetPlayerHealth;
#define SetPlayerHealth pt_SetPlayerHealth

native ORIG_GetPlayerHealth(playerid, &Float:health) = GetPlayerHealth;
#define GetPlayerHealth pt_GetPlayerHealth


/*
 * Armour
 */

native ORIG_SetPlayerArmour(playerid, Float:armour) = SetPlayerArmour;
#define SetPlayerArmour pt_SetPlayerArmour

native ORIG_GetPlayerArmour(playerid, &Float:armour) = GetPlayerArmour;
#define GetPlayerArmour pt_GetPlayerArmour


/*
 * Pos
 */

native ORIG_SetPlayerPos(playerid, Float:x, Float:y, Float:z) = SetPlayerPos;
#define SetPlayerPos pt_SetPlayerPos

native ORIG_SetVehiclePos(playerid, Float:x, Float:y, Float:z) = SetVehiclePos;
#define SetVehiclePos pt_SetVehiclePos