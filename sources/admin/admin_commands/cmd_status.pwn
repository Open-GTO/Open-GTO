/*

	About: status admin command
	Author: ziggi

*/

#if defined _admin_cmd_status_included
	#endinput
#endif

#define _admin_cmd_status_included
#pragma library admin_cmd_status

COMMAND:status(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid), PlayerPrivilegeAdmin) {
		return 0;
	}

	new
		subcmd[4],
		targetid = INVALID_PLAYER_ID,
		status;

	if (sscanf(params, "s[4]uI(0)", subcmd, targetid, status) || targetid == INVALID_PLAYER_ID) {
		SendClientMessage(playerid, COLOR_RED, _(ADMINC_COMMAND_STATUS_HELP));
		return 1;
	}

	new
		string[MAX_LANG_VALUE_STRING],
		targetname[MAX_PLAYER_NAME + 1],
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(playerid, playername, sizeof(playername));
	GetPlayerName(targetid, targetname, sizeof(targetname));

	if (strcmp(subcmd, "set", true) == 0) {
		SetPlayerMoney(targetid, amount);

		format(string, sizeof(string), _(ADMIN_COMMAND_MONEY_SET_PLAYER), playername, playerid, amount);
		SendClientMessage(targetid, -1, string);

		format(string, sizeof(string), _(ADMIN_COMMAND_MONEY_SET_SELF), targetname, targetid, amount);
		SendClientMessage(playerid, -1, string);
	} else if (strcmp(subcmd, "get", true) == 0) {
		amount = GetPlayerMoney(targetid);

		format(string, sizeof(string), _(ADMIN_COMMAND_MONEY_GET), targetname, targetid, amount);
		SendClientMessage(playerid, -1, string);
	}

	return 1;
}
