/*

	About: level admin command
	Author: ziggi

*/

#if defined _admin_cmd_level_included
	#endinput
#endif

#define _admin_cmd_level_included

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
		Lang_SendText(playerid, "ADMIN_COMMAND_LEVEL_HELP");
		return 1;
	}

	new
		targetid = INVALID_PLAYER_ID;

	if (strcmp(subparams, "all", true) == 0) {
		targetid = -1;
	} else if (sscanf(subparams, "u", targetid) || targetid == INVALID_PLAYER_ID) {
		Lang_SendText(playerid, "ADMIN_COMMAND_LEVEL_TARGET_ERROR");
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
		if (!IsValidPlayerLevel(amount)) {
			Lang_SendTextToAll("ADMIN_COMMAND_LEVEL_LEVEL_ERROR", GetMinPlayerLevel(), GetMaxPlayerLevel());
			return 1;
		}

		if (targetid == -1) {
			foreach (new id : Player) {
				SetPlayerLevel(id, amount);
			}

			Lang_SendTextToAll("ADMIN_COMMAND_LEVEL_SET_ALL", playername, playerid, amount);
		} else {
			SetPlayerLevel(targetid, amount);

			Lang_SendText(targetid, "ADMIN_COMMAND_LEVEL_SET_PLAYER", playername, playerid, amount);

			Lang_SendText(playerid, "ADMIN_COMMAND_LEVEL_SET_SELF", targetname, targetid, amount);
		}
	} else if (strcmp(subcmd, "get", true) == 0) {
		amount = GetPlayerLevel(targetid);

		Lang_SendText(playerid, "ADMIN_COMMAND_LEVEL_GET", targetname, targetid, amount);
	} else if (strcmp(subcmd, "give", true) == 0) {
		if (targetid == -1) {
			foreach (new id : Player) {
				GivePlayerLevel(id, amount);
			}

			Lang_SendTextToAll("ADMIN_COMMAND_LEVEL_GIVE_ALL", playername, playerid, amount);
		} else {
			GivePlayerLevel(targetid, amount);

			Lang_SendText(targetid, "ADMIN_COMMAND_LEVEL_GIVE_PLAYER", playername, playerid, amount);
			Lang_SendText(playerid, "ADMIN_COMMAND_LEVEL_GIVE_SELF", targetname, targetid, amount);
		}
	} else {
		Lang_SendText(playerid, "ADMIN_COMMAND_LEVEL_HELP");
	}

	return 1;
}
