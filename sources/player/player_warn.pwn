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

		new playername[MAX_PLAYER_NAME + 1];
		GetPlayerName(playerid, playername, sizeof(playername));
		Lang_SendTextToAll("ADMIN_WARN_JAILED", playername, playerid);
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
