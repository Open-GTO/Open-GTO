/*

	About: privileje admin command
	Author: ziggi

*/

#if defined _admin_cmd_privileje_included
	#endinput
#endif

#define _admin_cmd_privileje_included

COMMAND:priv(playerid, params[])
{
	return cmd_privileje(playerid, params);
}

COMMAND:privileje(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeRcon)) {
		return 0;
	}

	new
		subcmd[4],
		targetid = INVALID_PLAYER_ID,
		PlayerPrivilege:privileje;

	if (sscanf(params, "s[4]uK<privilege>(player)", subcmd, targetid, _:privileje)
	    || targetid == INVALID_PLAYER_ID
	    || _:privileje == -1) {
		Lang_SendText(playerid, $ADMIN_COMMAND_PRIVILEGE_HELP);
		return 1;
	}

	new
		string[MAX_LANG_VALUE_STRING],
		privileje_name[MAX_LANG_VALUE_STRING],
		targetname[MAX_PLAYER_NAME + 1],
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(playerid, playername, sizeof(playername));
	GetPlayerName(targetid, targetname, sizeof(targetname));

	if (strcmp(subcmd, "set", true) == 0) {
		SetPlayerPrivilege(targetid, privileje);

		GetPrivilegeName(Lang_GetPlayerLanguage(targetid), privileje, privileje_name);
		Lang_SendText(playerid, $ADMIN_COMMAND_PRIVILEGE_SET_PLAYER, playername, playerid, privileje_name);

		GetPrivilegeName(Lang_GetPlayerLanguage(playerid), privileje, privileje_name);
		Lang_SendText(playerid, $ADMIN_COMMAND_PRIVILEGE_SET_SELF, targetname, targetid, privileje_name);
	} else if (strcmp(subcmd, "get", true) == 0) {
		GetPrivilegeName(Lang_GetPlayerLanguage(playerid), GetPlayerPrivilege(targetid), privileje_name);

		Lang_SendText(playerid, $ADMIN_COMMAND_PRIVILEGE_GET, targetname, targetid, privileje_name);
	} else {
		Lang_SendText(playerid, $ADMIN_COMMAND_PRIVILEGE_HELP);
	}

	return 1;
}

SSCANF:privilege(string[])
{
	new
		privilge_name[MAX_LANG_VALUE_STRING];

	Lang_GetDefaultText("PRIVILEGE_PLAYER", privileje_name);

	if (strcmp(string, "p", true, 1) == 0 || strcmp(string, privilge_name, true, 1) == 0) {
		return _:PlayerPrivilegePlayer;
	}

	Lang_GetDefaultText("PRIVILEGE_MODER", privileje_name);

	if (strcmp(string, "m", true, 1) == 0 || strcmp(string, privilge_name, true, 1) == 0) {
		return _:PlayerPrivilegeModer;
	}

	Lang_GetDefaultText("PRIVILEGE_ADMIN", privileje_name);

	if (strcmp(string, "a", true, 1) == 0 || strcmp(string, privilge_name, true, 1) == 0) {
		return _:PlayerPrivilegeAdmin;
	}

	Lang_GetDefaultText("PRIVILEGE_RCON", privileje_name);

	if (strcmp(string, "r", true, 1) == 0 || strcmp(string, privilge_name, true, 1) == 0) {
		return _:PlayerPrivilegeRcon;
	}

	return -1;
}
