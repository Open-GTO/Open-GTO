/*

	About: ban admin command
	Author: ziggi

*/

#if defined _admin_cmd_ban_included
	#endinput
#endif

#define _admin_cmd_ban_included

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

COMMAND:ban(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
		return 0;
	}

	new
		target[MAX_BAN_TARGET_LENGTH],
		time,
		reason[MAX_BAN_REASON_LENGTH];

	if (sscanf(params, "s[" #MAX_BAN_TARGET_LENGTH "]K<ftime>(0)S()[" #MAX_BAN_REASON_LENGTH "]", target, time, reason)) {
		SendClientMessage(playerid, -1, _(playerid, ADMIN_COMMAND_BAN_HELP));
		SendClientMessage(playerid, -1, _(playerid, ADMIN_COMMAND_BAN_EXAMPLE));
		return 1;
	}

	oBan(target, reason, playerid, time);
	return 1;
}
