/*
	About: fuel station system
	Author: ziggi
*/

#if defined _fuelstation_included
	#endinput
#endif

#define _fuelstation_included
#pragma library fuelstation


static gFuelstation[][CoordInfo] = {
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

Fuelstation_OnGameModeInit()
{
	if (!IsVehicleFuelEnabled()) {
		return 0;
	}
	
	for (new i = 0; i < sizeof(gFuelstation); i++) {
		CreateDynamicMapIcon(gFuelstation[i][Coord_X], gFuelstation[i][Coord_Y], gFuelstation[i][Coord_Z], 55, 0);
		CreateDynamicPickup(1650, 23, gFuelstation[i][Coord_X], gFuelstation[i][Coord_Y], gFuelstation[i][Coord_Z]);
		CreateDynamic3DTextLabel(
			_(FUEL_STATION_3DTEXT), 0xFFFFFFFF,
			gFuelstation[i][Coord_X], gFuelstation[i][Coord_Y], gFuelstation[i][Coord_Z], 20.0, .testlos = 1);
	}
	return 1;
}

Fuelst_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
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

	if (GetVehicleFuel(vehicleid) >= float(GetMaxVehicleModelFuel(vehiclemodel))) {
		SendClientMessage(playerid, COLOR_RED, _(VEHICLE_FUEL_FUEL_IS_FULL));
		return 1;
	}

	FillVehicle(vehicleid, playerid);
	SendClientMessage(playerid, COLOR_YELLOW, _(VEHICLE_FUEL_IS_FUELING));
	return 1;
}

stock IsPlayerAtFuelStation(playerid)
{
	for (new fuelid = 0; fuelid < sizeof(gFuelstation); fuelid++) {
		if (IsPlayerInRangeOfPoint(playerid, 10, gFuelstation[fuelid][Coord_X], gFuelstation[fuelid][Coord_Y], gFuelstation[fuelid][Coord_Z])) {
			return 1;
		}
	}
	return 0;
}
