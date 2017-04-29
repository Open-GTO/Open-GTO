/*

	About: player map teleport system
	Author:	ziggi

*/

#if defined _pl_maptp_included
	#endinput
#endif

#define _pl_maptp_included

/*
	Vars
*/

static
	bool:gPlayerTeleportAllowed[MAX_PLAYERS char];

/*
	OnPlayerClickMap
*/

public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
	if (IsAllowPlayerTeleport(playerid)) {
		TeleportPlayerToPos(playerid, fX, fY, fZ, 0.0);
	}

	#if defined PMaptp_OnPlayerClickMap
		return PMaptp_OnPlayerClickMap(playerid, fX, fY, fZ);
	#else
		return 1;
	#endif
}
#if defined _ALS_OnPlayerClickMap
	#undef OnPlayerClickMap
#else
	#define _ALS_OnPlayerClickMap
#endif

#define OnPlayerClickMap PMaptp_OnPlayerClickMap
#if defined PMaptp_OnPlayerClickMap
	forward PMaptp_OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ);
#endif

/*
	Functions
*/

stock IsAllowPlayerTeleport(playerid) {
	return _:gPlayerTeleportAllowed{playerid};
}

stock SetPlayerTeleportStatus(playerid, bool:isallow) {
	gPlayerTeleportAllowed{playerid} = isallow;
}
