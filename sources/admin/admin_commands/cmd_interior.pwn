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
		Lang_SendText(playerid, "ADMIN_COMMAND_INTERIOR_HELP");
		return 1;
	}

	new
		targetid = INVALID_PLAYER_ID;

	if (strcmp(subparams, "all", true) == 0) {
		targetid = -1;
	} else if (sscanf(subparams, "u", targetid) || targetid == INVALID_PLAYER_ID) {
		Lang_SendText(playerid, "ADMIN_COMMAND_INTERIOR_TARGET_ERROR");
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
				SetPlayerInterior(id, interior);
			}

			Lang_SendTextToAll("ADMIN_COMMAND_INTERIOR_SET_ALL", playername, playerid, interior);
		} else {
			SetPlayerInterior(targetid, interior);

			Lang_SendText(targetid, "ADMIN_COMMAND_INTERIOR_SET_PLAYER", playername, playerid, interior);
			Lang_SendText(playerid, "ADMIN_COMMAND_INTERIOR_SET_SELF", targetname, targetid, interior);
		}
	} else if (strcmp(subcmd, "get", true) == 0) {
		interior = GetPlayerInterior(targetid);

		Lang_SendText(playerid, "ADMIN_COMMAND_INTERIOR_GET", targetname, targetid, interior);
	} else {
		Lang_SendText(playerid, "ADMIN_COMMAND_INTERIOR_HELP");
	}

	return 1;
}
