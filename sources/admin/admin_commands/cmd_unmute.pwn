/*

	About: unmute admin command
	Author: ziggi

*/

#if defined _admin_cmd_unmute_included
	#endinput
#endif

#define _admin_cmd_unmute_included

COMMAND:unmute(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
		return 0;
	}

	new
		subparams[32],
		reason[MAX_MUTE_REASON_LENGTH];

	if (sscanf(params, "s[32]S()[" #MAX_MUTE_REASON_LENGTH "]", subparams, reason)) {
		Lang_SendText(playerid, "ADMIN_COMMAND_UNMUTE_HELP");
		return 1;
	}

	new
		targetid = INVALID_PLAYER_ID;

	if (strcmp(subparams, "all", true) == 0) {
		targetid = -1;
	} else if (sscanf(subparams, "u", targetid) || targetid == INVALID_PLAYER_ID) {
		Lang_SendText(playerid, "ADMIN_COMMAND_UNMUTE_TARGET_ERROR");
		return 1;
	}

	new
		is_with_reason,
		targetname[MAX_PLAYER_NAME + 1],
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(playerid, playername, sizeof(playername));
	is_with_reason = strlen(reason) != 0;

	if (targetid == -1) {
		if (is_with_reason) {
			Lang_SendTextToAll("ADMIN_COMMAND_UNMUTE_ALL_REASON", playername, playerid, reason);
		} else {
			Lang_SendTextToAll("ADMIN_COMMAND_UNMUTE_ALL", playername, playerid);
		}

		foreach (targetid : Player) {
			UnMutePlayer(targetid);
		}
	} else {
		if (!IsPlayerMuted(targetid)) {
			Lang_SendText(playerid, "ADMIN_COMMAND_UNMUTE_PLAYER_ERROR");
			return 1;
		}

		GetPlayerName(targetid, targetname, sizeof(targetname));

		if (is_with_reason) {
			Lang_SendTextToAll("ADMIN_COMMAND_UNMUTE_PLAYER_REASON", playername, playerid, targetname, targetid, reason);
			Lang_SendText(playerid, "ADMIN_COMMAND_UNMUTE_PLAYER_SELF_REASON", targetname, targetid, reason);
		} else {
			Lang_SendTextToAll("ADMIN_COMMAND_UNMUTE_PLAYER", playername, playerid, targetname, targetid);
			Lang_SendText(playerid, "ADMIN_COMMAND_UNMUTE_PLAYER_SELF", targetname, targetid);
		}

		UnMutePlayer(targetid);
	}

	return 1;
}
