/*

	About: weapon admin command
	Author: ziggi

*/

#if defined _admin_cmd_weapon_included
	#endinput
#endif

#define _admin_cmd_weapon_included
#pragma library admin_cmd_weapon

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
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	new
		subcmd[5],
		subparams[32],
		weaponid,
		amount;

	if (sscanf(params, "s[5]s[32]k<weapon>I(0)", subcmd, subparams, weaponid, amount)) {
		SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_WEAPON_HELP));
		return 1;
	}

	new
		targetid = INVALID_PLAYER_ID;

	if (strcmp(subparams, "all", true) == 0) {
		targetid = -1;
	} else if (sscanf(subparams, "u", targetid)) {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_WEAPON_TARGET_ERROR));
		return 1;
	}

	new
		string[MAX_LANG_VALUE_STRING],
		targetname[MAX_PLAYER_NAME + 1],
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(playerid, playername, sizeof(playername));

	if (targetid != -1) {
		GetPlayerName(targetid, targetname, sizeof(targetname));
	}

	if (strcmp(subcmd, "set", true) == 0) {
		if (targetid == -1) {
			foreach (new id : Player) {
				SetPlayerWeapon(id, weaponid, amount);
			}

			format(string, sizeof(string), _(ADMIN_COMMAND_WEAPON_SET_ALL), playername, playerid, weaponid, amount);
			SendClientMessageToAll(-1, string);
		} else {
			SetPlayerWeapon(targetid, weaponid, amount);

			format(string, sizeof(string), _(ADMIN_COMMAND_WEAPON_SET_PLAYER), playername, playerid, weaponid, amount);
			SendClientMessage(targetid, -1, string);

			format(string, sizeof(string), _(ADMIN_COMMAND_WEAPON_SET_SELF), targetname, targetid, weaponid, amount);
			SendClientMessage(playerid, -1, string);
		}
	} else if (strcmp(subcmd, "get", true) == 0) {
		if (!IsPlayerConnected(targetid)) {
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_WEAPON_TARGET_ERROR));
			return 1;
		}

		format(string, sizeof(string), _(ADMIN_COMMAND_WEAPON_GET), targetname, targetid);
		SendClientMessage(playerid, -1, string);

		for (new slot = 0; slot < PLAYER_WEAPON_SLOTS; slot++) {
			GetPlayerWeaponData(playerid, slot, weaponid, amount);

			if (weaponid == 0) {
				continue;
			}

			format(string, sizeof(string), _(ADMIN_COMMAND_WEAPON_GET_ITEM), slot, weaponid, amount);
			SendClientMessage(playerid, -1, string);
		}
	} else if (strcmp(subcmd, "give", true) == 0) {
		if (targetid == -1) {
			foreach (new id : Player) {
				GivePlayerWeapon(id, weaponid, amount);
			}

			format(string, sizeof(string), _(ADMIN_COMMAND_WEAPON_GIVE_ALL), playername, playerid, weaponid, amount);
			SendClientMessageToAll(-1, string);
		} else {
			GivePlayerWeapon(targetid, weaponid, amount);

			format(string, sizeof(string), _(ADMIN_COMMAND_WEAPON_GIVE_PLAYER), playername, playerid, weaponid, amount);
			SendClientMessage(targetid, -1, string);

			format(string, sizeof(string), _(ADMIN_COMMAND_WEAPON_GIVE_SELF), targetname, targetid, weaponid, amount);
			SendClientMessage(playerid, -1, string);
		}
	}

	return 1;
}
