/*

	About: level admin command
	Author: ziggi

*/

#if defined _admin_cmd_level_included
	#endinput
#endif

#define _admin_cmd_level_included
#pragma library admin_cmd_level

COMMAND:level(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
		return 0;
	}

	new
		subcmd[5],
		subparams[32],
		amount;

	if (sscanf(params, "s[5]s[32]I(1)", subcmd, subparams, amount)) {
		SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_LEVEL_HELP));
		return 1;
	}

	new
		targetid = INVALID_PLAYER_ID;

	if (strcmp(subparams, "all", true) == 0) {
		targetid = -1;
	} else if (sscanf(subparams, "u", targetid) || targetid == INVALID_PLAYER_ID) {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_LEVEL_TARGET_ERROR));
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
				SetPlayerLevel(id, amount);
			}

			format(string, sizeof(string), _(ADMIN_COMMAND_LEVEL_SET_ALL), playername, playerid, amount);
			SendClientMessageToAll(-1, string);
		} else {
			SetPlayerLevel(targetid, amount);

			format(string, sizeof(string), _(ADMIN_COMMAND_LEVEL_SET_PLAYER), playername, playerid, amount);
			SendClientMessage(targetid, -1, string);

			format(string, sizeof(string), _(ADMIN_COMMAND_LEVEL_SET_SELF), targetname, targetid, amount);
			SendClientMessage(playerid, -1, string);
		}
	} else if (strcmp(subcmd, "get", true) == 0) {
		if (!IsPlayerConnected(targetid)) {
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_LEVEL_TARGET_ERROR));
			return 1;
		}

		amount = GetPlayerLevel(targetid);

		format(string, sizeof(string), _(ADMIN_COMMAND_LEVEL_GET), targetname, targetid, amount);
		SendClientMessage(playerid, -1, string);
	} else if (strcmp(subcmd, "give", true) == 0) {
		if (targetid == -1) {
			foreach (new id : Player) {
				GivePlayerLevel(id, amount);
			}

			format(string, sizeof(string), _(ADMIN_COMMAND_LEVEL_GIVE_ALL), playername, playerid, amount);
			SendClientMessageToAll(-1, string);
		} else {
			GivePlayerLevel(targetid, amount);

			format(string, sizeof(string), _(ADMIN_COMMAND_LEVEL_GIVE_PLAYER), playername, playerid, amount);
			SendClientMessage(targetid, -1, string);

			format(string, sizeof(string), _(ADMIN_COMMAND_LEVEL_GIVE_SELF), targetname, targetid, amount);
			SendClientMessage(playerid, -1, string);
		}
	}

	return 1;
}
