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
		string[MAX_LANG_VALUE_STRING],
		targetname[MAX_PLAYER_NAME + 1],
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(playerid, playername, sizeof(playername));
	is_with_reason = strlen(reason) != 0;

	if (targetid == -1) {
		format(string, sizeof(string), _(playerid, ADMIN_COMMAND_UNMUTE_ALL), playername, playerid, reason);
		if (is_with_reason) {
			format(string, sizeof(string), _(playerid, ADMIN_COMMAND_UNMUTE_REASON), string, reason);
		}
		SendClientMessageToAll(-1, string);

		foreach (targetid : Player) {
			UnMutePlayer(targetid);
		}
	} else {
		GetPlayerName(targetid, targetname, sizeof(targetname));

		format(string, sizeof(string), _(playerid, ADMIN_COMMAND_UNMUTE_PLAYER), playername, playerid, targetname, targetid);
		if (is_with_reason) {
			format(string, sizeof(string), _(playerid, ADMIN_COMMAND_UNMUTE_REASON), string, reason);
		}
		SendClientMessageToAll(-1, string);

		format(string, sizeof(string), _(playerid, ADMIN_COMMAND_UNMUTE_PLAYER_SELF), targetname, targetid);
		if (is_with_reason) {
			format(string, sizeof(string), _(playerid, ADMIN_COMMAND_UNMUTE_REASON), string, reason);
		}
		SendClientMessage(playerid, -1, string);

		UnMutePlayer(targetid);
	}

	return 1;
}
