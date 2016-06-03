/*

	About: world admin command
	Author: ziggi

*/

#if defined _admin_cmd_world_included
	#endinput
#endif

#define _admin_cmd_world_included

COMMAND:world(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
		return 0;
	}

	new
		subcmd[5],
		subparams[32],
		world;

	if (sscanf(params, "s[5]s[32]I(0)", subcmd, subparams, world)) {
		SendClientMessage(playerid, COLOR_RED, _(ADMIN_COMMAND_WORLD_HELP));
		return 1;
	}

	new
		targetid = INVALID_PLAYER_ID;

	if (strcmp(subparams, "all", true) == 0) {
		targetid = -1;
	} else if (sscanf(subparams, "u", targetid) || targetid == INVALID_PLAYER_ID) {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_WORLD_TARGET_ERROR));
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
				SetPlayerVirtualWorld(id, world);
			}

			format(string, sizeof(string), _(ADMIN_COMMAND_WORLD_SET_ALL), playername, playerid, world);
			SendClientMessageToAll(-1, string);
		} else {
			SetPlayerVirtualWorld(targetid, world);

			format(string, sizeof(string), _(ADMIN_COMMAND_WORLD_SET_PLAYER), playername, playerid, world);
			SendClientMessage(targetid, -1, string);

			format(string, sizeof(string), _(ADMIN_COMMAND_WORLD_SET_SELF), targetname, targetid, world);
			SendClientMessage(playerid, -1, string);
		}
	} else if (strcmp(subcmd, "get", true) == 0) {
		if (!IsPlayerConnected(targetid)) {
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_WORLD_TARGET_ERROR));
			return 1;
		}

		world = GetPlayerVirtualWorld(targetid);

		format(string, sizeof(string), _(ADMIN_COMMAND_WORLD_GET), targetname, targetid, world);
		SendClientMessage(playerid, -1, string);
	} else {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_WORLD_HELP));
	}

	return 1;
}
