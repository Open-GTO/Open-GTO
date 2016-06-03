/*

	About: say admin command
	Author: ziggi

*/

#if defined _admin_cmd_say_included
	#endinput
#endif

#define _admin_cmd_say_included

COMMAND:say(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
		return 0;
	}

	new
		playername[MAX_PLAYER_NAME + 1],
		string[MAX_LANG_VALUE_STRING];

	if (strlen(params) == 0) {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_SAY_HELP));
		return 1;
	}

	GetPlayerName(playerid, playername, sizeof(playername));
	format(string, sizeof(string), _(ADMIN_COMMAND_SAY_TEXT), playername, playerid, params);

	SendClientMessageToAll(COLOR_BLUE, string);
	return 1;
}
