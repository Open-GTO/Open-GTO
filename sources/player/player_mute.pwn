/*

	About: player mute system
	Author:	ziggi

*/

#if defined _pl_mute_included
	#endinput
#endif

#define _pl_mute_included

/*
	Defines
*/

#if !defined MAX_MUTE_REASON_LENGTH
	#define MAX_MUTE_REASON_LENGTH 64
#endif

/*
	Vars
*/

static
	gPlayerMutedCount[MAX_PLAYERS],
	gPlayerMuteTime[MAX_PLAYERS];

/*
	For public
*/

stock Player_Mute_OnPlayerText(playerid, text[])
{
	#pragma unused text

	if (IsPlayerMuted(playerid)) {
		SendClientMessage(playerid, COLOR_RED, _(MUTED_HELP_MESSAGE));
		return 0;
	}

	return 1;
}

/*
	For timer
*/

stock MutePlayerTimer(playerid)
{
	static
		mutetime;

	mutetime = GetPlayerMuteTime(playerid);

	if (mutetime != 0 && gettime() >= mutetime) {
		UnMutePlayer(playerid);

		new string[MAX_LANG_VALUE_STRING];
		GetPlayerName(playerid, string, sizeof(string));
		format(string, sizeof(string), _(ADMIN_MUTE_UNMUTED), string, playerid);
		SendClientMessageToAll(COLOR_YELLOW, string);
	}
}

/*
	Functions
*/

stock MutePlayer(playerid, mutetime)
{
	SetPlayerMuteTime(playerid, gettime() + mutetime);
	AddPlayerMutedCount(playerid, 1);
}

stock UnMutePlayer(playerid)
{
	SetPlayerMuteTime(playerid, 0);
}

stock GetPlayerMutedCount(playerid) {
	return gPlayerMutedCount[playerid];
}

stock SetPlayerMutedCount(playerid, amount) {
	gPlayerMutedCount[playerid] = amount;
}

stock AddPlayerMutedCount(playerid, amount = 1) {
	gPlayerMutedCount[playerid] += amount;
}

stock GetPlayerMuteTime(playerid) {
	return gPlayerMuteTime[playerid];
}

stock SetPlayerMuteTime(playerid, time) {
	gPlayerMuteTime[playerid] = time;
}

stock IsPlayerMuted(playerid) {
	return gPlayerMuteTime[playerid] != 0;
}
