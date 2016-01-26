/*

	About: freeze admin command
	Author: ziggi

*/

#if defined _admin_cmd_freeze_included
	#endinput
#endif

#define _admin_cmd_freeze_included
#pragma library admin_cmd_freeze

COMMAND:freeze(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
		return 0;
	}

	new
		subparams[32],
		time,
		reason[MAX_FREEZE_REASON_LENGTH];

	if (sscanf(params, "s[32]k<ftime>S()[" #MAX_FREEZE_REASON_LENGTH "]", subparams, time, reason)) {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_FREEZE_HELP));
		return 1;
	}

	new
		targetid = INVALID_PLAYER_ID;

	if (strcmp(subparams, "all", true) == 0) {
		targetid = -1;
	} else if (sscanf(subparams, "u", targetid) || targetid == INVALID_PLAYER_ID) {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_FREEZE_TARGET_ERROR));
		return 1;
	}

	new
		is_with_reason,
		timeword[MAX_LANG_VALUE_STRING],
		string[MAX_LANG_VALUE_STRING],
		targetname[MAX_PLAYER_NAME + 1],
		playername[MAX_PLAYER_NAME + 1];

	is_with_reason = strlen(reason) != 0;
	Declension_GetWord(timeword, sizeof(timeword), time, _(DECLENSION_SECOND_4), _(DECLENSION_SECOND_2), _(DECLENSION_SECOND_3));
	GetPlayerName(playerid, playername, sizeof(playername));

	if (targetid == -1) {
		format(string, sizeof(string), _(ADMIN_COMMAND_FREEZE_ALL), playername, playerid, time, timeword);
		if (is_with_reason) {
			format(string, sizeof(string), _(ADMIN_COMMAND_FREEZE_REASON), string, reason);
		}
		SendClientMessageToAll(-1, string);

		foreach (targetid : Player) {
			FreezePlayer(targetid, time);
		}
	} else {
		GetPlayerName(targetid, targetname, sizeof(targetname));

		format(string, sizeof(string), _(ADMIN_COMMAND_FREEZE_PLAYER), playername, playerid, targetname, targetid, time, timeword);
		if (is_with_reason) {
			format(string, sizeof(string), _(ADMIN_COMMAND_FREEZE_REASON), string, reason);
		}
		SendClientMessageToAll(-1, string);

		format(string, sizeof(string), _(ADMIN_COMMAND_FREEZE_PLAYER_SELF), targetname, targetid, time, timeword);
		if (is_with_reason) {
			format(string, sizeof(string), _(ADMIN_COMMAND_FREEZE_REASON), string, reason);
		}
		SendClientMessage(playerid, -1, string);

		FreezePlayer(targetid, time);
	}

	return 1;
}
