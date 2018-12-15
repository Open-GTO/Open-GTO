/*

	About: privilege admin command
	Author: ziggi

*/

#if defined _admin_cmd_privilege_included
	#endinput
#endif

#define _admin_cmd_privilege_included

COMMAND:priv(playerid, params[])
{
	return cmd_privilege(playerid, params);
}

COMMAND:privilege(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeRcon)) {
		return 0;
	}

	new
		subcmd[4],
		targetid = INVALID_PLAYER_ID,
		PlayerPrivilege:privilege;

	if (sscanf(params, "s[4]uK<privilege>(player)", subcmd, targetid, _:privilege)
	    || targetid == INVALID_PLAYER_ID
	    || _:privilege == -1) {
		Lang_SendText(playerid, "ADMIN_COMMAND_PRIVILEGE_HELP");
		return 1;
	}

	new
		privilege_name[MAX_LANG_VALUE_STRING],
		targetname[MAX_PLAYER_NAME + 1],
		playername[MAX_PLAYER_NAME + 1];

	GetPlayerName(playerid, playername, sizeof(playername));
	GetPlayerName(targetid, targetname, sizeof(targetname));

	if (strcmp(subcmd, "set", true) == 0) {
		SetPlayerPrivilege(targetid, privilege);

		GetPrivilegeNameForPlayer(targetid, privilege, privilege_name);
		Lang_SendText(targetid, "ADMIN_COMMAND_PRIVILEGE_SET_PLAYER", playername, playerid, privilege_name);

		GetPrivilegeNameForPlayer(playerid, privilege, privilege_name);
		Lang_SendText(playerid, "ADMIN_COMMAND_PRIVILEGE_SET_SELF", targetname, targetid, privilege_name);
	} else if (strcmp(subcmd, "get", true) == 0) {
		GetPrivilegeNameForPlayer(playerid, GetPlayerPrivilege(targetid), privilege_name);

		Lang_SendText(playerid, "ADMIN_COMMAND_PRIVILEGE_GET", targetname, targetid, privilege_name);
	} else {
		Lang_SendText(playerid, "ADMIN_COMMAND_PRIVILEGE_HELP");
	}

	return 1;
}

SSCANF:privilege(string[])
{
	new
		privilege_name[MAX_LANG_VALUE_STRING];

	if (IsNumeric(string)) {
		new privilege = strval(string);
		if (IsPrivilegeValid(privilege)) {
			return privilege;
		}
		return -1;
	}

	foreach (new Lang:lang : LangIterator) {
		Lang_GetText(lang, "PRIVILEGE_PLAYER", privilege_name);
		if (string[0] == 'p' || string[0] == privilege_name[0]) {
			return _:PlayerPrivilegePlayer;
		}

		Lang_GetText(lang, "PRIVILEGE_MODER", privilege_name);
		if (string[0] == 'm' || string[0] == privilege_name[0]) {
			return _:PlayerPrivilegeModer;
		}

		Lang_GetText(lang, "PRIVILEGE_ADMIN", privilege_name);
		if (string[0] == 'a' || string[0] == privilege_name[0]) {
			return _:PlayerPrivilegeAdmin;
		}

		Lang_GetText(lang, "PRIVILEGE_RCON", privilege_name);
		if (string[0] == 'r' || string[0] == privilege_name[0]) {
			return _:PlayerPrivilegeRcon;
		}
	}

	return -1;
}
