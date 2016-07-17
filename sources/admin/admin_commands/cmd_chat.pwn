/*

	About: chat admin command
	Author: ziggi

*/

#if defined _admin_cmd_chat_included
	#endinput
#endif

#define _admin_cmd_chat_included

COMMAND:chat(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
		return 0;
	}

	new
		subparams[32];

	if (sscanf(params, "s[32]", subparams)) {
		Lang_SendText(playerid, $ADMIN_COMMAND_CHAT_HELP);
		return 1;
	}

	if (strcmp(subparams, "clean", true) == 0) {
		Chat_ClearAll();

		new
			playername[MAX_PLAYER_NAME + 1];

		GetPlayerName(playerid, playername, sizeof(playername));

		Lang_SendTextToAll($ADMIN_COMMAND_CHAT_CLEAN, playername, playerid);
	} else {
		Lang_SendText(playerid, $ADMIN_COMMAND_CHAT_HELP);
	}

	return 1;
}
