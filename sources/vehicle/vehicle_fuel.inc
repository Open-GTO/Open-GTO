/*

	About: vehicle fuel system
	Author:	ziggi

*/

#if defined _vehicle_fuel_included
	#endinput
#endif

#define _vehicle_fuel_included
#pragma library vehicle_fuel


new Text:background, PlayerText:speedom[MAX_PLAYERS];
new Float:Fuel[MAX_VEHICLES];
new fill_TimerID[MAX_PLAYERS] = {0,...};

static IsEnabled = VEHICLE_FUEL_ENABLED;

forward Float:GetVehicleFuel(vehicleid);
public Float:GetVehicleFuel(vehicleid)
{
	return Fuel[vehicleid];
}

forward Float:SetVehicleFuel(vehicleid, Float:amount);
public Float:SetVehicleFuel(vehicleid, Float:amount)
{
	new Float:max_fuel = float( GetMaxVehicleFuel( GetVehicleModel(vehicleid) ) );

	if (amount == -1) {
		amount = max_fuel / 2 + random(floatround( max_fuel / 2 ));
	} else if (amount > max_fuel) {
		amount = max_fuel;
	} else if (amount < 0) {
		amount = 0;
	}
	
	Fuel[vehicleid] = amount;
	return amount;
}

stock veh_fuel_LoadConfig(file_config)
{
	ini_getInteger(file_config, "Vehicle_Fuel_Enabled", IsEnabled);
}

stock veh_fuel_SaveConfig(file_config)
{
	ini_setInteger(file_config, "Vehicle_Fuel_Enabled", IsEnabled);
}

stock veh_fuel_OnGameModeInit()
{
	if (!IsEnabled) {
		return 0;
	}

	for (new i = 1; i <= MAX_VEHICLES; i++) {
		if (IsValidVehicle(i)) {
			SetVehicleFuel(i, -1);
		}
	}

	background = TextDrawCreate(610.470397, 368.416748, "-");
	TextDrawLetterSize(background, 0.000000, 4.251411);
	TextDrawTextSize(background, 527.411682, 0.000000);
	TextDrawAlignment(background, 1);
	TextDrawColor(background, 0);
	TextDrawUseBox(background, true);
	TextDrawBoxColor(background, 100);
	TextDrawSetShadow(background, -5);
	TextDrawSetOutline(background, 0);
	TextDrawFont(background, 0);

	SetTimer("speed_Timer", 300, 1);
	Log_Game(lang_texts[19][20]);
	return 1;
}

stock veh_fuel_OnVehicleSpawn(vehicleid)
{
	if (!IsEnabled) {
		return 0;
	}

	SetVehicleFuel(vehicleid, -1);
	return 1;
}

stock veh_fuel_OnPlayerStateChange(playerid, newstate, oldstate)
{
	if (!IsEnabled) {
		return 0;
	}
	
	new vehicleid = GetPlayerVehicleID(playerid);
	
	if (newstate == PLAYER_STATE_DRIVER && vehicleid != 0) {
		PlayerTextDrawShow(playerid, speedom[playerid]);
		TextDrawShowForPlayer(playerid, background);

		new engine, lights, alarm, doors, bonnet, boot, objective;
		GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);

		if (Fuel[vehicleid] <= 0.1) {
			SetVehicleParamsEx(vehicleid, VEHICLE_PARAMS_OFF, lights, alarm, doors, bonnet, boot, objective);

			if (!vshop_IsShopVehicle(vehicleid)) {
				SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[19][26]);
			}
		} else {
			SetVehicleParamsEx(vehicleid, VEHICLE_PARAMS_ON, lights, alarm, doors, bonnet, boot, objective);
		}
		return 1;
	}
	if (oldstate == PLAYER_STATE_DRIVER) {
		TextDrawHideForPlayer(playerid, background);
		PlayerTextDrawHide(playerid, speedom[playerid]);
		return 1;
	}
	return 1;
}

stock veh_fuel_OnPlayerConnect(playerid)
{
	speedom[playerid] = CreatePlayerTextDraw(playerid, 535.529479, 370.416564, "0 KM/H~n~0.0 L.");
	PlayerTextDrawLetterSize(playerid, speedom[playerid], 0.336627, 1.728331);
	PlayerTextDrawTextSize(playerid, speedom[playerid], 700.058609, 700.000061);
	PlayerTextDrawAlignment(playerid, speedom[playerid], 1);
	PlayerTextDrawColor(playerid, speedom[playerid], -156);
	PlayerTextDrawSetShadow(playerid, speedom[playerid], 0);
	PlayerTextDrawSetOutline(playerid, speedom[playerid], 0);
	PlayerTextDrawBackgroundColor(playerid, speedom[playerid], 51);
	PlayerTextDrawFont(playerid, speedom[playerid], VEHICLE_FUEL_TD_FONT);
	PlayerTextDrawSetProportional(playerid, speedom[playerid], 1);
	return 1;
}

