/*

	About: player health interface header
	Author:	ziggi

*/

#if defined _player_health_int_h_included
	#endinput
#endif

#define _player_health_int_h_included

/*
	SetPlayerHealth
*/

stock PlayerHealthTD_SetPlayerHealth(playerid, Float:health)
{
	PlayerHealthTD_UpdateString(playerid, health);
	return SetPlayerHealth(playerid, health);
}
#if defined _ALS_SetPlayerHealth
	#undef SetPlayerHealth
#else
	#define _ALS_SetPlayerHealth
#endif

#define SetPlayerHealth PlayerHealthTD_SetPlayerHealth
