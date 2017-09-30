/*

	About: xp admin command
	Author: ziggi

*/

#if defined _admin_cmd_xp_included
	#endinput
#endif

#define _admin_cmd_xp_included

COMMAND:xp(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
		return 0;
	}

	new
		subcmd[5],
		subparams[32],
		amount;

	if (sscanf(params, "s[5]s[32]I(1)", subcmd, subparams, amount)) {
		Lang_SendText(playerid, "ADMIN_COMMAND_XP_HELP");
		return 1;
	}

	new
		targetid = INVALID_PLAYER_ID;

	if (strcmp(subparams, "all", true) == 0) {
		targetid = -1;
	} else if (sscanf(subparams, "u", targetid) || targetid == INVALID_PLAYER_ID) {
		Lang_SendText(playerid, "ADMIN_COMMAND_XP_TARGET_ERROR");
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
				SetPlayerXP(id, amount);
			}

			Lang_SendTextToAll("ADMIN_COMMAND_XP_SET_ALL", playername, playerid, amount);
		} else {
			SetPlayerXP(targetid, amount);

			Lang_SendText(targetid, "ADMIN_COMMAND_XP_SET_PLAYER", playername, playerid, amount);
			Lang_SendText(playerid, "ADMIN_COMMAND_XP_SET_SELF", targetname, targetid, amount);
		}
	} else if (strcmp(subcmd, "get", true) == 0) {
		if (!IsPlayerConnected(targetid)) {
			Lang_SendText(playerid, "ADMIN_COMMAND_XP_TARGET_ERROR");
			return 1;
		}

		amount = GetPlayerXP(targetid);

		Lang_SendText(playerid, "ADMIN_COMMAND_XP_GET", targetname, targetid, amount);
	} else if (strcmp(subcmd, "give", true) == 0) {
		if (targetid == -1) {
			foreach (new id : Player) {
				GivePlayerXP(id, amount);
			}

			Lang_SendTextToAll("ADMIN_COMMAND_XP_GIVE_ALL", playername, playerid, amount);
		} else {
			GivePlayerXP(targetid, amount);

			Lang_SendText(targetid, "ADMIN_COMMAND_XP_GIVE_PLAYER", playername, playerid, amount);
			Lang_SendText(playerid, "ADMIN_COMMAND_XP_GIVE_SELF", targetname, targetid, amount);
		}
	} else {
		Lang_SendText(playerid, "ADMIN_COMMAND_XP_HELP");
	}

	return 1;
}
