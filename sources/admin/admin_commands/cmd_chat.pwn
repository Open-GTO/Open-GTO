/*

	About: chat admin command
	Author: ziggi

*/

#if defined _admin_cmd_chat_included
	#endinput
#endif

#define _admin_cmd_chat_included
#pragma library admin_cmd_chat

COMMAND:chat(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
		return 0;
	}

	new
		subparams[32];

	if (sscanf(params, "s[32]", subparams)) {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_CHAT_HELP));
		return 1;
	}

	if (strcmp(subparams, "clean", true) == 0) {
		Chat_ClearAll();

		new
			string[MAX_LANG_VALUE_STRING],
			playername[MAX_PLAYER_NAME + 1];

		GetPlayerName(playerid, playername, sizeof(playername));

		format(string, sizeof(string), _(ADMIN_COMMAND_CHAT_CLEAN), playername, playerid);
		SendClientMessageToAll(-1, string);
	} else {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_CHAT_HELP));
	}

	return 1;
}
