/*

	About: player health text draw header
	Author:	ziggi

*/

#if defined _player_health_td_h_included
	#endinput
#endif

#define _player_health_td_h_included

/*
	SetPlayerHealth
*/

stock PlayerHealth_SetPlayerHealth(playerid, Float:health)
{
	PlayerHealth_UpdateString(playerid, health);
	return SetPlayerHealth(playerid, health);
}
#if defined _ALS_SetPlayerHealth
	#undef SetPlayerHealth
#else
	#define _ALS_SetPlayerHealth
#endif
 
#define SetPlayerHealth PlayerHealth_SetPlayerHealth
