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
		playername[MAX_PLAYER_NAME + 1];

	if (strlen(params) == 0) {
		Lang_SendText(playerid, $ADMIN_COMMAND_SAY_HELP);
		return 1;
	}

	GetPlayerName(playerid, playername, sizeof(playername));
	Lang_SendTextToAll($ADMIN_COMMAND_SAY_TEXT, playername, playerid, params);
	return 1;
}
