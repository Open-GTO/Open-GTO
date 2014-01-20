/*

	About: anti weapon hack
	Author:	ziggi

*/

#if defined _weaponhack_included
	#endinput
#endif

#define _weaponhack_included
#pragma library weaponhack


static IsEnabled = ANTI_WEAPON_HACK_ENABLED;

stock pt_weapon_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Protection_Weapon_IsEnabled", IsEnabled);
}

stock pt_weapon_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Protection_Weapon_IsEnabled", IsEnabled);
}

stock pt_weapon_OnPlayerPickUpPickup(playerid,pickupid)
{
	if (!IsEnabled) {
		return 0;
	}

	switch (GetPickupModel(pickupid)) {
		// парашют
		case 371: {
			GivePlayerOwnedWeaponEx(playerid, 46, 1);
		}
		// здоровье
		case 1240: {
			oSetPlayerHealth(playerid, GetMaxHealth(playerid));
		}
		// armour
		case 1242: {
			oSetPlayerArmour(playerid, 100);
		}
	}
	return 1;
}

stock pt_weapon_OnPlayerExitVehicle(playerid,vehicleid)
{
	switch ( GetVehicleModel(vehicleid) ) {
		case 592,577,511,512,593,520,553,476,519,460,513,// самолёты
			 548,425,417,487,488,497,563,447,469: // вертолёты
		{
			GivePlayerOwnedWeaponEx(playerid, 46, 1);
		}
	}
	return 1;
}

stock pt_weapon_Sync(playerid)
{
	if (!IsEnabled) {
		return 0;
	}

	new weapon[2],
		string[MAX_STRING];
	
	for (new i = 0; i < PLAYER_WEAPON_SLOTS; i++) {
		GetPlayerWeaponData(playerid, i, weapon[0], weapon[1]);
		
		// weapon
		format(string, sizeof(string), "ac_WeaponID_Slot%d", i);
		if ( GetPVarInt(playerid, string) != weapon[0] && weapon[0] != 0 ) {
			KickPlayer(playerid, lang_texts[11][20]);
		}
		
		// ammo
		format(string, sizeof(string), "ac_WeaponAmmo_Slot%d", i);
		if (GetPVarInt(playerid, string) < weapon[1]) {
			KickPlayer(playerid, lang_texts[11][21]);
		} else {
			SetPVarInt(playerid, string, weapon[1] + 1);
		}
	}
	return 1;
}

stock oGivePlayerWeapon(playerid, weaponid, ammo)
{
	new string[MAX_STRING];
	format(string, sizeof(string), "ac_WeaponID_Slot%d", GetWeaponSlot(weaponid));
	SetPVarInt(playerid, string, weaponid);
	
	format(string, sizeof(string), "ac_WeaponAmmo_Slot%d", GetWeaponSlot(weaponid));
	SetPVarInt(playerid, string, GetPVarInt(playerid, string) + ammo);
	
	GivePlayerWeapon(playerid, weaponid, ammo);
	return 1;
}

stock oResetPlayerWeapons(playerid)
{
	new string[MAX_STRING];
	for (new i = 0; i < PLAYER_WEAPON_SLOTS; i++)
	{
		format(string, sizeof(string), "ac_WeaponID_Slot%d", i);
		DeletePVar(playerid, string);
	}
	ResetPlayerWeapons(playerid);
	return 1;
}
