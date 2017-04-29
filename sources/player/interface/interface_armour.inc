/*

	About: player armour interface header
	Author:	ziggi

*/

#if defined _player_armour_int_h_included
	#endinput
#endif

#define _player_armour_int_h_included

/*
	SetPlayerArmour
*/

stock PlayerArmourTD_SetPlayerArmour(playerid, Float:armour)
{
	PlayerArmourTD_UpdateString(playerid, armour);
	return SetPlayerArmour(playerid, armour);
}
#if defined _ALS_SetPlayerArmour
	#undef SetPlayerArmour
#else
	#define _ALS_SetPlayerArmour
#endif

#define SetPlayerArmour PlayerArmourTD_SetPlayerArmour
