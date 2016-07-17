/*

	About: netstats admin command
	Author: ziggi

*/

#if defined _admin_cmd_netstats_included
	#endinput
#endif

#define _admin_cmd_netstats_included

COMMAND:netstats(playerid, params[])
{
	if (!IsPlayerHavePrivilege(playerid, PlayerPrivilegeModer)) {
		return 0;
	}

	new
		targetid;

	if (sscanf(params, "u", targetid)) {
		Lang_SendText(playerid, $ADMIN_COMMAND_GETINFO_HELP);
		return 1;
	}

	if (targetid == -1) {
		Lang_SendText(playerid, $ADMIN_COMMAND_GETINFO_TARGET_ERROR);
		return 1;
	}

	new
		string[MAX_LANG_VALUE_STRING];

	NetStats_GetIpPort(targetid, string, sizeof(string));
	format(string, sizeof(string), _(playerid, ADMIN_COMMAND_NETSTATS_0), string);
	SendClientMessage(playerid, -1, string);

	format(string, sizeof(string), _(playerid, ADMIN_COMMAND_NETSTATS_1), NetStats_GetConnectedTime(targetid));
	SendClientMessage(playerid, -1, string);

	format(string, sizeof(string), _(playerid, ADMIN_COMMAND_NETSTATS_2), NetStats_ConnectionStatus(targetid));
	SendClientMessage(playerid, -1, string);

	format(string, sizeof(string), _(playerid, ADMIN_COMMAND_NETSTATS_3), NetStats_PacketLossPercent(targetid));
	SendClientMessage(playerid, -1, string);

	format(string, sizeof(string), _(playerid, ADMIN_COMMAND_NETSTATS_4), NetStats_MessagesReceived(targetid));
	SendClientMessage(playerid, -1, string);

	format(string, sizeof(string), _(playerid, ADMIN_COMMAND_NETSTATS_5), NetStats_MessagesSent(targetid));
	SendClientMessage(playerid, -1, string);

	format(string, sizeof(string), _(playerid, ADMIN_COMMAND_NETSTATS_6), NetStats_MessagesRecvPerSecond(targetid));
	SendClientMessage(playerid, -1, string);

	return 1;
}
