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
		time = -1,
		reason[MAX_JAIL_REASON_LENGTH];

	if (sscanf(params, "s[32]k<ftime>S()[" #MAX_JAIL_REASON_LENGTH "]", subparams, time, reason)) {
		Lang_SendText(playerid, "ADMIN_COMMAND_JAIL_HELP");
		return 1;
	}

	if (time == -1) {
		Lang_SendText(playerid, "ADMIN_COMMAND_TIME_ERROR");
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
		bool:is_with_reason,
		timestring[MAX_LANG_VALUE_STRING],
		targetname[MAX_PLAYER_NAME + 1],
		playername[MAX_PLAYER_NAME + 1];

	is_with_reason = strlen(reason) != 0;
	GetPlayerName(playerid, playername, sizeof(playername));

	if (targetid == -1) {
		if (is_with_reason) {
			foreach (new i : Player) {
				GetTimeStringFromSeconds(i, time, timestring);
				Lang_SendText(i, "ADMIN_COMMAND_JAIL_ALL_REASON", playername, playerid, timestring, reason);
			}
		} else {
			foreach (new i : Player) {
				GetTimeStringFromSeconds(i, time, timestring);
				Lang_SendText(i, "ADMIN_COMMAND_JAIL_ALL", playername, playerid, timestring);
			}
		}

		foreach (targetid : Player) {
			JailPlayer(targetid, time);
		}
	} else {
		GetPlayerName(targetid, targetname, sizeof(targetname));

		if (is_with_reason) {
			foreach (new i : Player) {
				if (i == playerid) {
					continue;
				}
				GetTimeStringFromSeconds(i, time, timestring);
				Lang_SendText(i, "ADMIN_COMMAND_JAIL_PLAYER_REASON", playername, playerid, targetname, targetid, timestring, reason);
			}

			GetTimeStringFromSeconds(playerid, time, timestring);
			Lang_SendText(playerid, "ADMIN_COMMAND_JAIL_PLAYER_SELF_REASON", targetname, targetid, timestring, reason);
		} else {
			foreach (new i : Player) {
				if (i == playerid) {
					continue;
				}
				GetTimeStringFromSeconds(i, time, timestring);
				Lang_SendText(i, "ADMIN_COMMAND_JAIL_PLAYER", playername, playerid, targetname, targetid, timestring);
			}

			GetTimeStringFromSeconds(playerid, time, timestring);
			Lang_SendText(playerid, "ADMIN_COMMAND_JAIL_PLAYER_SELF", targetname, targetid, timestring);
		}

		JailPlayer(targetid, time);
	}

	return 1;
}
