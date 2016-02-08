/*

	About: player team header
	Author: ziggi

*/

#if defined _player_team_h_included
	#endinput
#endif

#define _player_team_h_included

/*
	SetPlayerTeam
*/

stock ORIG_SetPlayerTeam(playerid, teamid)
{
	return
		SetPlayerTeam(playerid, teamid);
}

#if defined _ALS_SetPlayerTeam
	#undef SetPlayerTeam
#else
	#define _ALS_SetPlayerTeam
#endif
 
#define SetPlayerTeam REDEF_SetPlayerTeam

/*
	GetPlayerTeam
*/

stock ORIG_GetPlayerTeam(playerid)
{
	return
		GetPlayerTeam(playerid);
}

#if defined _ALS_GetPlayerTeam
	#undef GetPlayerTeam
#else
	#define _ALS_GetPlayerTeam
#endif
 
#define GetPlayerTeam REDEF_GetPlayerTeam
