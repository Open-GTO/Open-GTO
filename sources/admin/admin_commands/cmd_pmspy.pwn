/*

	About: pmspy admin command
	Author: ziggi

*/

#if defined _admin_cmd_pmspy_included
	#endinput
#endif

#define _admin_cmd_pmspy_included

COMMAND:pmspy(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeAdmin)) {
		return 0;
	}

	if (!IsPrivateMessageSpyActive(playerid)) {
		SetPrivateMessageSpyStatus(playerid, true);
		Lang_SendText(playerid, $ADMIN_COMMAND_PMSPY_ENABLED);
	} else {
		SetPrivateMessageSpyStatus(playerid, false);
		Lang_SendText(playerid, $ADMIN_COMMAND_PMSPY_DISABLED);
	}
	return 1;
}
