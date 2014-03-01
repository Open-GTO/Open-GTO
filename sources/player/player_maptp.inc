/*

	About: player map teleport system
	Author:	ziggi

*/

#if defined _pl_maptp_included
	#endinput
#endif

#define _pl_maptp_included
#pragma library pl_maptp


stock pl_maptp_OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
	if (!IsAllowPlayerTeleport(playerid)) {
		return 0;
	}

	new vehicleid = GetPlayerVehicleID(playerid);
	if (vehicleid != 0) {
		oSetVehiclePos(vehicleid, fX, fY, fZ);
		LinkVehicleToInterior(vehicleid, 0);
		SetVehicleVirtualWorld(vehicleid, 0);
	} else {
		oSetPlayerPos(playerid, fX, fY, fZ);
		SetPlayerInterior(playerid, 0);
		SetPlayerVirtualWorld(playerid, 0);
	}
	return 1;
}

stock IsAllowPlayerTeleport(playerid) {
	return GetPVarInt(playerid, "AllowTeleport") == 1 ? 1 : 0;
}

stock oAllowPlayerTeleport(playerid, isallow) {
	SetPVarInt(playerid, "AllowTeleport", isallow);
}
#if defined AllowPlayerTeleport
	#define AllowPlayerTeleport oAllowPlayerTeleport
#endif