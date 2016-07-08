/*

	About: unfreeze admin command
	Author: ziggi

*/

#if defined _admin_cmd_unfreeze_included
	#endinput
#endif

#define _admin_cmd_unfreeze_included

COMMAND:unfreeze(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
		return 0;
	}

	new
		subparams[32],
		reason[MAX_FREEZE_REASON_LENGTH];

	if (sscanf(params, "s[32]S()[" #MAX_FREEZE_REASON_LENGTH "]", subparams, reason)) {
		SendClientMessage(playerid, -1, _(playerid, ADMIN_COMMAND_UNFREEZE_HELP));
		return 1;
	}

	new
		targetid = INVALID_PLAYER_ID;

	if (strcmp(subparams, "all", true) == 0) {
		targetid = -1;
	} else if (sscanf(subparams, "u", targetid) || targetid == INVALID_PLAYER_ID) {
		SendClientMessage(playerid, -1, _(playerid, ADMIN_COMMAND_UNFREEZE_TARGET_ERROR));
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
		format(string, sizeof(string), _(playerid, ADMIN_COMMAND_UNFREEZE_ALL), playername, playerid, reason);
		if (is_with_reason) {
			format(string, sizeof(string), _(playerid, ADMIN_COMMAND_UNFREEZE_REASON), string, reason);
		}
		SendClientMessageToAll(-1, string);

		foreach (targetid : Player) {
			UnFreezePlayer(targetid);
		}
	} else {
		GetPlayerName(targetid, targetname, sizeof(targetname));

		format(string, sizeof(string), _(playerid, ADMIN_COMMAND_UNFREEZE_PLAYER), playername, playerid, targetname, targetid);
		if (is_with_reason) {
			format(string, sizeof(string), _(playerid, ADMIN_COMMAND_UNFREEZE_REASON), string, reason);
		}
		SendClientMessageToAll(-1, string);

		format(string, sizeof(string), _(playerid, ADMIN_COMMAND_UNFREEZE_PLAYER_SELF), targetname, targetid);
		if (is_with_reason) {
			format(string, sizeof(string), _(playerid, ADMIN_COMMAND_UNFREEZE_REASON), string, reason);
		}
		SendClientMessage(playerid, -1, string);

		UnFreezePlayer(targetid);
	}

	return 1;
}
