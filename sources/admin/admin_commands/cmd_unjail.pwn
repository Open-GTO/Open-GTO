/*

	About: unjail admin command
	Author: ziggi

*/

#if defined _admin_cmd_unjail_included
	#endinput
#endif

#define _admin_cmd_unjail_included

COMMAND:unjail(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
		return 0;
	}

	new
		subparams[32],
		reason[MAX_JAIL_REASON_LENGTH];

	if (sscanf(params, "s[32]S()[" #MAX_JAIL_REASON_LENGTH "]", subparams, reason)) {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_UNJAIL_HELP));
		return 1;
	}

	new
		targetid = INVALID_PLAYER_ID;

	if (strcmp(subparams, "all", true) == 0) {
		targetid = -1;
	} else if (sscanf(subparams, "u", targetid) || targetid == INVALID_PLAYER_ID) {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_UNJAIL_TARGET_ERROR));
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
		format(string, sizeof(string), _(ADMIN_COMMAND_UNJAIL_ALL), playername, playerid, reason);
		if (is_with_reason) {
			format(string, sizeof(string), _(ADMIN_COMMAND_UNJAIL_REASON), string, reason);
		}
		SendClientMessageToAll(-1, string);

		foreach (targetid : Player) {
			UnJailPlayer(targetid);
		}
	} else {
		GetPlayerName(targetid, targetname, sizeof(targetname));

		format(string, sizeof(string), _(ADMIN_COMMAND_UNJAIL_PLAYER), playername, playerid, targetname, targetid);
		if (is_with_reason) {
			format(string, sizeof(string), _(ADMIN_COMMAND_UNJAIL_REASON), string, reason);
		}
		SendClientMessageToAll(-1, string);

		format(string, sizeof(string), _(ADMIN_COMMAND_UNJAIL_PLAYER_SELF), targetname, targetid);
		if (is_with_reason) {
			format(string, sizeof(string), _(ADMIN_COMMAND_UNJAIL_REASON), string, reason);
		}
		SendClientMessage(playerid, -1, string);

		UnJailPlayer(targetid);
	}

	return 1;
}
