/*
	About: fuel station system
	Author: ziggi
*/

#if defined _fuelstation_included
	#endinput
#endif

#define _fuelstation_included
#pragma library fuelstation

/*
	Defines
*/

#define INVALID_FUELSTATION_ID -1

/*
	Enums
*/

enum e_Fuelstation_Info {
	Float:e_fsPosX,
	Float:e_fsPosY,
	Float:e_fsPosZ,
	STREAMER_TAG_AREA e_fsAreaID,
}

/*
	Vars
*/

static gFuelstation[][e_Fuelstation_Info] = {
	{1595.5406, 2198.0520, 10.3863},
	{2202.0649, 2472.6697, 10.5677},
	{2115.1929, 919.9908, 10.5266},
	{2640.7209, 1105.9565, 10.5274},
	{608.5971, 1699.6238, 6.9922},
	{618.4878, 1684.5792, 6.9922},
	{2146.3467, 2748.2893, 10.5245},
	{-1679.4595, 412.5129, 6.9973},
	{-1327.5607, 2677.4316, 49.8093},
	{-1470.0050, 1863.2375, 32.3521},
	{-2409.2200, 976.2798, 45.2969},
	{-2244.1396, -2560.5833, 31.9219},
	{-1606.0544, -2714.3083, 48.5335},
	{1937.4293, -1773.1865, 13.3828},
	{-91.3854, -1169.9175, 2.4213},
	{660.4590, -565.0394, 16.3359},
	{1383.1796, 461.9314, 20.1255}
};

static
	gPlayerStationID[MAX_PLAYERS] = {INVALID_FUELSTATION_ID, ...};

/*
	Functions
*/

Fuelstation_OnGameModeInit()
{
	if (!IsVehicleFuelEnabled()) {
		return 0;
	}
	
	for (new i = 0; i < sizeof(gFuelstation); i++) {
		CreateDynamicMapIcon(gFuelstation[i][e_fsPosX], gFuelstation[i][e_fsPosY], gFuelstation[i][e_fsPosZ], 55, 0);
		CreateDynamicPickup(1650, 23, gFuelstation[i][e_fsPosX], gFuelstation[i][e_fsPosY], gFuelstation[i][e_fsPosZ]);
		CreateDynamic3DTextLabel(
				_(FUEL_STATION_3DTEXT), 0xFFFFFFFF,
				gFuelstation[i][e_fsPosX], gFuelstation[i][e_fsPosY], gFuelstation[i][e_fsPosZ], 20.0,
				.testlos = 1
			);
		gFuelstation[i][e_fsAreaID] = CreateDynamicSphere(gFuelstation[i][e_fsPosX], gFuelstation[i][e_fsPosY], gFuelstation[i][e_fsPosZ], 10.0);
	}
	return 1;
}

Fuelstation_OnPlayerStateChange(playerid, newstate, oldstate)
{
	#pragma unused oldstate
	if (newstate != PLAYER_STATE_DRIVER) {
		return 0;
	}

	if (gPlayerStationID[playerid] != INVALID_FUELSTATION_ID) {
		Message_Alert(playerid, "", _(VEHICLE_FUEL_ENTER_AREA_ALERT), 2000);
	}

	return 1;
}

Fuelstation_OnPlayerEnterDyArea(playerid, STREAMER_TAG_AREA areaid)
{
	new stid = INVALID_FUELSTATION_ID;

	for (new i = 0; i < sizeof(gFuelstation); i++) {
		if (gFuelstation[i][e_fsAreaID] == areaid) {
			stid = i;
			break;
		}
	}

	if (stid == INVALID_FUELSTATION_ID) {
		return 0;
	}

	gPlayerStationID[playerid] = stid;

	if (IsPlayerInAnyVehicle(playerid)) {
		Message_Alert(playerid, "", _(VEHICLE_FUEL_ENTER_AREA_ALERT), 2000);
	}

	return 1;
}

Fuelstation_OnPlayerLeaveDyArea(playerid, STREAMER_TAG_AREA areaid)
{
	new stid = INVALID_FUELSTATION_ID;

	for (new i = 0; i < sizeof(gFuelstation); i++) {
		if (gFuelstation[i][e_fsAreaID] == areaid) {
			stid = i;
			break;
		}
	}

	if (stid == INVALID_FUELSTATION_ID) {
		return 0;
	}

	gPlayerStationID[playerid] = INVALID_FUELSTATION_ID;

	return 1;
}

Fuelstation_OnPlayerKeyStateCh(playerid, newkeys, oldkeys)
{
	if (!IsVehicleFuelEnabled()) {
		return 0;
	}

	if (!PRESSED(KEY_SUBMISSION)) {
		return 0;
	}

	if (!IsPlayerAtFuelStation(playerid)) {
		return 0;
	}

	new vehicleid = GetPlayerVehicleID(playerid);
	if (vehicleid == 0) {
		return 0;
	}

	if (IsVehicleRefilling(vehicleid)) {
		SendClientMessage(playerid, COLOR_RED, _(VEHICLE_FUEL_IS_FUELING_ERROR));
		return 1;
	}

	new vehiclemodel = GetVehicleModel(vehicleid);
	switch (vehiclemodel) {
		case 481, 509, 510: {
			SendClientMessage(playerid, COLOR_RED, _(VEHICLE_FUEL_WITHOUT_FUEL_ENGINE));
			return 1;
		}
	}

	if (GetVehicleFuel(vehicleid) - 1.0 >= float(GetMaxVehicleModelFuel(vehiclemodel))) {
		SendClientMessage(playerid, COLOR_RED, _(VEHICLE_FUEL_FUEL_IS_FULL));
		return 1;
	}

	FillVehicle(vehicleid, playerid);
	SendClientMessage(playerid, COLOR_YELLOW, _(VEHICLE_FUEL_IS_FUELING));
	Message_Alert(playerid, "", _(VEHICLE_FUEL_IS_FUELING_ALERT), 2000);
	return 1;
}

stock Fuelstation_OnVehicleFilled(vehicleid, playerid, money)
{
	#pragma unused vehicleid
	new string[MAX_LANG_VALUE_STRING];

	format(string, sizeof(string), _(VEHICLE_FUEL_AFTER_FUEL), money);
	SendClientMessage(playerid, COLOR_YELLOW, string);

	format(string, sizeof(string), _(VEHICLE_FUEL_AFTER_FUEL_ALERT), money);
	Message_Alert(playerid, "", string);

	return 1;
}

stock IsPlayerAtFuelStation(playerid)
{
	return gPlayerStationID[playerid] != INVALID_FUELSTATION_ID;
}