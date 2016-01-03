/*

	About: armour admin command
	Author: ziggi

*/

#if defined _admin_cmd_armour_included
	#endinput
#endif

#define _admin_cmd_armour_included
#pragma library admin_cmd_armour

COMMAND:armour(playerid, params[])
{
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}

	new
		subcmd[5],
		subparams[32],
		Float:amount;

	if (sscanf(params, "s[5]s[32]F(100.0)", subcmd, subparams, amount)) {
		SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_ARMOUR_HELP));
		return 1;
	}

	new
		targetid = INVALID_PLAYER_ID;

	if (strcmp(subparams, "all", true) == 0) {
		targetid = -1;
	} else if (sscanf(subparams, "u", targetid)) {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_ARMOUR_TARGET_ERROR));
		return 1;
	}

	new
		string[MAX_LANG_VALUE_STRING],
		playername[MAX_PLAYER_NAME + 1],
		targetname[MAX_PLAYER_NAME + 1];

	GetPlayerName(playerid, playername, sizeof(playername));

	if (targetid != -1) {
		GetPlayerName(targetid, targetname, sizeof(targetname));
	}

	if (strcmp(subcmd, "set", true) == 0) {
		if (targetid == -1) {
			foreach (new id : Player) {
				SetPlayerArmour(id, amount);
			}

			format(string, sizeof(string), _(ADMIN_COMMAND_ARMOUR_SET_ALL), playername, playerid, amount);
			SendClientMessageToAll(-1, string);
		} else {
			SetPlayerArmour(targetid, amount);

			format(string, sizeof(string), _(ADMIN_COMMAND_ARMOUR_SET_PLAYER), playername, playerid, targetname, targetid, amount);
			SendMessageToNearPlayerPlayers(string, 40.0, playerid);

			format(string, sizeof(string), _(ADMIN_COMMAND_ARMOUR_SET_SELF), targetname, targetid, amount);
			SendClientMessage(playerid, -1, string);
		}
	} else if (strcmp(subcmd, "get", true) == 0) {
		if (!IsPlayerConnected(targetid)) {
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_ARMOUR_TARGET_ERROR));
			return 1;
		}

		GetPlayerArmour(targetid, amount);

		format(string, sizeof(string), _(ADMIN_COMMAND_ARMOUR_GET), targetname, targetid, amount);
		SendClientMessage(playerid, -1, string);
	} else if (strcmp(subcmd, "give", true) == 0) {
		new
			Float:current_armour;

		if (targetid == -1) {
			foreach (new id : Player) {
				GetPlayerArmour(id, current_armour);
				SetPlayerArmour(id, current_armour + amount);
			}

			format(string, sizeof(string), _(ADMIN_COMMAND_ARMOUR_GIVE_ALL), playername, playerid, amount);
			SendClientMessageToAll(-1, string);
		} else {
			GetPlayerArmour(targetid, current_armour);
			SetPlayerArmour(targetid, current_armour + amount);

			format(string, sizeof(string), _(ADMIN_COMMAND_ARMOUR_GIVE_PLAYER), playername, playerid, targetname, targetid, amount);
			SendMessageToNearPlayerPlayers(string, 40.0, playerid);

			format(string, sizeof(string), _(ADMIN_COMMAND_ARMOUR_GIVE_SELF), targetname, targetid, amount);
			SendClientMessage(playerid, -1, string);
		}
	}

	return 1;
}
