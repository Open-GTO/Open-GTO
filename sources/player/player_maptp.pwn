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

	TeleportPlayerToPos(playerid, fX, fY, fZ, 0.0);
	return 1;
}

stock IsAllowPlayerTeleport(playerid) {
	return GetPVarInt(playerid, "AllowTeleport") == 1 ? 1 : 0;
}

stock SetPlayerTeleportStatus(playerid, isallow) {
	SetPVarInt(playerid, "AllowTeleport", isallow);
}
