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
		Lang_SendText(playerid, "ADMIN_COMMAND_WARN_HELP");
		return 1;
	}

	new
		targetid = INVALID_PLAYER_ID;

	if (strcmp(subparams, "all", true) == 0) {
		targetid = -1;
	} else if (sscanf(subparams, "u", targetid) || targetid == INVALID_PLAYER_ID) {
		Lang_SendText(playerid, "ADMIN_COMMAND_WARN_TARGET_ERROR");
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
		__(playerid, ADMIN_COMMAND_WARN_NOREASON, reason);
	}

	if (strcmp(subcmd, "set", true) == 0) {
		if (targetid == -1) {
			foreach (new id : Player) {
				SetPlayerWarnsCount(id, amount);
			}

			Lang_SendTextToAll("ADMIN_COMMAND_WARN_SET_ALL", playername, playerid, amount, reason);
		} else {
			SetPlayerWarnsCount(targetid, amount);

			Lang_SendText(targetid, "ADMIN_COMMAND_WARN_SET_PLAYER", playername, playerid, amount, reason);
			Lang_SendText(playerid, "ADMIN_COMMAND_WARN_SET_SELF", targetname, targetid, amount, reason);
		}
	} else if (strcmp(subcmd, "get", true) == 0) {
		if (!IsPlayerConnected(targetid)) {
			Lang_SendText(playerid, "ADMIN_COMMAND_WARN_TARGET_ERROR");
			return 1;
		}

		amount = GetPlayerWarnsCount(targetid);

		Lang_SendText(playerid, "ADMIN_COMMAND_WARN_GET", targetname, targetid, amount);
	} else if (strcmp(subcmd, "give", true) == 0) {
		new
			warnword[MAX_LANG_VALUE_STRING];

		Declension_GetWord(warnword, sizeof(warnword), amount, _(playerid, ADMIN_WARN_DECLENSION_1), _(playerid, ADMIN_WARN_DECLENSION_2), _(playerid, ADMIN_WARN_DECLENSION_2));

		if (targetid == -1) {
			foreach (new id : Player) {
				GivePlayerWarn(id, amount);
			}

			Lang_SendTextToAll("ADMIN_COMMAND_WARN_GIVE_ALL", playername, playerid, amount, warnword, reason);
		} else {
			GivePlayerWarn(targetid, amount);

			Lang_SendText(targetid, "ADMIN_COMMAND_WARN_GIVE_PLAYER", playername, playerid, amount, warnword, reason);
			Lang_SendText(playerid, "ADMIN_COMMAND_WARN_GIVE_SELF", targetname, targetid, amount, warnword, reason);
		}
	} else {
		Lang_SendText(playerid, "ADMIN_COMMAND_WARN_HELP");
	}

	return 1;
}
