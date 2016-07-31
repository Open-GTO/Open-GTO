/*

	About: unban admin command
	Author: ziggi

*/

#if defined _admin_cmd_unban_included
	#endinput
#endif

#define _admin_cmd_unban_included

/*
	Defines
*/

#if !defined MAX_BAN_REASON_LENGTH
	#define MAX_BAN_REASON_LENGTH 64
#endif

#if !defined MAX_BAN_TARGET_LENGTH
	#define MAX_BAN_TARGET_LENGTH 32
#endif

/*
	Command
*/

COMMAND:unban(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
		return 0;
	}

	new
		target[MAX_BAN_TARGET_LENGTH];

	if (sscanf(params, "s[" #MAX_BAN_TARGET_LENGTH "]", target)) {
		Lang_SendText(playerid, "ADMIN_COMMAND_UNBAN_HELP");
		return 1;
	}

	new
		filename[MAX_STRING];

	format(filename, sizeof(filename), "%s%s"DATA_FILES_FORMAT, db_ban, target);

	if (ini_fileExist(filename)) {
		ini_fileRemove(filename);

		Lang_SendText(playerid, "ADMIN_COMMAND_UNBAN_SUCCESS", target);
	} else {
		Lang_SendText(playerid, "ADMIN_COMMAND_UNBAN_FAIL", target);
	}
	return 1;
}
