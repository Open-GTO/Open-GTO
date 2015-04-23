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

pt_weapon_Check(playerid, slotid, weapon[2])
{
	if (!IsEnabled) {
		return 0;
	}

	// weapon
	if (PlayerWeapons[playerid][slotid][pwid] != weapon[0] && weapon[0] != 0) {
		KickPlayer(playerid, lang_texts[11][20]);
	}
	
	// ammo
	if (PlayerWeapons[playerid][slotid][pbullets] < weapon[1]) {
		KickPlayer(playerid, lang_texts[11][21]);
	}
	return 1;
}
