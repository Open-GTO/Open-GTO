/*

	About: player armour text draw header
	Author:	ziggi

*/

#if defined _player_armour_td_h_included
	#endinput
#endif

#define _player_armour_td_h_included

/*
	SetPlayerArmour
*/

stock PlayerArmour_SetPlayerArmour(playerid, Float:armour)
{
	PlayerArmour_UpdateString(playerid, armour);
	return SetPlayerArmour(playerid, armour);
}
#if defined _ALS_SetPlayerArmour
	#undef SetPlayerArmour
#else
	#define _ALS_SetPlayerArmour
#endif
 
#define SetPlayerArmour PlayerArmour_SetPlayerArmour
