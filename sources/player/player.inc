/*

	About: player header
	Author:	ziggi

*/

#if defined _player_h_included
	#endinput
#endif

#define _player_h_included


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
