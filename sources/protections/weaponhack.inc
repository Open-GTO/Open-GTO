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

stock pt_weapon_OnPlayerPickUpPickup(playerid, pickupid)
{
	if (!IsEnabled) {
		return 0;
	}

	switch (GetPickupModel(pickupid)) {
		// парашют
		case 371: {
			oGivePlayerWeapon(playerid, 46, 1);
		}
		// armour
		case 1242: {
			oSetPlayerArmour(playerid, 100);
		}
	}
	return 1;
}

stock pt_weapon_OnPlayerExitVehicle(playerid, vehicleid)
{
	switch ( GetVehicleModel(vehicleid) ) {
		case 592,577,511,512,593,520,553,476,519,460,513,// самолёты
			 548,425,417,487,488,497,563,447,469: // вертолёты
		{
			oGivePlayerWeapon(playerid, 46, 1);
		}
	}
	return 1;
}

stock pt_weapon_Sync(playerid)
{
	if (!IsEnabled) {
		return 0;
	}

	new weapon[2];
	
	for (new i = 0; i < PLAYER_WEAPON_SLOTS; i++) {
		GetPlayerWeaponData(playerid, i, weapon[0], weapon[1]);

		// is weapon ammo empty
		if ( (weapon[0] == 0 || weapon[1] == 0) && PlayerWeapons[playerid][i][pwid] != 0 ) {
			PlayerWeapons[playerid][i][pbullets] = 0;
			continue;
		}

		// weapon
		if (PlayerWeapons[playerid][i][pwid] != weapon[0] && weapon[0] != 0) {
			KickPlayer(playerid, lang_texts[11][20]);
		}
		
		// ammo
		if (PlayerWeapons[playerid][i][pbullets] < weapon[1]) {
			KickPlayer(playerid, lang_texts[11][21]);
		} else {
			PlayerWeapons[playerid][i][pbullets] = weapon[1];
		}
	}
	return 1;
}
