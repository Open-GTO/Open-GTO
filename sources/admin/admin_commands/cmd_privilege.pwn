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
		Lang_SendText(playerid, "ADMIN_COMMAND_PRIVILEGE_HELP");
		return 1;
	}

	new
		privileje_name[MAX_LANG_VALUE_STRING],
		targetname[MAX_PLAYER_NAME + 1],
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(playerid, playername, sizeof(playername));
	GetPlayerName(targetid, targetname, sizeof(targetname));

	if (strcmp(subcmd, "set", true) == 0) {
		SetPlayerPrivilege(targetid, privileje);

		GetPrivilegeNameForPlayer(targetid, privileje, privileje_name);
		Lang_SendText(playerid, "ADMIN_COMMAND_PRIVILEGE_SET_PLAYER", playername, playerid, privileje_name);

		GetPrivilegeNameForPlayer(playerid, privileje, privileje_name);
		Lang_SendText(playerid, "ADMIN_COMMAND_PRIVILEGE_SET_SELF", targetname, targetid, privileje_name);
	} else if (strcmp(subcmd, "get", true) == 0) {
		GetPrivilegeNameForPlayer(playerid, GetPlayerPrivilege(targetid), privileje_name);

		Lang_SendText(playerid, "ADMIN_COMMAND_PRIVILEGE_GET", targetname, targetid, privileje_name);
	} else {
		Lang_SendText(playerid, "ADMIN_COMMAND_PRIVILEGE_HELP");
	}

	return 1;
}

SSCANF:privilege(string[])
{
	new
		privileje_name[MAX_LANG_VALUE_STRING];

	foreach (new Lang:lang : LangIterator) {
		Lang_GetText(lang, "PRIVILEGE_PLAYER", privileje_name);
		if (string[0] == 'p' || string[0] == privileje_name[0]) {
			return _:PlayerPrivilegePlayer;
		}

		Lang_GetText(lang, "PRIVILEGE_MODER", privileje_name);
		if (string[0] == 'm' || string[0] == privileje_name[0]) {
			return _:PlayerPrivilegeModer;
		}

		Lang_GetText(lang, "PRIVILEGE_ADMIN", privileje_name);
		if (string[0] == 'a' || string[0] == privileje_name[0]) {
			return _:PlayerPrivilegeAdmin;
		}

		Lang_GetText(lang, "PRIVILEGE_RCON", privileje_name);
		if (string[0] == 'r' || string[0] == privileje_name[0]) {
			return _:PlayerPrivilegeRcon;
		}
	}

	return -1;
}