COMMAND:fill(playerid, params[])
{
	if (!IsEnabled) {
		return 0;
	}

	new vehicleid = GetPlayerVehicleID(playerid);
	if (vehicleid == 0) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[19][21]);
		return 1;
	}
	
	if (!IsPlayerAtFuelStation(playerid)) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[19][22]);
		return 1;
	}
	
	if (GetPVarInt(playerid, "Refueling")) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[19][23]);
		return 1;
	}
	
	new vehiclemodel = GetVehicleModel(vehicleid);

	new max_fuel = GetMaxVehicleFuel(vehiclemodel);

	if (max_fuel == 0) {
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[19][24]);
		return 1;
	}
	
	if (Fuel[vehicleid] >= max_fuel) {
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[19][25]);
		return 1;
	}
	
	SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[19][32]);
	fill_Vehicle(vehicleid, playerid);
	return 1;
}

COMMAND:engine(playerid, params[])
{
	if (!IsEnabled) {
		return 0;
	}
	
	new vehicleid = GetPlayerVehicleID(playerid);
	if (vehicleid == 0) {
		SendClientMessage(playerid, COLOUR_RED, lang_texts[19][21]);
		return 1;
	}
	
	if (Fuel[vehicleid] <= 0.1) {
		SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[19][26]);
	} else {
		new engine, lights, alarm, doors, bonnet, boot, objective;
		GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);

		if (engine == VEHICLE_PARAMS_ON) {
			SetVehicleParamsEx(vehicleid, VEHICLE_PARAMS_OFF, lights, alarm, doors, bonnet, boot, objective);
		} else {
			SetVehicleParamsEx(vehicleid, VEHICLE_PARAMS_ON, lights, alarm, doors, bonnet, boot, objective);
		}
	}
	return 1;
}

forward speed_Timer();
public speed_Timer()
{
	new string[MAX_STRING], vehicleid, vehiclemodel, speed_count;
	
	foreach (new playerid : Player) {
		vehicleid = GetPlayerVehicleID(playerid);
		if (vehicleid == 0) {
			continue;
		}
		
		vehiclemodel = GetVehicleModel(vehicleid);
		if (GetMaxVehicleFuel(vehiclemodel) == 0) {
			continue;
		}
		
		if (Fuel[vehicleid] <= 0.1) {
			new engine, lights, alarm, doors, bonnet, boot, objective;
			GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
			SetVehicleParamsEx(vehicleid, VEHICLE_PARAMS_OFF, lights, alarm, doors, bonnet, boot, objective);

			Fuel[vehicleid] = 0.0;
		}
		
		if (Fuel[vehicleid] > GetMaxVehicleFuel(vehiclemodel)) {
			Fuel[vehicleid] = GetMaxVehicleFuel(vehiclemodel);
		}
		
		speed_count = GetVehicleSpeed(vehicleid);
		switch (speed_count) {
			case 0: {
				Fuel[vehicleid] -= 0.0001;
			}
			case 1..60: {
				Fuel[vehicleid] -= 0.0015;
			}
			case 61..90: {
				Fuel[vehicleid] -= 0.003;
			}
			default: {
				Fuel[vehicleid] -= 0.005;
			}
		}
		
		format(string, sizeof(string), lang_texts[19][30], speed_count, Fuel[vehicleid]);
		
		PlayerTextDrawSetString(playerid, speedom[playerid], string);
	}
	return 1;
}

forward fill_Timer(playerid);
public fill_Timer(playerid)
{
	new vehicleid = GetPlayerVehicleID(playerid);
	
	Fuel[vehicleid] += VEHICLE_FUEL_SPEED;
	if (vehicleid == 0 || !IsPlayerAtFuelStation(playerid) || Fuel[vehicleid] >= GetMaxVehicleFuel( GetVehicleModel(vehicleid) )) {
		new RefueledAmount = floatround( Fuel[vehicleid] - GetPVarFloat(playerid, "OldFuel") ) * VEHICLE_FUEL_TARIF;
		oGivePlayerMoney(playerid, -RefueledAmount);
		
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[19][27], RefueledAmount);
		SendClientMessage(playerid, COLOUR_YELLOW, string);
		
		if (fill_TimerID[playerid] != 0) {
			KillTimer(fill_TimerID[playerid]);
		}

		SetPVarInt(playerid, "Refueling", 0);
	}
	return 0;
}

stock fill_Vehicle(vehicleid, playerid)
{
	SetPVarFloat(playerid, "OldFuel", Fuel[vehicleid]);
	fill_TimerID[playerid] = SetTimerEx("fill_Timer", 1000, 1, "i", playerid);
	SetPVarInt(playerid, "Refueling", 1);
	return 1;
}
