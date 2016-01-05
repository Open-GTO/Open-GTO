/*

	About: ban admin command
	Author: ziggi

*/

#if defined _admin_cmd_ban_included
	#endinput
#endif

#define _admin_cmd_ban_included
#pragma library admin_cmd_ban

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
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_BAN_HELP));
		SendClientMessage(playerid, -1, _(ADMIN_COMMAND_BAN_EXAMPLE));
		return 1;
	}

	oBan(target, reason, playerid, time);
	return 1;
}

/*
	SSCANF:ftime
*/

SSCANF:ftime(string[])
{
	new
		length,
		time_second,
		time_type;

	length = strlen(string);
	
	if (length < 1) {
		return -1;
	}

	time_type = string[length - 1];

	strdel(string, length - 1, length);
	time_second = strval(string);

	switch (time_type) {
		case 's', 'с': {

		}
		case 'm', 'м': {
			time_second *= 60;
		}
		case 'h', 'ч': {
			time_second *= 60 * 60;
		}
		case 'd', 'д': {
			time_second *= 60 * 60 * 24;
		}
		case 'w', 'н': {
			time_second *= 60 * 60 * 24 * 7;
		}
		case 'y', 'г': {
			time_second *= 60 * 60 * 24 * 365;
		}
	}

	return time_second;
}
