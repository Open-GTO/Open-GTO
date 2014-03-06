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

COMMAND:whack(playerid, params[])
{
	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][3]);
		return 1;
	}

	new idx = 0;
	new receiverid = strval(strcharsplit(params, idx, ' '));

	if (!IsPlayerConnected(receiverid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][2]);
		return 1;
	}
	
	new weaponid = strval(strcharsplit(params, idx, ' '));
	if (weaponid < 0 && weaponid > 46) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[12][45]);
		return 1;
	}
	
	new ammamount = strval(strcharsplit(params, idx, ' '));
	if (ammamount == 0) {
		ammamount = 10;
	}

	GivePlayerWeapon(receiverid, weaponid, ammamount);

	new string[MAX_STRING];
	format(string, sizeof(string), lang_texts[12][46], oGetPlayerName(playerid), oGetWeaponName(weaponid), weaponid, ammamount);
	SendClientMessage(receiverid, COLOUR_XP_GOOD, string);
	
	format(string, sizeof(string), lang_texts[12][47], oGetPlayerName(receiverid), receiverid, oGetWeaponName(weaponid), weaponid, ammamount);
	SendClientMessage(playerid, COLOUR_XP_GOOD, string);
	return 1;
}