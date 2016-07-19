/*

	About: cmdlist admin command
	Author: ziggi

*/

#if defined _admin_cmd_cmdlist_included
	#endinput
#endif

#define _admin_cmd_cmdlist_included

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

		Lang_GetPlayerText(playerid, "ADMIN_COMMAND_CMDLIST_MODER", string);

		if (IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
			Lang_GetPlayerText(playerid, "ADMIN_COMMAND_CMDLIST_ADMIN", temp);
			strcat(string, temp);
		}

		if (IsPlayerHavePrivilege(playerid, PlayerPrivilegeRcon)) {
			Lang_GetPlayerText(playerid, "ADMIN_COMMAND_CMDLIST_RCON", temp);
			strcat(string, temp);
		}

		Dialog_Message2(playerid,
			"ADMIN_COMMAND_CMDLIST_DIALOG_HEADER",
			string,
			"ADMIN_COMMAND_ABOUT_DIALOG_BUTTON_OK");
	} else {
		Lang_SendText(playerid, "ADMIN_COMMAND_CMDLIST_HEADER");
		Lang_SendText(playerid, $ADMIN_COMMAND_CMDLIST_MODER_0);
		Lang_SendText(playerid, $ADMIN_COMMAND_CMDLIST_MODER_1);
		Lang_SendText(playerid, $ADMIN_COMMAND_CMDLIST_MODER_2);
		Lang_SendText(playerid, $ADMIN_COMMAND_CMDLIST_MODER_3);

		if (IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
			Lang_SendText(playerid, $ADMIN_COMMAND_CMDLIST_ADMIN_0);
			Lang_SendText(playerid, $ADMIN_COMMAND_CMDLIST_ADMIN_1);
			Lang_SendText(playerid, $ADMIN_COMMAND_CMDLIST_ADMIN_2);
			Lang_SendText(playerid, $ADMIN_COMMAND_CMDLIST_ADMIN_3);
		}

		if (IsPlayerHavePrivilege(playerid, PlayerPrivilegeRcon)) {
			Lang_SendText(playerid, $ADMIN_COMMAND_CMDLIST_RCON_0);
		}
	}

	return 1;
}
