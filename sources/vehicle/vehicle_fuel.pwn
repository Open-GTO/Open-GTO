/*

	About: vehicle fuel system
	Author:	ziggi

*/

#if defined _vehicle_fuel_included
	#endinput
#endif

#define _vehicle_fuel_included

/*
	Forwards
*/

forward Float:GetVehicleFuel(vehicleid);
forward Float:SetVehicleFuel(vehicleid, Float:amount);
forward Float:GiveVehicleFuel(vehicleid, Float:amount);
forward OnVehicleFilled(vehicleid, playerid, money);

/*
	Vars
*/

static
	IsEnabled = VEHICLE_FUEL_ENABLED,
	Float:gFuel[MAX_VEHICLES],
	Float:gOldFuel[MAX_VEHICLES],
	bool:gIsRefilling[MAX_VEHICLES char],
	Timer_Fill[MAX_PLAYERS] = {0,...};

/*
	Config
*/

Vehicle_Fuel_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Vehicle_Fuel_Enabled", IsEnabled);
}

Vehicle_Fuel_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Vehicle_Fuel_Enabled", IsEnabled);
}

/*
	OnGameModeInit
*/

Vehicle_Fuel_OnGameModeInit()
{
	if (!IsEnabled) {
		return 0;
	}

	SetTimer("Vehicle_Fuel_SpeedTimer", 300, 1);
	Log_Init("vehicle", "Vehicle Fuel System v2.7 initialized.");
	return 1;
}

/*
	OnVehicleSpawn
*/

Vehicle_Fuel_OnVehicleSpawn(vehicleid)
{
	if (!IsEnabled) {
		return 0;
	}

	SetVehicleFuel(vehicleid, -1);
	return 1;
}

/*
	OnPlayerStateChange
*/

Vehicle_Fuel_OnPlayerStateChang(playerid, newstate, oldstate)
{
	if (!IsEnabled) {
		return 0;
	}

	new vehicleid = GetPlayerVehicleID(playerid);
	new Float:max_fuel = GetVehicleModelMaxFuel( GetVehicleModel(vehicleid) );

	if (newstate == PLAYER_STATE_DRIVER && vehicleid != 0 && max_fuel != 0.0) {
		Vehicle_ShowTextdraw(playerid);

		if (gFuel[vehicleid] <= 0.1 && !VehShop_IsShopVehicle(vehicleid)) {
			Lang_SendText(playerid, "VEHICLE_FUEL_EMPTY");
		}

		Vehicle_ToggleEngine(vehicleid);
		return 1;
	}

	if (oldstate == PLAYER_STATE_DRIVER) {
		Vehicle_HideTextdraw(playerid);
		return 1;
	}

	return 1;
}

COMMAND:fill(playerid, params[])
{
	if (!IsEnabled) {
		return 0;
	}

	new vehicleid = GetPlayerVehicleID(playerid);
	if (vehicleid == 0) {
		Lang_SendText(playerid, "VEHICLE_FUEL_NOT_IN_VEHICLE");
		return 1;
	}

	if (!IsPlayerAtFuelStation(playerid)) {
		Lang_SendText(playerid, "VEHICLE_FUEL_NOT_ON_FUEL_ST");
		return 1;
	}

	if (IsVehicleRefilling(vehicleid)) {
		Lang_SendText(playerid, "VEHICLE_FUEL_IS_FUELING_ERROR");
		return 1;
	}

	new vehiclemodel = GetVehicleModel(vehicleid);

	new Float:max_fuel = GetVehicleModelMaxFuel(vehiclemodel);

	if (max_fuel == 0.0) {
		Lang_SendText(playerid, "VEHICLE_FUEL_WITHOUT_FUEL_ENGINE");
		return 1;
	}

	if (gFuel[vehicleid] >= max_fuel) {
		Lang_SendText(playerid, "VEHICLE_FUEL_FUEL_IS_FULL");
		return 1;
	}

	Lang_SendText(playerid, "VEHICLE_FUEL_IS_FUELING");
	FillVehicle(vehicleid, playerid);
	return 1;
}

