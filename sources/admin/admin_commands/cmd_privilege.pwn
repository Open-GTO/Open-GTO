/*

	About: privileje admin command
	Author: ziggi

*/

#if defined _admin_cmd_privileje_included
	#endinput
#endif

#define _admin_cmd_privileje_included
#pragma library admin_cmd_privileje

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
		|| targetid == INVALID_PLAYER_ID || _:privileje == -1) {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_PRIVILEGE_HELP));
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
		GetPrivilegeName(privileje, privileje_name);

		format(string, sizeof(string), _(ADMIN_COMMAND_PRIVILEGE_SET_PLAYER), playername, playerid, privileje_name);
		SendClientMessage(targetid, -1, string);

		format(string, sizeof(string), _(ADMIN_COMMAND_PRIVILEGE_SET_SELF), targetname, targetid, privileje_name);
		SendClientMessage(playerid, -1, string);
	} else if (strcmp(subcmd, "get", true) == 0) {
		GetPlayerPrivilegeName(targetid, privileje_name);

		format(string, sizeof(string), _(ADMIN_COMMAND_PRIVILEGE_GET), targetname, targetid, privileje_name);
		SendClientMessage(playerid, -1, string);
	}

	return 1;
}

SSCANF:privilege(string[])
{
	new
		privilge_name[MAX_LANG_VALUE_STRING];

	__(PRIVILEGE_PLAYER, privilge_name);

	if (strcmp(string, "p", true, 1) == 0 || strcmp(string, privilge_name, true, 1) == 0) {
		return _:PlayerPrivilegePlayer;
	} 

	__(PRIVILEGE_MODER, privilge_name);

	if (strcmp(string, "m", true, 1) == 0 || strcmp(string, privilge_name, true, 1) == 0) {
		return _:PlayerPrivilegeModer;
	}

	__(PRIVILEGE_ADMIN, privilge_name);

	if (strcmp(string, "a", true, 1) == 0 || strcmp(string, privilge_name, true, 1) == 0) {
		return _:PlayerPrivilegeAdmin;
	}

	__(PRIVILEGE_RCON, privilge_name);

	if (strcmp(string, "r", true, 1) == 0 || strcmp(string, privilge_name, true, 1) == 0) {
		return _:PlayerPrivilegeRcon;
	}

	return -1;
}
