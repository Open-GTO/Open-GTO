/*

	About: protection health header
	Author:	ziggi

*/

#if defined _protection_health_h_included
	#endinput
#endif

#define _protection_health_h_included
#pragma library protection_health_h

/*
	SetPlayerHealth
*/

stock ORIG_SetPlayerHealth(playerid, Float:health)
{
	return
		SetPlayerHealth(playerid, health);
}

#if defined _ALS_SetPlayerHealth
	#undef SetPlayerHealth
#else
	#define _ALS_SetPlayerHealth
#endif
 
#define SetPlayerHealth REDEF_SetPlayerHealth

/*
	GetPlayerHealth
*/

stock ORIG_GetPlayerHealth(playerid, &Float:health)
{
	return
		GetPlayerHealth(playerid, health);
}

#if defined _ALS_GetPlayerHealth
	#undef GetPlayerHealth
#else
	#define _ALS_GetPlayerHealth
#endif
 
#define GetPlayerHealth REDEF_GetPlayerHealth
