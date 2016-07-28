/*

	About: player pm spy system
	Author:	ziggi

*/

#if defined _player_pm_spy_included
	#endinput
#endif

#define _player_pm_spy_included

/*
	Vars
*/

new
	Iterator:PmSpyPlayer<MAX_PLAYERS>;

/*
	OnPlayerConnect
*/

public OnPlayerConnect(playerid)
{
	SetPrivateMessageSpyStatus(playerid, false);

	#if defined PmSpy_OnPlayerConnect
		return PmSpy_OnPlayerConnect(playerid);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerConnect
	#undef OnPlayerConnect
#else
	#define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect PmSpy_OnPlayerConnect
#if defined PmSpy_OnPlayerConnect
	forward PmSpy_OnPlayerConnect(playerid);
#endif

/*
	Functions
*/

stock SendPrivateMessageToSpies(senderid, receiveid, message[])
{
	new
		string[MAX_LANG_VALUE_STRING],
		sendername[MAX_PLAYER_NAME + 1],
		receivename[MAX_PLAYER_NAME + 1];

	GetPlayerName(senderid, sendername, sizeof(sendername));
	GetPlayerName(receiveid, receivename, sizeof(receivename));

	foreach (new i : PmSpyPlayer) {
		if (i == senderid) {
			continue;
		}

		Lang_SendText(i, "PLAYER_PMSPY_MESSAGE",
		              sendername, senderid,
		              receivename, receiveid,
		              message);
	}
}

stock SetPrivateMessageSpyStatus(playerid, bool:status)
{
	if (status) {
		Iter_Add(PmSpyPlayer, playerid);
	} else {
		Iter_Remove(PmSpyPlayer, playerid);
	}
}

stock IsPrivateMessageSpyActive(playerid)
{
	return Iter_Contains(PmSpyPlayer, playerid);
}
