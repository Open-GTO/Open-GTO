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
		Lang_SendText(playerid, "ADMIN_COMMAND_GETINFO_HELP");
		return 1;
	}

	if (targetid == -1) {
		Lang_SendText(playerid, "ADMIN_COMMAND_GETINFO_TARGET_ERROR");
		return 1;
	}

	new ip_port[15 + 1 + 4 + 1];
	NetStats_GetIpPort(targetid, ip_port, sizeof(ip_port));
	Lang_SendText(playerid, "ADMIN_COMMAND_NETSTATS_0", ip_port);
	Lang_SendText(playerid, "ADMIN_COMMAND_NETSTATS_1", NetStats_GetConnectedTime(targetid));
	Lang_SendText(playerid, "ADMIN_COMMAND_NETSTATS_2", NetStats_ConnectionStatus(targetid));
	Lang_SendText(playerid, "ADMIN_COMMAND_NETSTATS_3", NetStats_PacketLossPercent(targetid));
	Lang_SendText(playerid, "ADMIN_COMMAND_NETSTATS_4", NetStats_MessagesReceived(targetid));
	Lang_SendText(playerid, "ADMIN_COMMAND_NETSTATS_5", NetStats_MessagesSent(targetid));
	Lang_SendText(playerid, "ADMIN_COMMAND_NETSTATS_6", NetStats_MessagesRecvPerSecond(targetid));
	return 1;
}
