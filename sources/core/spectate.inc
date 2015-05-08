/*

	About: spectate system
	Author:	ziggi

*/

#if defined _spectate_included
	#endinput
#endif

#define _spectate_included
#pragma library spectate


enum e_Spec_Info {
	e_Interior,
	e_World,
	Float:e_Pos[4],
	e_SpecID,
	bool:e_AfterSpec,
}

static
	spec_data[MAX_PLAYERS][e_Spec_Info];


forward OnPlayerSpectate(playerid, specid, status);


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
		spec_data[playerid][e_AfterSpec] = false;
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
	spec_data[playerid][e_Interior] = GetPlayerInterior(playerid);
	spec_data[playerid][e_World] = GetPlayerVirtualWorld(playerid);
	GetPlayerPos(playerid, spec_data[playerid][e_Pos][0], spec_data[playerid][e_Pos][1], spec_data[playerid][e_Pos][2]);
	GetPlayerFacingAngle(playerid, spec_data[playerid][e_Pos][3]);

	Spectate_SetSpecID(playerid, specid);

	SetPlayerInterior(playerid, GetPlayerInterior(specid));
	SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(specid));

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
	spec_data[playerid][e_AfterSpec] = true;

	TogglePlayerSpectating(playerid, 0);

	CallLocalFunction("OnPlayerSpectate", "iii", playerid, specid, 0);
}


stock Spectate_GetSpecID(playerid) {
	return spec_data[playerid][e_SpecID];
}

stock Spectate_SetSpecID(playerid, specid) {
	spec_data[playerid][e_SpecID] = specid;
}

stock Spectate_IsSpectating(playerid) {
	return Spectate_GetSpecID(playerid) != INVALID_PLAYER_ID;
}

stock Spectate_IsAfterSpec(playerid) {
	return spec_data[playerid][e_AfterSpec] ? 1 : 0;
}

stock Spectate_GetPlayerPos(playerid, &Float:x, &Float:y, &Float:z, &Float:a, &interior, &world) {
	x = spec_data[playerid][e_Pos][0];
	y = spec_data[playerid][e_Pos][1];
	z = spec_data[playerid][e_Pos][2];
	a = spec_data[playerid][e_Pos][3];
	interior = spec_data[playerid][e_Interior];
	world = spec_data[playerid][e_World];
}