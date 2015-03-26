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
	if (player_IsMuted(senderid)) {
		SendClientMessage(senderid, COLOUR_RED, _(MUTED_HELP_MESSAGE));
		return 0;
	}

	new string[MAX_STRING];

	if (strlen(message) < MIN_SEND_SYMBOLS) {
		format(string, sizeof(string), lang_texts[12][95], MIN_SEND_SYMBOLS);
		SendClientMessage(senderid, COLOUR_PM, string);
		return 0;
	}
	
	if (strlen(message) > MAX_SEND_SYMBOLS) {
		format(string, sizeof(string), lang_texts[12][92], MAX_SEND_SYMBOLS);
		SendClientMessage(senderid, COLOUR_PM, string);
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

		format(string, sizeof(string), lang_texts[9][33], sendername, senderid, receivename, receiveid, message);
		SendClientMessage(i, COLOUR_PM, string);
	}
	
	format(string, sizeof(string), lang_texts[9][31], receivename, receiveid, message);
	SendClientMessage(senderid, COLOUR_PM, string);
	
	format(string, sizeof(string), lang_texts[9][32], sendername, senderid, message);
	SendClientMessage(receiveid, COLOUR_PM, string);
	
	Log_Player(lang_texts[9][34], sendername, senderid, receivename, receiveid, message);
	return 1;
}

COMMAND:pm(playerid, params[])
{
	if (isnull(params)) {
		SendClientMessage(playerid, COLOUR_PM, lang_texts[15][26]);
		return 1;
	}

	new id = strval(params);
	if (!IsPlayerConnected(id) || id == playerid) {
		SendClientMessage(playerid, COLOUR_PM, lang_texts[15][27]);
		return 1;
	}
	
	pl_pm_Send(playerid, id, params);
	return 1;
}