stock FillVehicle(vehicleid, playerid)
{
	gOldFuel[vehicleid] = gFuel[vehicleid];
	Timer_Fill[playerid] = SetTimerEx("Vehicle_Fuel_FillTimer", 1000, 1, "i", playerid);
	ToggleVehicleRefillingStatus(vehicleid, true);
	return 1;
}

forward Vehicle_Fuel_FillTimer(playerid);
public Vehicle_Fuel_FillTimer(playerid)
{
	new vehicleid = GetPlayerVehicleID(playerid);
	new model = GetVehicleModel(vehicleid);

	gFuel[vehicleid] += VEHICLE_FUEL_SPEED;

	if (vehicleid == 0 || !IsPlayerAtFuelStation(playerid) || gFuel[vehicleid] >= GetVehicleModelMaxFuel(model)) {
		new fill_money = floatround( gFuel[vehicleid] - gOldFuel[vehicleid] ) * VEHICLE_FUEL_FILL_TARIF;
		GivePlayerMoney(playerid, -fill_money);

		if (Timer_Fill[playerid] != 0) {
			KillTimer(Timer_Fill[playerid]);
			Timer_Fill[playerid] = 0;
		}

		ToggleVehicleRefillingStatus(vehicleid, false);
	#if defined OnVehicleFilled
		OnVehicleFilled(vehicleid, playerid, fill_money);
	#else
		CallRemoteFunction("OnVehicleFilled", "iii", vehicleid, playerid, fill_money);
	#endif
	}
	return 0;
}

forward Vehicle_Fuel_SpeedTimer();
public Vehicle_Fuel_SpeedTimer()
{
	new vehicleid, Float:speed_count, Float:max_fuel;

	foreach (new playerid : Player) {
		vehicleid = GetPlayerVehicleID(playerid);
		if (vehicleid == 0) {
			continue;
		}

		max_fuel = GetVehicleModelMaxFuel( GetVehicleModel(vehicleid) );
		if (max_fuel == 0.0) {
			continue;
		}

		if (gFuel[vehicleid] <= 0.1) {
			Vehicle_ToggleEngine(vehicleid, 0);

			gFuel[vehicleid] = 0.0;
		}

		if (gFuel[vehicleid] > max_fuel) {
			gFuel[vehicleid] = max_fuel;
		}

		speed_count = GetVehicleSpeed(vehicleid);
		gFuel[vehicleid] -= (speed_count + 1.0) / float(VEHICLE_FUEL_TARIF);

		Vehicle_UpdateTextdraw(playerid, speed_count, gFuel[vehicleid]);
	}
	return 1;
}

stock IsVehicleFuelEnabled()
{
	return IsEnabled;
}

stock ToggleVehicleRefillingStatus(vehicleid, bool:toggle)
{
	gIsRefilling{vehicleid} = toggle;
}

stock IsVehicleRefilling(vehicleid)
{
	return gIsRefilling{vehicleid};
}

stock Float:GetVehicleFuel(vehicleid)
{
	return gFuel[vehicleid];
}

stock Float:SetVehicleFuel(vehicleid, Float:amount)
{
	new Float:max_fuel = GetVehicleModelMaxFuel( GetVehicleModel(vehicleid) );

	if (amount == -1.0) {
		amount = max_fuel / 2.0 + float(random(floatround( max_fuel / 2 )));
	} else if (amount > max_fuel) {
		amount = max_fuel;
	} else if (amount < 0.0) {
		amount = 0.0;
	}

	gFuel[vehicleid] = amount;
	return amount;
}

stock SetVehicleMaxFuel(vehicleid)
{
	gFuel[vehicleid] = GetVehicleModelMaxFuel( GetVehicleModel(vehicleid) );
}

stock Float:GiveVehicleFuel(vehicleid, Float:amount)
{
	return SetVehicleFuel(vehicleid, gFuel[vehicleid] + amount);
}
