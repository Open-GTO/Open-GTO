/*

	About: warn admin command
	Author: ziggi

*/

#if defined _admin_cmd_warn_included
	#endinput
#endif

#define _admin_cmd_warn_included

COMMAND:warn(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
		return 0;
	}

	new
		subcmd[5],
		subparams[32],
		amount,
		reason[MAX_WARN_REASON_LENGTH];

	if (sscanf(params, "s[5]s[32]I(1)S()[" #MAX_WARN_REASON_LENGTH "]", subcmd, subparams, amount, reason)) {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_WARN_HELP));
		return 1;
	}

	new
		targetid = INVALID_PLAYER_ID;

	if (strcmp(subparams, "all", true) == 0) {
		targetid = -1;
	} else if (sscanf(subparams, "u", targetid) || targetid == INVALID_PLAYER_ID) {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_WARN_TARGET_ERROR));
		return 1;
	}

	new
		string[MAX_LANG_VALUE_STRING],
		targetname[MAX_PLAYER_NAME + 1],
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(playerid, playername, sizeof(playername));

	if (targetid != -1) {
		GetPlayerName(targetid, targetname, sizeof(targetname));
	}

	if (strlen(reason) == 0) {
		__(ADMIN_COMMAND_WARN_NOREASON, reason);
	}

	if (strcmp(subcmd, "set", true) == 0) {
		if (targetid == -1) {
			foreach (new id : Player) {
				SetPlayerWarnsCount(id, amount);
			}

			format(string, sizeof(string), _(ADMIN_COMMAND_WARN_SET_ALL), playername, playerid, amount, reason);
			SendClientMessageToAll(-1, string);
		} else {
			SetPlayerWarnsCount(targetid, amount);

			format(string, sizeof(string), _(ADMIN_COMMAND_WARN_SET_PLAYER), playername, playerid, amount, reason);
			SendClientMessage(targetid, -1, string);

			format(string, sizeof(string), _(ADMIN_COMMAND_WARN_SET_SELF), targetname, targetid, amount, reason);
			SendClientMessage(playerid, -1, string);
		}
	} else if (strcmp(subcmd, "get", true) == 0) {
		if (!IsPlayerConnected(targetid)) {
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_WARN_TARGET_ERROR));
			return 1;
		}

		amount = GetPlayerWarnsCount(targetid);

		format(string, sizeof(string), _(ADMIN_COMMAND_WARN_GET), targetname, targetid, amount);
		SendClientMessage(playerid, -1, string);
	} else if (strcmp(subcmd, "give", true) == 0) {
		new
			warnword[MAX_LANG_VALUE_STRING];

		Declension_GetWord(warnword, sizeof(warnword), amount, _(ADMIN_WARN_DECLENSION_1), _(ADMIN_WARN_DECLENSION_2), _(ADMIN_WARN_DECLENSION_2));

		if (targetid == -1) {
			foreach (new id : Player) {
				GivePlayerWarn(id, amount);
			}

			format(string, sizeof(string), _(ADMIN_COMMAND_WARN_GIVE_ALL), playername, playerid, amount, warnword, reason);
			SendClientMessageToAll(-1, string);
		} else {
			GivePlayerWarn(targetid, amount);

			format(string, sizeof(string), _(ADMIN_COMMAND_WARN_GIVE_PLAYER), playername, playerid, amount, warnword, reason);
			SendClientMessage(targetid, -1, string);

			format(string, sizeof(string), _(ADMIN_COMMAND_WARN_GIVE_SELF), targetname, targetid, amount, warnword, reason);
			SendClientMessage(playerid, -1, string);
		}
	} else {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_WARN_HELP));
	}

	return 1;
}
