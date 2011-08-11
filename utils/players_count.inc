/*
	Author:	ZiGGi
	Date:	04.06.2011
*/

#if defined _players_count_included
	#endinput
#endif

#define _players_count_included
#pragma library players_count


#define GetPlayerLastID()	players_lastID

new players_lastID = -1;

stock plcount_OnPlayerConnect(playerid)
{
	if(players_lastID < playerid) players_lastID = playerid;
	return 1;
}

stock plcount_OnPlayerDisconnect(playerid,reason)
{
	#pragma unused reason
    if(players_lastID == playerid && playerid != 0)
	{
		do players_lastID--;
		while((IsPlayerNPC(playerid) || (!IsPlayerConnected(players_lastID))) && (players_lastID > 0));
	}
	return 1;
}