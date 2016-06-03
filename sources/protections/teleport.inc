/*

	About: protection health header
	Author:	ziggi

*/

#if defined _protection_teleport_h_included
	#endinput
#endif

#define _protection_teleport_h_included

/*
	SetPlayerPos
*/

stock ORIG_SetPlayerPos(playerid, Float:x, Float:y, Float:z)
{
	return
		SetPlayerPos(playerid, x, y, z);
}

#if defined _ALS_SetPlayerPos
	#undef SetPlayerPos
#else
	#define _ALS_SetPlayerPos
#endif

#define SetPlayerPos REDEF_SetPlayerPos

/*
	SetVehiclePos
*/

stock ORIG_SetVehiclePos(vehicleid, Float:x, Float:y, Float:z)
{
	return
		SetVehiclePos(vehicleid, x, y, z);
}

#if defined _ALS_SetVehiclePos
	#undef SetVehiclePos
#else
	#define _ALS_SetVehiclePos
#endif

#define SetVehiclePos REDEF_SetVehiclePos
