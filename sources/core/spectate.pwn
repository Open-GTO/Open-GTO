/*

	About: spectate system
	Author:	ziggi

*/

#if defined _spectate_included
	#endinput
#endif

#define _spectate_included


enum e_Spec_Info {
	e_sInterior,
	e_sWorld,
	Float:e_sPosX,
	Float:e_sPosY,
	Float:e_sPosZ,
	Float:e_sPosA,
	e_SpecID,
	bool:e_AfterSpec,
}

static
	gSpecData[MAX_PLAYERS][e_Spec_Info];


forward OnPlayerSpectate(playerid, specid, status);


Spectate_OnPlayerConnect(playerid)
{
	gSpecData[playerid][e_SpecID] = INVALID_PLAYER_ID;
	gSpecData[playerid][e_AfterSpec] = false;
	return 1;
}

Spectate_OnPlayerStateChange(playerid, newstate, oldstate)
{
	#pragma unused oldstate
	if (newstate == PLAYER_STATE_DRIVER || newstate == PLAYER_STATE_PASSENGER) {
		new vehicleid = GetPlayerVehicleID(playerid);

		foreach (new id : Player) {
			if (Spectate_GetSpecID(id) == playerid) {
				PlayerSpectateVehicle(id, vehicleid);
			}
		}
	} else if (newstate == PLAYER_STATE_ONFOOT) {
		foreach (new id : Player) {
			if (Spectate_GetSpecID(id) == playerid) {
				PlayerSpectatePlayer(id, playerid);
			}
		}
	} else if (newstate == PLAYER_STATE_WASTED) {
		foreach (new id : Player) {
			if (Spectate_GetSpecID(id) == playerid) {
				Spectate_Stop(id);
			}
		}
	}
	return 1;
}

Spectate_OnPlayerSpawn(playerid)
{
	if (Spectate_IsAfterSpec(playerid)) {
		gSpecData[playerid][e_AfterSpec] = false;
	}
	return 1;
}

Spectate_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	#pragma unused oldinteriorid
	foreach (new id : Player) {
		if (Spectate_GetSpecID(id) == playerid) {
			SetPlayerInterior(id, newinteriorid);
		}
	}
	return 1;
}

stock Spectate_Start(playerid, specid)
{
	if (Spectate_GetSpecID(playerid) != INVALID_PLAYER_ID) {
		Spectate_Stop(playerid);
	}

	gSpecData[playerid][e_sInterior] = GetPlayerInterior(playerid);
	gSpecData[playerid][e_sWorld] = GetPlayerVirtualWorld(playerid);
	GetPlayerPos(playerid, gSpecData[playerid][e_sPosX], gSpecData[playerid][e_sPosY], gSpecData[playerid][e_sPosZ]);
	GetPlayerFacingAngle(playerid, gSpecData[playerid][e_sPosA]);

	Spectate_SetSpecID(playerid, specid);

	SetPlayerInterior(playerid, GetPlayerInterior(specid));
	SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(specid));

	TogglePlayerInterfaceVisibility(playerid, false);

	TogglePlayerSpectating(playerid, 1);

	new vehicleid = GetPlayerVehicleID(specid);
	if (vehicleid != 0) {
		PlayerSpectateVehicle(playerid, vehicleid);
	} else {
		PlayerSpectatePlayer(playerid, specid);
	}

	CallLocalFunction("OnPlayerSpectate", "iii", playerid, specid, 1);
}

stock Spectate_Stop(playerid)
{
	new specid = Spectate_GetSpecID(playerid);

	Spectate_SetSpecID(playerid, INVALID_PLAYER_ID);
	gSpecData[playerid][e_AfterSpec] = true;

	TogglePlayerInterfaceVisibility(playerid, true);

	TogglePlayerSpectating(playerid, 0);

	CallLocalFunction("OnPlayerSpectate", "iii", playerid, specid, 0);
}


stock Spectate_GetSpecID(playerid) {
	return gSpecData[playerid][e_SpecID];
}

stock Spectate_SetSpecID(playerid, specid) {
	gSpecData[playerid][e_SpecID] = specid;
}

stock Spectate_IsSpectating(playerid) {
	return Spectate_GetSpecID(playerid) != INVALID_PLAYER_ID;
}

stock Spectate_IsAfterSpec(playerid) {
	return _:gSpecData[playerid][e_AfterSpec];
}

stock Spectate_GetPlayerPos(playerid, &Float:x, &Float:y, &Float:z, &Float:a, &interior, &world) {
	x = gSpecData[playerid][e_sPosX];
	y = gSpecData[playerid][e_sPosY];
	z = gSpecData[playerid][e_sPosZ];
	a = gSpecData[playerid][e_sPosA];
	interior = gSpecData[playerid][e_sInterior];
	world = gSpecData[playerid][e_sWorld];
}
