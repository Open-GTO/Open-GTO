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
 * For public
 */

pt_weapon_OnPlayerExitVehicle(playerid, vehicleid)
{
	new Float:pos[3];
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);

	if (pos[2] >= 55.0) {
		new model = GetVehicleModel(vehicleid);
		switch (model) {
			case 592,577,511,512,520,593,553,476,519,460,513,548,425,417,487,488,497,563,447,469: {
				GivePlayerWeapon(playerid, WEAPON_PARACHUTE, 1);
			}
		}
	}
	return 1;
}

pt_weapon_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	#pragma unused playerid, hittype, hitid, fX, fY, fZ
	new slotid = GetWeaponSlot(weaponid);
	PlayerWeapons[playerid][slotid][pbullets]--;
	return 1;
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

	if (weaponid == 0) {
		return 1;
	}

	// weapon
	if (PlayerWeapons[playerid][slotid][pwid] != weaponid) {
		Admin_SendProtectReport(playerid, _(PROTECTION_WEAPON_WEAPON), PlayerWeapons[playerid][slotid][pwid], weaponid);
	}
	
	// ammo
	if (PlayerWeapons[playerid][slotid][pbullets] < ammo) {
		Admin_SendProtectReport(playerid, _(PROTECTION_WEAPON_AMMO), weaponid, PlayerWeapons[playerid][slotid][pbullets], ammo);
	}
	return 1;
}
