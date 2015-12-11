/*

	About: vehicle fuel system
	Author:	ziggi

*/

#if defined _vehicle_fuel_included
	#endinput
#endif

#define _vehicle_fuel_included
#pragma library vehicle_fuel

forward Float:GetVehicleFuel(vehicleid);
forward Float:SetVehicleFuel(vehicleid, Float:amount);
forward Float:GiveVehicleFuel(vehicleid, Float:amount);

static
	IsEnabled = VEHICLE_FUEL_ENABLED,
	Float:gFuel[MAX_VEHICLES],
	Float:gOldFuel[MAX_VEHICLES],
	gIsRefilling[MAX_VEHICLES char],
	Timer_Fill[MAX_PLAYERS] = {0,...};

Vehicle_Fuel_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Vehicle_Fuel_Enabled", IsEnabled);
}

Vehicle_Fuel_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Vehicle_Fuel_Enabled", IsEnabled);
}

Vehicle_Fuel_OnGameModeInit()
{
	if (!IsEnabled) {
		return 0;
	}

	for (new i = 1; i <= MAX_VEHICLES; i++) {
		if (IsValidVehicle(i)) {
			SetVehicleFuel(i, -1);
		}
	}

	SetTimer("Vehicle_Fuel_SpeedTimer", 300, 1);
	Log_Game(_(VEHICLE_FUEL_INIT));
	return 1;
}

Vehicle_Fuel_OnVehicleSpawn(vehicleid)
{
	if (!IsEnabled) {
		return 0;
	}

	SetVehicleFuel(vehicleid, -1);
	return 1;
}

Vehicle_Fuel_OnPlayerStateChang(playerid, newstate, oldstate)
{
	if (!IsEnabled) {
		return 0;
	}
	
	new vehicleid = GetPlayerVehicleID(playerid);
	
	if (newstate == PLAYER_STATE_DRIVER && vehicleid != 0) {
		Vehicle_ShowTextdraw(playerid);

		if (gFuel[vehicleid] <= 0.1 && !vshop_IsShopVehicle(vehicleid)) {
			SendClientMessage(playerid, COLOR_YELLOW, _(VEHICLE_FUEL_EMPTY));
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
		SendClientMessage(playerid, COLOR_RED, _(VEHICLE_FUEL_NOT_IN_VEHICLE));
		return 1;
	}
	
	if (!IsPlayerAtFuelStation(playerid)) {
		SendClientMessage(playerid, COLOR_RED, _(VEHICLE_FUEL_NOT_ON_FUEL_ST));
		return 1;
	}
	
	if (IsVehicleRefilling(vehicleid)) {
		SendClientMessage(playerid, COLOR_RED, _(VEHICLE_FUEL_IS_FUELING_ERROR));
		return 1;
	}
	
	new vehiclemodel = GetVehicleModel(vehicleid);

	new max_fuel = GetMaxVehicleModelFuel(vehiclemodel);

	if (max_fuel == 0) {
		SendClientMessage(playerid, COLOR_YELLOW, _(VEHICLE_FUEL_WITHOUT_FUEL_ENGINE));
		return 1;
	}
	
	if (gFuel[vehicleid] >= float(max_fuel)) {
		SendClientMessage(playerid, COLOR_YELLOW, _(VEHICLE_FUEL_FUEL_IS_FULL));
		return 1;
	}
	
	SendClientMessage(playerid, COLOR_YELLOW, _(VEHICLE_FUEL_IS_FUELING));
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

	if (vehicleid == 0 || !IsPlayerAtFuelStation(playerid) || gFuel[vehicleid] >= float(GetMaxVehicleModelFuel(model))) {
		new fill_money = floatround( gFuel[vehicleid] - gOldFuel[vehicleid] ) * VEHICLE_FUEL_FILL_TARIF;
		GivePlayerMoney(playerid, -fill_money);
		
		if (Timer_Fill[playerid] != 0) {
			KillTimer(Timer_Fill[playerid]);
			Timer_Fill[playerid] = 0;
		}

		ToggleVehicleRefillingStatus(vehicleid, false);
		
		new string[MAX_LANG_VALUE_STRING];
		format(string, sizeof(string), _(VEHICLE_FUEL_AFTER_FUEL), fill_money);
		SendClientMessage(playerid, COLOR_YELLOW, string);
	}
	return 0;
}

forward Vehicle_Fuel_SpeedTimer();
public Vehicle_Fuel_SpeedTimer()
{
	new vehicleid, speed_count, Float:max_fuel;
	
	foreach (new playerid : Player) {
		vehicleid = GetPlayerVehicleID(playerid);
		if (vehicleid == 0) {
			continue;
		}
		
		max_fuel = float(GetMaxVehicleModelFuel( GetVehicleModel(vehicleid) ));
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
		gFuel[vehicleid] -= float(speed_count + 1) / float(VEHICLE_FUEL_TARIF);

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
	gIsRefilling{vehicleid} = _:toggle;
}

stock IsVehicleRefilling(vehicleid)
{
	return gIsRefilling{vehicleid} != 0;
}

public Float:GetVehicleFuel(vehicleid)
{
	return gFuel[vehicleid];
}

public Float:SetVehicleFuel(vehicleid, Float:amount)
{
	new Float:max_fuel = float( GetMaxVehicleModelFuel( GetVehicleModel(vehicleid) ) );

	if (amount == -1) {
		amount = max_fuel / 2 + random(floatround( max_fuel / 2 ));
	} else if (amount > max_fuel) {
		amount = max_fuel;
	} else if (amount < 0) {
		amount = 0;
	}
	
	gFuel[vehicleid] = amount;
	return amount;
}

public Float:GiveVehicleFuel(vehicleid, Float:amount)
{
	return SetVehicleFuel(vehicleid, gFuel[vehicleid] + amount);
}
