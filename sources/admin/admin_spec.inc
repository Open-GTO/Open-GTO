/*

	About: admin spec system
	Author:	ziggi

*/

#if defined _adm_spec_included
	#endinput
#endif

#define _adm_spec_included
#pragma library adm_spec


stock adm_spec_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{	
	#pragma unused ispassenger
	foreach (new id : Player) {
		if (player_IsSpectating(id) && player_GetSpecID(id) == playerid) {
			PlayerSpectateVehicle(id, vehicleid);
		}
	}
	return 1;
}

stock adm_spec_OnPlayerExitVehicle(playerid, vehicleid)
{
	#pragma unused vehicleid
	foreach (new id : Player) {
		if (player_IsSpectating(id) && player_GetSpecID(id) == playerid) {
			PlayerSpectatePlayer(id, playerid);
		}
	}
	return 1;
}

stock adm_spec_OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	#pragma unused oldinteriorid
	foreach (new id : Player) {
		if (player_IsSpectating(id) && player_GetSpecID(id) == playerid) {
			SetPlayerInterior(id, newinteriorid);
		}
	}
	return 1;
}

stock adm_spec_OnPlayerDeath(playerid, killerid, reason)
{
	#pragma unused reason, killerid
	foreach (new id : Player) {
		if (player_IsSpectating(id) && player_GetSpecID(id) == playerid) {
			TogglePlayerSpectating(id, 0);
			player_SetSpectating(id, 0);
			player_SetSpecID(id, INVALID_PLAYER_ID);
			SetPlayerInterior(playerid, GetPVarInt(id, "spec_interior"));
			DeletePVar(id, "spec_interior");
		}
	}
	return 1;
}

COMMAND:spec(playerid, params[])
{
	if (!IsPlayerMod(playerid)) {
		return 0;
	}

	if (isnull(params)) {
		if (player_IsSpectating(playerid)) {
			SetPVarInt(playerid, "spec_after_off", 1);
			TogglePlayerSpectating(playerid, 0);
			player_SetSpecID(playerid, INVALID_PLAYER_ID);
			player_SetSpectating(playerid, 0);
			SetPlayerInterior(playerid, GetPVarInt(playerid, "spec_interior"));
			SetPlayerVirtualWorld(playerid, GetPVarInt(playerid, "spec_virtualworld"));
			SetPlayerSkin(playerid, GetPVarInt(playerid, "spec_skin"));
			
			GivePlayerOwnedWeapons(playerid);
			
			new Float:pos[4];
			pos[0] = GetPVarFloat(playerid, "spec_pos_x");
			pos[1] = GetPVarFloat(playerid, "spec_pos_y");
			pos[2] = GetPVarFloat(playerid, "spec_pos_z");
			pos[3] = GetPVarFloat(playerid, "spec_pos_a");
			SetPlayerPos(playerid, pos[0], pos[1], pos[2]);
			SetPlayerFacingAngle(playerid, pos[3]);
			
			DeletePVar(playerid, "spec_interior");
			DeletePVar(playerid, "spec_virtualworld");
			DeletePVar(playerid, "spec_skin");
			DeletePVar(playerid, "spec_pos_x");
			DeletePVar(playerid, "spec_pos_y");
			DeletePVar(playerid, "spec_pos_z");
			DeletePVar(playerid, "spec_pos_a");
		} else {
			SendClientMessage(playerid, COLOUR_RED, lang_texts[13][19]);
		}
		return 1;
	}

	new specid = strval(params);
	if (!IsPlayerConnected(specid) || specid == playerid) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[13][20]);
		return 1;
	}
	
	player_SetSpecID(playerid, specid);
	player_SetSpectating(playerid, 1);
	TogglePlayerSpectating(playerid, 1);
	SetPlayerInterior(playerid, GetPlayerInterior(specid));
	SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(specid));
	
	// сохраним параметры
	SetPVarInt(playerid, "spec_interior", GetPlayerInterior(playerid));
	SetPVarInt(playerid, "spec_virtualworld", GetPlayerVirtualWorld(playerid));
	SetPVarInt(playerid, "spec_skin", GetPlayerSkin(playerid));

	new Float:pos[4];
	GetPlayerPos(playerid, pos[0], pos[1], pos[2]);
	GetPlayerFacingAngle(playerid, pos[3]);

	SetPVarFloat(playerid, "spec_pos_x", pos[0]);
	SetPVarFloat(playerid, "spec_pos_y", pos[1]);
	SetPVarFloat(playerid, "spec_pos_z", pos[2]);
	SetPVarFloat(playerid, "spec_pos_a", pos[3]);
	
	SendClientMessage(playerid, COLOUR_BLUE, lang_texts[13][21]);
	
	new vehid = GetPlayerVehicleID(specid);
	if (vehid != 0) {
		PlayerSpectateVehicle(playerid, vehid);
	} else {
		PlayerSpectatePlayer(playerid, specid);
	}
	return 1;
}
