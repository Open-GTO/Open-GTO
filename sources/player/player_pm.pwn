/*

	About: player pm system
	Author:	ziggi

*/

#if defined _player_pm_included
	#endinput
#endif

#define _player_pm_included

stock SendPlayerPrivateMessage(senderid, receiveid, message[])
{
	if (IsPlayerMuted(senderid)) {
		SendClientMessage(senderid, COLOR_RED, _(senderid, MUTED_HELP_MESSAGE));
		return 0;
	}

	new
		string[MAX_STRING],
		length = strlen(message);

	if (length < MIN_SEND_SYMBOLS || length > MAX_SEND_SYMBOLS) {
		format(string, sizeof(string), _(senderid, PLAYER_PM_LENGTH_ERROR), MIN_SEND_SYMBOLS, MAX_SEND_SYMBOLS);
		SendClientMessage(senderid, COLOR_PM, string);
		return 0;
	}

	SendPrivateMessageToSpies(senderid, receiveid, message);

	new
		sendername[MAX_PLAYER_NAME + 1],
		receivename[MAX_PLAYER_NAME + 1];

	GetPlayerName(receiveid, receivename, sizeof(receivename));
	GetPlayerName(senderid, sendername, sizeof(sendername));

	format(string, sizeof(string), _(senderid, PLAYER_PM_FOR), receivename, receiveid, message);
	SendClientMessage(senderid, COLOR_PM, string);

	format(string, sizeof(string), _(receiveid, PLAYER_PM_FROM), sendername, senderid, message);
	SendClientMessage(receiveid, COLOR_PM, string);

	Log_Player(_d(PLAYER_PM_LOG), sendername, senderid, receivename, receiveid, message);
	return 1;
}
