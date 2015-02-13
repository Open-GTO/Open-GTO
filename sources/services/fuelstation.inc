/*
	About: fuel station system
	Author: ziggi
*/

#if defined _fuelstation_included
	#endinput
#endif

#define _fuelstation_included
#pragma library fuelstation

static IsEnabled = VEHICLE_FUEL_ENABLED;

static FuelStations[][CoordInfo] = {
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

stock fuelstation_OnGameModeInit()
{
	if (!IsEnabled) {
		return 1;
	}
	
	for (new i = 0; i < sizeof(FuelStations); i++) {
		CreateStreamMapIcon(55, FuelStations[i][Coord_X], FuelStations[i][Coord_Y], FuelStations[i][Coord_Z]);
		CreatePickup(1650 , 23, FuelStations[i][Coord_X], FuelStations[i][Coord_Y], FuelStations[i][Coord_Z]);
		Create3DTextLabel(
			"Заправка\n{33AA33}Нажмите кнопку '{FFFFFF}NUM2{33AA33}'", 0xFFFFFFFF,
			FuelStations[i][Coord_X], FuelStations[i][Coord_Y], FuelStations[i][Coord_Z],
			20.0, 0, 1
		);
	}
	return 1;
}

stock IsPlayerAtFuelStation(playerid)
{
	for (new fuelid = 0; fuelid < sizeof(FuelStations); fuelid++) {
		if (IsPlayerInRangeOfPoint(playerid, 10, FuelStations[fuelid][Coord_X], FuelStations[fuelid][Coord_Y], FuelStations[fuelid][Coord_Z])) {
			return 1;
		}
	}
	return 0;
}