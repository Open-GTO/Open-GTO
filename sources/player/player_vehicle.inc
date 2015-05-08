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

new vehicle_increase_levels[] = {PLAYER_VEHICLES_INCREASE_LEVELS};

#define MAX_PLAYER_VEHICLES    sizeof(vehicle_increase_levels)

enum pvInfo {
	pv_ID,
	pv_Model,
	pv_Color[2],
	Float:pv_Fuel,
	pv_Lock,
	pv_Paintjob,
	pv_Component[VEHICLES_COMPONENTS]
}
new PlayerVehicle[MAX_PLAYERS][MAX_PLAYER_VEHICLES][pvInfo];

stock pveh_OnPlayerDisconnect(playerid, reason)
{
	#pragma unused reason
	for (new i = 0; i < MAX_PLAYER_VEHICLES; i++) {
		RemovePlayerVehicle(playerid, i);
	}
	return 1;
}

stock pveh_OnVehicleDeath(vehicleid, killerid)
{
	#pragma unused killerid
	foreach (new playerid : Player) {
		new slot = GetPlayerVehicleSlotByID(playerid, vehicleid);
		if (slot != -1) {
			SetTimerEx("DestroyPlayerVehicle", 4000, 0, "d", vehicleid);
			PlayerVehicle[playerid][slot][pv_ID] = 0;
			vehicleid = 0;
			break;
		}
	}
	return 1;
}

stock pveh_OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
	new slot = GetPlayerVehicleSlotByID(playerid, vehicleid);
	if (slot == -1) {
		return 0;
	}
	PlayerVehicle[playerid][slot][pv_Paintjob] = paintjobid;
	return 1;
}

stock pveh_OnVehicleRespray(playerid, vehicleid, color1, color2)
{
	new slot = GetPlayerVehicleSlotByID(playerid, vehicleid);
	if (slot == -1) {
		return 0;
	}
	PlayerVehicle[playerid][slot][pv_Color][0] = color1;
	PlayerVehicle[playerid][slot][pv_Color][1] = color2;
	return 1;
}

stock pveh_OnVehicleMod(playerid, vehicleid, componentid)
{
	#pragma unused vehicleid
	new slot = GetPlayerVehicleSlotByID(playerid, vehicleid);
	if (slot == -1) {
		return 0;
	}
	PlayerVehicle[playerid][slot][pv_Component][ GetVehicleComponentType(componentid) ] = componentid;
	return 1;
}

stock AddPlayerVehicle(playerid, model, color1, color2, Float:fuel, lock = 0, paintjob = -1, component[VEHICLES_COMPONENTS] = {0, ...})
{
	new slot = GetPlayerVehicleFreeSlot(playerid);
	PlayerVehicle[playerid][slot][pv_ID] = 0;
	PlayerVehicle[playerid][slot][pv_Model] = model;
	PlayerVehicle[playerid][slot][pv_Color][0] = color1;
	PlayerVehicle[playerid][slot][pv_Color][1] = color2;
	PlayerVehicle[playerid][slot][pv_Fuel] = fuel;
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
		PlayerVehicle[playerid][slot][pv_ID] = 0;
	}
	PlayerVehicle[playerid][slot][pv_ID] = 0;
	PlayerVehicle[playerid][slot][pv_Model] = 0;
	PlayerVehicle[playerid][slot][pv_Color][0] = 0;
	PlayerVehicle[playerid][slot][pv_Color][1] = 0;
	PlayerVehicle[playerid][slot][pv_Fuel] = 0;
	PlayerVehicle[playerid][slot][pv_Lock] = 0;
	PlayerVehicle[playerid][slot][pv_Paintjob] = -1;
	for (new i = 0; i < VEHICLES_COMPONENTS; i++) {
		PlayerVehicle[playerid][slot][pv_Component][i] = 0;
	}
	return 1;
}

stock CreatePlayerVehicle(playerid, slot, Float:pveh_X, Float:pveh_Y, Float:pveh_Z, Float:pveh_A)
{
	new vehicleid;

	// если транспорт создан
	vehicleid = PlayerVehicle[playerid][slot][pv_ID];
	if (vehicleid != 0) {
		foreach (new id : Player) {
			if(GetPlayerVehicleID(id) == vehicleid) {
				RemovePlayerFromVehicle(id);
			}
		}
		SetVehiclePos(vehicleid, pveh_X, pveh_Y, pveh_Z);
		SetVehicleZAngle(vehicleid, pveh_A);
		return vehicleid;
	}
	
	// если не создан, то создаём
	vehicleid = CreateVehicle(
		PlayerVehicle[playerid][slot][pv_Model],
		pveh_X, pveh_Y, pveh_Z, pveh_A,
		PlayerVehicle[playerid][slot][pv_Color][0], PlayerVehicle[playerid][slot][pv_Color][1],
		vehicles_GetRespawnTime()
	);
	
	PlayerVehicle[playerid][slot][pv_ID] = vehicleid;
	
	SetVehicleFuel(vehicleid, PlayerVehicle[playerid][slot][pv_Fuel]);
	
	if (PlayerVehicle[playerid][slot][pv_Paintjob] != -1) {
		ChangeVehiclePaintjob(vehicleid, PlayerVehicle[playerid][slot][pv_Paintjob]);
	}
	
	for (new i = 0; i < VEHICLES_COMPONENTS; i++) {
		AddVehicleComponent(vehicleid, PlayerVehicle[playerid][slot][pv_Component][i]);
	}
	
	return vehicleid;
}

