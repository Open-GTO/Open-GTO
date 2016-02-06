/*

	About: anti teleport hack
	Author:	ziggi

*/

#if defined _teleport_included
	#endinput
#endif

#define _teleport_included
#pragma library teleport

/*
	Vars
*/

static
	IsEnabled = PROTECTION_TELEPORT_ENABLED,
	DelayTickCount = PROTECTION_TELEPORT_DELAY,
	TimerTime = PROTECTION_TELEPORT_TIMER,
	Float:MaxInVehicleDistance = PROTECTION_TELEPORT_MAX_VEHICLE_DISTANCE,
	Float:MaxDistance = PROTECTION_TELEPORT_MAX_DISTANCE,
	LastTickCount,
	Float:gPrevPos[MAX_PLAYERS][2],
	Float:gOldPos[MAX_PLAYERS][2];


/*
	Config
*/

Prot_Teleport_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Protection_Teleport_IsEnabled", IsEnabled);
	ini_getInteger(file_config, "Protection_Teleport_DelayTime", DelayTickCount);
	ini_getInteger(file_config, "Protection_Teleport_TimerTime", TimerTime);
	ini_getFloat(file_config, "Protection_Teleport_MaxInVehicleDistance", MaxInVehicleDistance);
	ini_getFloat(file_config, "Protection_Teleport_MaxDistance", MaxDistance);
}

Prot_Teleport_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Protection_Teleport_IsEnabled", IsEnabled);
	ini_setInteger(file_config, "Protection_Teleport_DelayTime", DelayTickCount);
	ini_setInteger(file_config, "Protection_Teleport_TimerTime", TimerTime);
	ini_setFloat(file_config, "Protection_Teleport_MaxInVehicleDistance", MaxInVehicleDistance);
	ini_setFloat(file_config, "Protection_Teleport_MaxDistance", MaxDistance);
}

/*
	For public
*/

stock Prot_Teleport_OnGameModeInit()
{
	if (IsEnabled) {
		SetTimer("Prot_Teleport_Timer", TimerTime, 1);
	}
}

stock Prot_Teleport_OnPlayerSpawn(playerid)
{
	static
		Float:si_x,
		Float:si_y;

	GetSpawnInfo(playerid, _, _, si_x, si_y, _, _, _, _, _, _, _, _);
	
	gPrevPos[playerid][0] = si_x;
	gPrevPos[playerid][1] = si_y;

	gOldPos[playerid][0] = si_x;
	gOldPos[playerid][1] = si_y;
}

/*
	SetPlayerPos
*/

stock REDEF_SetPlayerPos(playerid, Float:x, Float:y, Float:z)
{
	static Float:buf_z;
	GetPlayerPos(playerid, gPrevPos[playerid][0], gPrevPos[playerid][1], buf_z);

	gOldPos[playerid][0] = x;
	gOldPos[playerid][1] = y;

	return ORIG_SetPlayerPos(playerid, x, y, z);
}


/*
	SetVehiclePos
*/

stock REDEF_SetVehiclePos(vehicleid, Float:x, Float:y, Float:z)
{
	static
		Float:prev_x,
		Float:prev_y,
		Float:prev_z;

	GetVehiclePos(vehicleid, prev_x, prev_y, prev_z);

	foreach (new playerid : Player) {
		if (GetPlayerVehicleID(playerid) == vehicleid) {
			gPrevPos[playerid][0] = prev_x;
			gPrevPos[playerid][1] = prev_y;

			gOldPos[playerid][0] = x;
			gOldPos[playerid][1] = y;
		}
	}

	return ORIG_SetVehiclePos(vehicleid, x, y, z);
}

/*
	For timer
*/

forward Prot_Teleport_Timer();
public Prot_Teleport_Timer()
{
	foreach (new playerid : Player) {
		Prot_Teleport_Sync(playerid);
	}
}

Prot_Teleport_Sync(playerid)
{
	if (!IsEnabled) {
		return 0;
	}

	if (!IsPlayerSpawned(playerid)) {
		return 0;
	}

	new
		current_tick = GetTickCount();

	if (current_tick - LastTickCount < DelayTickCount) {
		return 1;
	}

	// save last time check
	LastTickCount = current_tick;

	// calculate distance
	new
		Float:pos_x,
		Float:pos_y,
		Float:pos_z,
		Float:distance_prev,
		Float:distance_old;

	GetPlayerPos(playerid, pos_x, pos_y, pos_z);
	
	distance_prev = GetDistanceBetweenPoints(gPrevPos[playerid][0], gPrevPos[playerid][1], 0.0, pos_x, pos_y, 0.0);
	distance_old = GetDistanceBetweenPoints(gOldPos[playerid][0], gOldPos[playerid][1], 0.0, pos_x, pos_y, 0.0);

	gOldPos[playerid][0] = pos_x;
	gOldPos[playerid][1] = pos_y;

	// checks
	new Float:max_distance = IsPlayerInAnyVehicle(playerid) ? MaxInVehicleDistance : MaxDistance;

	if (distance_prev > max_distance && distance_old > max_distance) {
		Admin_SendProtectReport(playerid, _(PROTECTION_TELEPORT), distance_old, max_distance);
	}

	return 1;
}
