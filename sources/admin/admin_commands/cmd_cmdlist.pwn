/*

	About: cmdlist admin command
	Author: ziggi

*/

#if defined _admin_cmd_cmdlist_included
	#endinput
#endif

#define _admin_cmd_cmdlist_included
#pragma library admin_cmd_cmdlist

COMMAND:cmdlist(playerid, params[])
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

		__m(ADMIN_COMMAND_CMDLIST_MODER, string);

		__m(ADMIN_COMMAND_CMDLIST_ADMIN, temp);
		strcat(string, temp);

		__m(ADMIN_COMMAND_CMDLIST_RCON, temp);
		strcat(string, temp);

		Dialog_Message(playerid, _(ADMIN_COMMAND_CMDLIST_DIALOG_HEADER), string, _(ADMIN_COMMAND_ABOUT_DIALOG_BUTTON_OK));
	} else {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_CMDLIST_HEADER));

		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_CMDLIST_MODER_0));
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_CMDLIST_MODER_1));
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_CMDLIST_MODER_2));
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_CMDLIST_MODER_3));

		if (IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_CMDLIST_ADMIN_0));
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_CMDLIST_ADMIN_1));
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_CMDLIST_ADMIN_2));
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_CMDLIST_ADMIN_3));
		}

		if (IsPlayerHavePrivilege(playerid, PlayerPrivilegeRcon)) {
			SendClientMessage(playerid, -1, _(ADMIN_COMMAND_CMDLIST_RCON_0));
		}
	}

	return 1;
}
