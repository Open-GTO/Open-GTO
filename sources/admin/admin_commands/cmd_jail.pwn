/*

	About: jail admin command
	Author: ziggi

*/

#if defined _admin_cmd_jail_included
	#endinput
#endif

#define _admin_cmd_jail_included

COMMAND:jail(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
		return 0;
	}

	new
		subparams[32],
		time,
		reason[MAX_JAIL_REASON_LENGTH];

	if (sscanf(params, "s[32]k<ftime>S()[" #MAX_JAIL_REASON_LENGTH "]", subparams, time, reason)) {
		Lang_SendText(playerid, "ADMIN_COMMAND_JAIL_HELP");
		return 1;
	}

	new
		targetid = INVALID_PLAYER_ID;

	if (strcmp(subparams, "all", true) == 0) {
		targetid = -1;
	} else if (sscanf(subparams, "u", targetid) || targetid == INVALID_PLAYER_ID) {
		Lang_SendText(playerid, "ADMIN_COMMAND_JAIL_TARGET_ERROR");
		return 1;
	}

	new
		is_with_reason,
		timeword[MAX_LANG_VALUE_STRING],
		targetname[MAX_PLAYER_NAME + 1],
		playername[MAX_PLAYER_NAME + 1];

	is_with_reason = strlen(reason) != 0;
	Declension_GetSeconds2(playerid, time, timeword);
	GetPlayerName(playerid, playername, sizeof(playername));

	if (targetid == -1) {
		if (is_with_reason) {
			Lang_SendTextToAll("ADMIN_COMMAND_JAIL_ALL_REASON", playername, playerid, time, timeword, string, reason);
		} else {
			Lang_SendTextToAll("ADMIN_COMMAND_JAIL_ALL", playername, playerid, time, timeword);
		}

		foreach (targetid : Player) {
			JailPlayer(targetid, time);
		}
	} else {
		GetPlayerName(targetid, targetname, sizeof(targetname));

		if (is_with_reason) {
			Lang_SendTextToAll("ADMIN_COMMAND_JAIL_PLAYER_REASON", playername, playerid, targetname, targetid, time, timeword, reason);
			Lang_SendText(playerid, "ADMIN_COMMAND_JAIL_PLAYER_SELF_REASON", targetname, targetid, time, timeword, reason);
		} else {
			Lang_SendTextToAll("ADMIN_COMMAND_JAIL_PLAYER", playername, playerid, targetname, targetid, time, timeword);
			Lang_SendText(playerid, "ADMIN_COMMAND_JAIL_PLAYER_SELF", targetname, targetid, time, timeword);
		}

		JailPlayer(targetid, time);
	}

	return 1;
}
