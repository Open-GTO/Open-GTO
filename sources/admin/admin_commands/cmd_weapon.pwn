/*

	About: weapon admin command
	Author: ziggi

*/

#if defined _admin_cmd_weapon_included
	#endinput
#endif

#define _admin_cmd_weapon_included

/*
	Defines
*/

#if !defined PLAYER_WEAPON_SLOTS
	#define PLAYER_WEAPON_SLOTS 13
#endif

/*
	Command
*/

COMMAND:weapon(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
		return 0;
	}

	new
		subcmd[5],
		subparams[32],
		weaponid,
		amount;

	if (sscanf(params, "s[5]s[32]K<weapon>(-1)I(0)", subcmd, subparams, weaponid, amount)) {
		Lang_SendText(playerid, "ADMIN_COMMAND_WEAPON_HELP");
		return 1;
	}

	new
		targetid = INVALID_PLAYER_ID;

	if (strcmp(subparams, "all", true) == 0) {
		targetid = -1;
	} else if (sscanf(subparams, "u", targetid) || targetid == INVALID_PLAYER_ID) {
		Lang_SendText(playerid, "ADMIN_COMMAND_WEAPON_TARGET_ERROR");
		return 1;
	}

	new
		targetname[MAX_PLAYER_NAME + 1],
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(playerid, playername, sizeof(playername));

	if (targetid != -1) {
		GetPlayerName(targetid, targetname, sizeof(targetname));
	}

	if (strcmp(subcmd, "set", true) == 0) {
		if (weaponid == -1) {
			Lang_SendText(playerid, "ADMIN_COMMAND_WEAPON_TARGET_ERROR");
			return 1;
		}
		if (targetid == -1) {
			foreach (new id : Player) {
				SetPlayerWeapon(id, weaponid, amount);
			}

			Lang_SendTextToAll("ADMIN_COMMAND_WEAPON_SET_ALL", playername, playerid, weaponid, amount);
		} else {
			SetPlayerWeapon(targetid, weaponid, amount);

			Lang_SendText(targetid, "ADMIN_COMMAND_WEAPON_SET_PLAYER", playername, playerid, weaponid, amount);
			Lang_SendText(playerid, "ADMIN_COMMAND_WEAPON_SET_SELF", targetname, targetid, weaponid, amount);
		}
	} else if (strcmp(subcmd, "get", true) == 0) {
		if (!IsPlayerConnected(targetid)) {
			Lang_SendText(playerid, "ADMIN_COMMAND_WEAPON_TARGET_ERROR");
			return 1;
		}

		Lang_SendText(playerid, "ADMIN_COMMAND_WEAPON_GET", targetname, targetid);

		new
			weaponname[MAX_NAME];

		for (new slot = 0; slot < PLAYER_WEAPON_SLOTS; slot++) {
			GetPlayerWeaponData(playerid, slot, weaponid, amount);
			GetPlayerWeaponName(playerid, weaponid, weaponname, sizeof(weaponname));

			if (weaponid == 0) {
				continue;
			}

			Lang_SendText(playerid, "ADMIN_COMMAND_WEAPON_GET_ITEM", slot, weaponid, weaponname, amount);
		}
	} else if (strcmp(subcmd, "give", true) == 0) {
		if (weaponid == -1) {
			Lang_SendText(playerid, "ADMIN_COMMAND_WEAPON_TARGET_ERROR");
			return 1;
		}

		if (amount == 0) {
			amount = 100;
		}

		if (targetid == -1) {
			foreach (new id : Player) {
				GivePlayerWeapon(id, weaponid, amount);
			}

			Lang_SendTextToAll("ADMIN_COMMAND_WEAPON_GIVE_ALL", playername, playerid, weaponid, amount);
		} else {
			GivePlayerWeapon(targetid, weaponid, amount);

			Lang_SendText(targetid, "ADMIN_COMMAND_WEAPON_GIVE_PLAYER", playername, playerid, weaponid, amount);
			Lang_SendText(playerid, "ADMIN_COMMAND_WEAPON_GIVE_SELF", targetname, targetid, weaponid, amount);
		}
	} else {
		Lang_SendText(playerid, "ADMIN_COMMAND_WEAPON_HELP");
	}

	return 1;
}