stock CreateVehicleDbString(playerid, slot)
{
	if (PlayerVehicle[playerid][slot][pv_ID] != 0) {
		PlayerVehicle[playerid][slot][pv_Fuel] = GetVehicleFuel(PlayerVehicle[playerid][slot][pv_ID]);
	}
	new vehstr[MAX_STRING];
	format(vehstr, sizeof(vehstr), "%d/%d/%d/%f/%d/%d",
		PlayerVehicle[playerid][slot][pv_Model], PlayerVehicle[playerid][slot][pv_Color][0], PlayerVehicle[playerid][slot][pv_Color][1],
		PlayerVehicle[playerid][slot][pv_Fuel], PlayerVehicle[playerid][slot][pv_Lock], PlayerVehicle[playerid][slot][pv_Paintjob]
	);
	for (new i = 0; i < VEHICLES_COMPONENTS; i++) {
		format(vehstr, sizeof(vehstr), "%s/%d", vehstr, PlayerVehicle[playerid][slot][pv_Component][i]);
	}
	return vehstr;
}

stock SetVehicleFromDbString(playerid, slot, dbstring[])
{
	new idx = 0;
	PlayerVehicle[playerid][slot][pv_Model] = strval( strcharsplit(dbstring, idx, '/') );
	PlayerVehicle[playerid][slot][pv_Color][0] = strval( strcharsplit(dbstring, idx, '/') );
	PlayerVehicle[playerid][slot][pv_Color][1] = strval( strcharsplit(dbstring, idx, '/') );
	PlayerVehicle[playerid][slot][pv_Fuel] = floatstr( strcharsplit(dbstring, idx, '/') );
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
		if (PlayerVehicle[playerid][i][pv_Model] == 0) {
			return i;
		}
	}
	return -1;
}

stock GetPlayerVehicleCount(playerid)
{
	new count = 0;
	for (new i = 0; i < MAX_PLAYER_VEHICLES; i++) {
		if (PlayerVehicle[playerid][i][pv_Model] != 0) {
			count++;
		}
	}
	return count;
}

stock GetPlayerVehicleMaximumCount(playerid)
{
	return GetVehicleMaximumCountByLevel( GetPlayerLevel(playerid) );
}

stock GetVehicleMaximumCountByLevel(level)
{
	new count = 0;

	for (new i = 0; i < MAX_PLAYER_VEHICLES; i++) {
		if (level >= vehicle_increase_levels[i]) {
			count++;
		}
	}

	return count;
}

stock IsValidPlayerVehicleSlot(playerid, slot)
{
	if (PlayerVehicle[playerid][slot][pv_Model] == 0) {
		return 0;
	}
	return 1;
}

stock ChangePlayerVehicleColor(playerid, vehicleid, color1, color2)
{
	OnVehicleRespray(playerid, vehicleid, color1, color2);
	return ChangeVehicleColor(vehicleid, color1, color2);
}

stock GetPlayerVehicleCostBySlot(playerid, slot)
{
	return GetVehicleCost( PlayerVehicle[playerid][slot][pv_Model] );
}

stock GetPlayerVehicleModelBySlot(playerid, slot)
{
	return PlayerVehicle[playerid][slot][pv_Model];
}

stock RemovePlayerVehicleComponents(playerid, slot)
{
	for (new i = 0; i < VEHICLES_COMPONENTS; i++) {
		if (PlayerVehicle[playerid][slot][pv_ID] != 0) {
			RemoveVehicleComponent(PlayerVehicle[playerid][slot][pv_ID], PlayerVehicle[playerid][slot][pv_Component][i]);
		}
		PlayerVehicle[playerid][slot][pv_Component][i] = 0;
	}
}

forward DestroyPlayerVehicle(vehicleid);
public DestroyPlayerVehicle(vehicleid)
{
	DestroyVehicle(vehicleid);
	return 1;
}
