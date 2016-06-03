/*

	About: protection armour header
	Author:	ziggi

*/

#if defined _protection_armour_h_included
	#endinput
#endif

#define _protection_armour_h_included

/*
	SetPlayerArmour
*/

stock ORIG_SetPlayerArmour(playerid, Float:armour)
{
	return
		SetPlayerArmour(playerid, armour);
}

#if defined _ALS_SetPlayerArmour
	#undef SetPlayerArmour
#else
	#define _ALS_SetPlayerArmour
#endif

#define SetPlayerArmour REDEF_SetPlayerArmour

/*
	GetPlayerArmour
*/

stock ORIG_GetPlayerArmour(playerid, &Float:armour)
{
	return
		GetPlayerArmour(playerid, armour);
}

#if defined _ALS_GetPlayerArmour
	#undef GetPlayerArmour
#else
	#define _ALS_GetPlayerArmour
#endif

#define GetPlayerArmour REDEF_GetPlayerArmour
