/*

	About: player pm system
	Author:	ziggi

*/

#if defined _pl_pm_included
	#endinput
#endif

#define _pl_pm_included
#pragma library pl_pm


stock pl_pm_Send(senderid, receiveid, message[])
{
	if (IsPlayerMuted(senderid)) {
		SendClientMessage(senderid, COLOR_RED, _(MUTED_HELP_MESSAGE));
		return 0;
	}

	new string[MAX_STRING];

	if (strlen(message) < MIN_SEND_SYMBOLS) {
		format(string, sizeof(string), _(PLAYER_PM_MIN_SYMBOLS), MIN_SEND_SYMBOLS);
		SendClientMessage(senderid, COLOR_PM, string);
		return 0;
	}
	
	if (strlen(message) > MAX_SEND_SYMBOLS) {
		format(string, sizeof(string), _(PLAYER_PM_MAX_SYMBOLS), MAX_SEND_SYMBOLS);
		SendClientMessage(senderid, COLOR_PM, string);
		return 0;
	}

	new sendername[MAX_PLAYER_NAME + 1],
		receivename[MAX_PLAYER_NAME + 1];
	
	GetPlayerName(receiveid, receivename, sizeof(receivename));
	GetPlayerName(senderid, sendername, sizeof(sendername));
	
	foreach (new i : Player) {
		if (i == senderid || GetPVarInt(i, "Admin_PMshowing") != 1) {
			continue;
		}

		format(string, sizeof(string), _(PLAYER_PM_ADMIN), sendername, senderid, receivename, receiveid, message);
		SendClientMessage(i, COLOR_PM, string);
	}
	
	format(string, sizeof(string), _(PLAYER_PM_FOR), receivename, receiveid, message);
	SendClientMessage(senderid, COLOR_PM, string);
	
	format(string, sizeof(string), _(PLAYER_PM_FROM), sendername, senderid, message);
	SendClientMessage(receiveid, COLOR_PM, string);
	
	Log_Player(_(PLAYER_PM_LOG), sendername, senderid, receivename, receiveid, message);
	return 1;
}

COMMAND:pm(playerid, params[])
{
	if (isnull(params)) {
		SendClientMessage(playerid, COLOR_PM, _(COMMAND_PM_USE));
		return 1;
	}

	new id = strval(params);
	if (!IsPlayerConnected(id) || id == playerid) {
		SendClientMessage(playerid, COLOR_PM, _(COMMAND_PM_ERROR_ID));
		return 1;
	}
	
	pl_pm_Send(playerid, id, params);
	return 1;
}
