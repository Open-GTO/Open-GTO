/*

	About: player map teleport system
	Author:	ziggi

*/

#if defined _pl_maptp_included
	#endinput
#endif

#define _pl_maptp_included

stock PMaptp_OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
	if (!IsAllowPlayerTeleport(playerid)) {
		return 0;
	}

	new vehicleid = GetPlayerVehicleID(playerid);
	if (vehicleid != 0) {
		SetVehiclePos(vehicleid, fX, fY, fZ);
		LinkVehicleToInterior(vehicleid, 0);
		SetVehicleVirtualWorld(vehicleid, 0);
	} else {
		SetPlayerPos(playerid, fX, fY, fZ);
		SetPlayerInterior(playerid, 0);
		SetPlayerVirtualWorld(playerid, 0);
	}
	return 1;
}

stock IsAllowPlayerTeleport(playerid) {
	return GetPVarInt(playerid, "AllowTeleport") == 1 ? 1 : 0;
}

stock SetPlayerTeleportStatus(playerid, isallow) {
	SetPVarInt(playerid, "AllowTeleport", isallow);
}
