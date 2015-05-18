/*

	About: anti weapon hack
	Author:	ziggi

*/

#if defined _weaponhack_included
	#endinput
#endif

#define _weaponhack_included
#pragma library weaponhack


/*
 * Vars
 */

static IsEnabled = ANTI_WEAPON_HACK_ENABLED;


/*
 * Config
 */

pt_weapon_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Protection_Weapon_IsEnabled", IsEnabled);
}

pt_weapon_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Protection_Weapon_IsEnabled", IsEnabled);
}


/*
 * For timer
 */

pt_weapon_Check(playerid)
{
	if (!IsEnabled) {
		return 0;
	}

	new
		weaponid = GetPlayerWeapon(playerid),
		ammo = GetPlayerAmmo(playerid),
		slotid = GetWeaponSlot(weaponid);

	// weapon
	if (PlayerWeapons[playerid][slotid][pwid] != weaponid && weaponid != 0) {
		Admin_SendProtectReport(playerid, _(PROTECTION_WEAPON_WEAPON), PlayerWeapons[playerid][slotid][pwid], weaponid);
	}
	
	// ammo
	if (PlayerWeapons[playerid][slotid][pbullets] < ammo) {
		Admin_SendProtectReport(playerid, _(PROTECTION_WEAPON_AMMO), weaponid, PlayerWeapons[playerid][slotid][pbullets], ammo);
	}
	return 1;
}
