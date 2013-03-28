/*

	Описание: система личного транспорта
	Автор: ziggi

*/


#if defined _pvehicle_included
	#endinput
#endif

#define _pvehicle_included
#pragma library pvehicle


#define VEHICLES_COMPONENTS 14

enum pvInfo {
	pv_ID,
	pv_Model,
	pv_Color[2],
	pv_Lock,
	pv_Paintjob,
	pv_Component[VEHICLES_COMPONENTS]
}
new PlayerVehicle[MAX_PLAYERS][MAX_PLAYER_VEHICLES][pvInfo];

stock pveh_OnPlayerDisconnect(playerid, reason)
{
	#pragma unused reason
	for (new i = 0; i < MAX_PLAYER_VEHICLES; i++) {
		if (PlayerVehicle[playerid][i][pv_Model] != 0) {
			DestroyVehicle(PlayerVehicle[playerid][i][pv_ID]);
		}
	}
	return 1;
}

stock pveh_OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
	new slot = GetPlayerVehicleSlotByID(playerid, vehicleid);
    PlayerVehicle[playerid][slot][pv_Paintjob] = paintjobid;
    return 1;
}

stock pveh_OnVehicleRespray(playerid, vehicleid, color1, color2)
{
	new slot = GetPlayerVehicleSlotByID(playerid, vehicleid);
    PlayerVehicle[playerid][slot][pv_Color][0] = color1;
    PlayerVehicle[playerid][slot][pv_Color][1] = color2;
    return 1;
}

stock pveh_OnVehicleMod(playerid, vehicleid, componentid)
{
	#pragma unused vehicleid
	new slot = GetPlayerVehicleSlotByID(playerid, vehicleid);
	for (new i = 0; i < VEHICLES_COMPONENTS; i++) {
		if (PlayerVehicle[playerid][slot][pv_Component][i] == 0) {
			PlayerVehicle[playerid][slot][pv_Component][i] = componentid;
			break;
		}
	}
	return 1;
}

stock AddPlayerVehicle(playerid, model, color1, color2, lock = 0, paintjob = -1, component[VEHICLES_COMPONENTS] = {0, ...})
{
	new slot = GetPlayerVehicleFreeSlot(playerid);
	PlayerVehicle[playerid][slot][pv_Model] = model;
	PlayerVehicle[playerid][slot][pv_Color][0] = color1;
	PlayerVehicle[playerid][slot][pv_Color][1] = color2;
	PlayerVehicle[playerid][slot][pv_Lock] = lock;
	PlayerVehicle[playerid][slot][pv_Paintjob] = paintjob;
	for (new i = 0; i < VEHICLES_COMPONENTS; i++) {
		PlayerVehicle[playerid][slot][pv_Component][i] = component[i];
	}
	return slot;
}

stock RemovePlayerVehicle(playerid, slot)
{
	if (GetVehicleModel(PlayerVehicle[playerid][slot][pv_ID]) != 0) {
		DestroyVehicle(PlayerVehicle[playerid][slot][pv_ID]);
	}
	PlayerVehicle[playerid][slot][pv_ID] = 0;
	PlayerVehicle[playerid][slot][pv_Model] = 0;
	PlayerVehicle[playerid][slot][pv_Color][0] = 0;
	PlayerVehicle[playerid][slot][pv_Color][1] = 0;
	PlayerVehicle[playerid][slot][pv_Lock] = 0;
	PlayerVehicle[playerid][slot][pv_Paintjob] = -1;
	for (new i = 0; i < VEHICLES_COMPONENTS; i++) {
		PlayerVehicle[playerid][slot][pv_Component][i] = 0;
	}
	return 1;
}

stock CreatePlayerVehicle(playerid, slot, Float:Coord_X, Float:Coord_Y, Float:Coord_Z, Float:Coord_A)
{
	new vehicleid = CreateVehicle(
		PlayerVehicle[playerid][slot][pv_Model],
		Coord_X, Coord_Y, Coord_Z, Coord_A,
		PlayerVehicle[playerid][slot][pv_Color][0], PlayerVehicle[playerid][slot][pv_Color][1],
		CarsReSpawnTime
	);
	
	PlayerVehicle[playerid][slot][pv_Model] = vehicleid;
	
	if (PlayerVehicle[playerid][slot][pv_Paintjob] != -1) {
		ChangeVehiclePaintjob(vehicleid, PlayerVehicle[playerid][slot][pv_Paintjob]);
	}
	
	for (new i = 0; i < VEHICLES_COMPONENTS; i++) {
		AddVehicleComponent(vehicleid, PlayerVehicle[playerid][slot][pv_Component][i]);
	}
	
	return 1;
}

stock CreateVehicleDbString(playerid, slot)
{
	new vehstr[MAX_STRING];
	format(vehstr, sizeof(vehstr), "%d/%d/%d/%d/%d",
		PlayerVehicle[playerid][slot][pv_Model], PlayerVehicle[playerid][slot][pv_Color][0], PlayerVehicle[playerid][slot][pv_Color][1],
		PlayerVehicle[playerid][slot][pv_Lock], PlayerVehicle[playerid][slot][pv_Paintjob]
	);
	for (new i = 0; i < VEHICLES_COMPONENTS; i++) {
		format(vehstr, sizeof(vehstr), "%s/%d", vehstr, PlayerVehicle[playerid][slot][pv_Component][i]);
	}
	return vehstr;
}

stock SetVehicleFromDbString(playerid, slot, dbstring)
{
	new idx = 0;
	PlayerVehicle[playerid][slot][pv_Model] = strval( strcharsplit(dbstring, idx, '/') );
	PlayerVehicle[playerid][slot][pv_Color][0] = strval( strcharsplit(dbstring, idx, '/') );
	PlayerVehicle[playerid][slot][pv_Color][1] = strval( strcharsplit(dbstring, idx, '/') );
	PlayerVehicle[playerid][slot][pv_Lock] = strval( strcharsplit(dbstring, idx, '/') );
	PlayerVehicle[playerid][slot][pv_Paintjob] = strval( strcharsplit(dbstring, idx, '/') );
	
	for (new i = 0; i < VEHICLES_COMPONENTS; i++) {
		PlayerVehicle[playerid][slot][pv_Component][i] = strval( strcharsplit(dbstring, idx, '/') );
	}
	return 1;
}

stock GetPlayerVehicleSlotByID(playerid, vehicleid)
{
	for (new i = 0; i < MAX_PLAYER_VEHICLES; i++) {
		if (PlayerVehicle[playerid][i][pv_ID] == vehicleid) {
			return i;
		}
	}
	return -1;
}

stock GetPlayerVehicleFreeSlot(playerid)
{
	for (new i = 0; i < MAX_PLAYER_VEHICLES; i++) {
		if (PlayerVehicle[playerid][i][pv_Model] != 0) {
			return i;
		}
	}
	return -1;
}

stock GetPlayerVehicleCount(playerid)
{
	new count = GetPlayerVehicleFreeSlot(playerid);
	if (count == -1) {
		return MAX_PLAYER_VEHICLES;
	}
	return count + 1;
}
