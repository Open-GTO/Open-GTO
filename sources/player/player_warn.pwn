/*

	About: player warn system
	Author:	ziggi

*/

#if defined _player_warn_included
	#endinput
#endif

#define _player_warn_included

/*
	Defines
*/

#if !defined MAX_WARN_REASON_LENGTH
	#define MAX_WARN_REASON_LENGTH 64
#endif

#if !defined MAX_PLAYER_WARNS
	#define MAX_PLAYER_WARNS 3
#endif

/*
	Vars
*/

static
	gPlayerWarns[MAX_PLAYERS];

/*
	Vars functions
*/

stock SetPlayerWarnsCount(playerid, count)
{
	gPlayerWarns[playerid] = count;

	if (gPlayerWarns[playerid] >= MAX_PLAYER_WARNS) {
		JailPlayer(playerid);

		new string[MAX_LANG_VALUE_STRING];
		GetPlayerName(playerid, string, sizeof(string));
		format(string, sizeof(string), _(playerid, ADMIN_WARN_JAILED), string, playerid);

		SendClientMessageToAll(-1, string);
	}
}

stock GetPlayerWarnsCount(playerid)
{
	return gPlayerWarns[playerid];
}

stock GivePlayerWarn(playerid, count = 1)
{
	SetPlayerWarnsCount(playerid, GetPlayerWarnsCount(playerid) + count);
}
