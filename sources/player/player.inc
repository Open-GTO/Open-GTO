/*

	About: player header
	Author:	ziggi

*/

#if defined _player_h_included
	#endinput
#endif

#define _player_h_included
#pragma library player_h


forward Float:GetPlayerKillDeathRatio(playerid);

/*
	SetPlayerSkin
*/

stock ORIG_SetPlayerSkin(playerid, skinid)
{
	return
		SetPlayerSkin(playerid, skinid);
}

#if defined _ALS_SetPlayerSkin
	#undef SetPlayerSkin
#else
	#define _ALS_SetPlayerSkin
#endif
 
#define SetPlayerSkin REDEF_SetPlayerSkin

/*
	GetPlayerSkin
*/

stock ORIG_GetPlayerSkin(playerid)
{
	return
		GetPlayerSkin(playerid);
}

#if defined _ALS_GetPlayerSkin
	#undef GetPlayerSkin
#else
	#define _ALS_GetPlayerSkin
#endif
 
#define GetPlayerSkin REDEF_GetPlayerSkin

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
