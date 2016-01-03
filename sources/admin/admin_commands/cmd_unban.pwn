/*

	About: unban admin command
	Author: ziggi

*/

#if defined _admin_cmd_unban_included
	#endinput
#endif

#define _admin_cmd_unban_included
#pragma library admin_cmd_unban

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
	if (!IsPlayerAdm(playerid)) {
		return 0;
	}
	
	new
		target[MAX_BAN_TARGET_LENGTH];

	if (sscanf(params, "s[" #MAX_BAN_TARGET_LENGTH "]", target)) {
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_UNBAN_HELP));
		return 1;
	}
	
	new
		string[MAX_LANG_VALUE_STRING],
		filename[MAX_STRING];

	format(filename, sizeof(filename), "%s%s"DATA_FILES_FORMAT, db_ban, target);
	
	if (ini_fileExist(filename)) {
		ini_fileRemove(filename);
		
		format(string, sizeof(string), _(ADMIN_COMMAND_UNBAN_SUCCESS), target);
	} else {
		format(string, sizeof(string), _(ADMIN_COMMAND_UNBAN_FAIL), target);
	}

	SendClientMessage(playerid, COLOR_RED, string);
	return 1;
}
