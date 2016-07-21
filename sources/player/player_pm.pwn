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
		Lang_SendText(senderid, "MUTED_HELP_MESSAGE");
		return 0;
	}

	new
		string[MAX_STRING],
		length = strlen(message);

	if (length < MIN_SEND_SYMBOLS || length > MAX_SEND_SYMBOLS) {
		Lang_SendText(senderid, "PLAYER_PM_LENGTH_ERROR", MIN_SEND_SYMBOLS, MAX_SEND_SYMBOLS);
		return 0;
	}

	SendPrivateMessageToSpies(senderid, receiveid, message);

	new
		sendername[MAX_PLAYER_NAME + 1],
		receivename[MAX_PLAYER_NAME + 1];

	GetPlayerName(receiveid, receivename, sizeof(receivename));
	GetPlayerName(senderid, sendername, sizeof(sendername));

	Lang_SendText(senderid, "PLAYER_PM_FOR", receivename, receiveid, message);
	Lang_SendText(receiveid, "PLAYER_PM_FROM", sendername, senderid, message);

	Log_Player("PLAYER_PM_LOG", sendername, senderid, receivename, receiveid, message);
	return 1;
}
