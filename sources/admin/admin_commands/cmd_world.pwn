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
		Lang_SendText(playerid, "ADMIN_COMMAND_WORLD_HELP");
		return 1;
	}

	new
		targetid = INVALID_PLAYER_ID;

	if (strcmp(subparams, "all", true) == 0) {
		targetid = -1;
	} else if (sscanf(subparams, "u", targetid) || targetid == INVALID_PLAYER_ID) {
		Lang_SendText(playerid, "ADMIN_COMMAND_WORLD_TARGET_ERROR");
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
		if (targetid == -1) {
			foreach (new id : Player) {
				SetPlayerVirtualWorld(id, world);
			}

			Lang_SendTextToAll("ADMIN_COMMAND_WORLD_SET_ALL", playername, playerid, world);
		} else {
			SetPlayerVirtualWorld(targetid, world);

			Lang_SendText(targetid, "ADMIN_COMMAND_WORLD_SET_PLAYER", playername, playerid, world);
			Lang_SendText(playerid, "ADMIN_COMMAND_WORLD_SET_SELF", targetname, targetid, world);
		}
	} else if (strcmp(subcmd, "get", true) == 0) {
		world = GetPlayerVirtualWorld(targetid);

		Lang_SendText(playerid, "ADMIN_COMMAND_WORLD_GET", targetname, targetid, world);
	} else {
		Lang_SendText(playerid, "ADMIN_COMMAND_WORLD_HELP");
	}

	return 1;
}
