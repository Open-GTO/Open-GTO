/*

	About: about admin command
	Author: ziggi

*/

#if defined _admin_cmd_about_included
	#endinput
#endif

#define _admin_cmd_about_included

COMMAND:about(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
		return 0;
	}

	new
		subparams[32];

	if (sscanf(params, "S()[32]", subparams)) {
		return 1;
	}

	if (strcmp(subparams, "dialog", true) == 0) {
		new
			string[MAX_LANG_MULTI_STRING * 3],
			temp[MAX_LANG_MULTI_STRING];

		__m(playerid, ADMIN_COMMAND_ABOUT_MODER, string);

		__m(playerid, ADMIN_COMMAND_ABOUT_ADMIN, temp);
		strcat(string, temp);

		__m(playerid, ADMIN_COMMAND_ABOUT_RCON, temp);
		strcat(string, temp);

		Dialog_Message(playerid, _(playerid, ADMIN_COMMAND_ABOUT_DIALOG_HEADER), string, _(playerid, ADMIN_COMMAND_ABOUT_DIALOG_BUTTON_OK));

	} else {
		SendClientMessage(playerid, -1, _(playerid, ADMIN_COMMAND_ABOUT_MODER_0));
		SendClientMessage(playerid, -1, _(playerid, ADMIN_COMMAND_ABOUT_MODER_1));
		SendClientMessage(playerid, -1, _(playerid, ADMIN_COMMAND_ABOUT_MODER_2));

		if (IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
			SendClientMessage(playerid, -1, _(playerid, ADMIN_COMMAND_ABOUT_ADMIN_0));
			SendClientMessage(playerid, -1, _(playerid, ADMIN_COMMAND_ABOUT_ADMIN_1));
			SendClientMessage(playerid, -1, _(playerid, ADMIN_COMMAND_ABOUT_ADMIN_2));
		}

		if (IsPlayerHavePrivilege(playerid, PlayerPrivilegeRcon)) {
			SendClientMessage(playerid, -1, _(playerid, ADMIN_COMMAND_ABOUT_RCON_0));
			SendClientMessage(playerid, -1, _(playerid, ADMIN_COMMAND_ABOUT_RCON_1));
			SendClientMessage(playerid, -1, _(playerid, ADMIN_COMMAND_ABOUT_RCON_2));
		}
	}

	return 1;
}
