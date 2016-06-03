/*

	About: interior admin command
	Author: ziggi

*/

#if defined _admin_cmd_interior_included
	#endinput
#endif

#define _admin_cmd_interior_included

COMMAND:int(playerid, params[])
{
	return cmd_interior(playerid, params);
}

COMMAND:interior(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
		return 0;
	}

	new
		subcmd[5],
		subparams[32],
		interior;

	if (sscanf(params, "s[5]s[32]I(0)", subcmd, subparams, interior)) {
		SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_INTERIOR_HELP));
		return 1;
	}

	new
		targetid = INVALID_PLAYER_ID;

	if (strcmp(subparams, "all", true) == 0) {
		targetid = -1;
	} else if (sscanf(subparams, "u", targetid) || targetid == INVALID_PLAYER_ID) {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_INTERIOR_TARGET_ERROR));
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
				SetPlayerInterior(id, interior);
			}

			format(string, sizeof(string), _(ADMIN_COMMAND_INTERIOR_SET_ALL), playername, playerid, interior);
			SendClientMessageToAll(-1, string);
		} else {
			SetPlayerInterior(targetid, interior);

			format(string, sizeof(string), _(ADMIN_COMMAND_INTERIOR_SET_PLAYER), playername, playerid, interior);
			SendClientMessage(targetid, -1, string);

			format(string, sizeof(string), _(ADMIN_COMMAND_INTERIOR_SET_SELF), targetname, targetid, interior);
			SendClientMessage(playerid, -1, string);
		}
	} else if (strcmp(subcmd, "get", true) == 0) {
		if (!IsPlayerConnected(targetid)) {
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_INTERIOR_TARGET_ERROR));
			return 1;
		}

		interior = GetPlayerInterior(targetid);

		format(string, sizeof(string), _(ADMIN_COMMAND_INTERIOR_GET), targetname, targetid, interior);
		SendClientMessage(playerid, -1, string);
	} else {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_INTERIOR_HELP));
	}

	return 1;
}
