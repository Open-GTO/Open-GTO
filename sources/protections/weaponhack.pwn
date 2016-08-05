/*

	About: anti weapon hack
	Author:	ziggi

*/

#if defined _weaponhack_included
	#endinput
#endif

#define _weaponhack_included


/*
 * Vars
 */

static
	IsEnabled = ANTI_WEAPON_HACK_ENABLED,
	DelayTickCount = ANTI_WEAPON_HACK_DELAY_TIME,
	LastTickCount;


/*
 * Config
 */

pt_weapon_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Protection_Weapon_IsEnabled", IsEnabled);
	ini_getInteger(file_config, "Protection_Weapon_DelayTime", DelayTickCount);
}

pt_weapon_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Protection_Weapon_IsEnabled", IsEnabled);
	ini_setInteger(file_config, "Protection_Weapon_DelayTime", DelayTickCount);
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
	#pragma unused hittype, hitid, fX, fY, fZ

	new playerstate = GetPlayerState(playerid);
	if (playerstate == PLAYER_STATE_PASSENGER) {
		switch (weaponid) {
			case WEAPON_DRIVEBY_DISABLED: {
				Admin_SendProtectReport(playerid, "PROTECTION_WEAPON_NOP_ARMED", playerstate, weaponid);
			}
		}
	}
	return 1;
}

pt_weapon_OnPlayerDeath(playerid, killerid, reason)
{
	if (playerid == killerid) {
		Admin_SendProtectReport(playerid, "PROTECTION_WEAPON_FLOOD", playerid, reason);
		return 0;
	}
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
		slotid = GetWeaponSlot(weaponid),
		current_tick = GetTickCount();

	if (weaponid == 0 || GetTickDiff(current_tick, LastTickCount) < DelayTickCount) {
		return 1;
	}

	// save last time check
	LastTickCount = current_tick;

	// weapon
	if (PlayerWeapons[playerid][slotid][pwid] != weaponid) {
		Admin_SendProtectReport(playerid, "PROTECTION_WEAPON_WEAPON", PlayerWeapons[playerid][slotid][pwid], weaponid);
	}

	// ammo
	if (PlayerWeapons[playerid][slotid][pbullets] < ammo) {
		Admin_SendProtectReport(playerid, "PROTECTION_WEAPON_AMMO", weaponid, PlayerWeapons[playerid][slotid][pbullets], ammo);
	}
	return 1;
}
