/*

	About: vehicle fuel system
	Author:	ziggi

*/

#if defined _vehicle_fuel_included
	#endinput
#endif

#define _vehicle_fuel_included
#pragma library vehicle_fuel


new FuelList[] = {
70, 45, 40, 298, 40, 40, 200, 80, 60, 40, 40, 40, 40, 45, 45, 40, 70, 100,
45, 40, 40, 40, 45, 45, 20, 200, 40, 70, 70, 40, 45, 60, 90, 100, 35, 0,
40, 50, 40, 40, 45, 5, 40, 50, 65, 40, 35, 90, 20, 60, 0, 40, 20, 20,
20, 60, 50, 20, 40, 45, 90, 30, 20, 35, 5, 5, 40, 40, 20, 90, 90, 20,
20, 20, 40, 40, 40, 40, 40, 45, 40, 0, 45, 45, 20, 20, 30, 90, 90, 70,
70, 40, 40, 20, 40, 45, 40, 90, 50, 50, 40, 5, 40, 40, 40, 50, 40, 40,
50, 0, 0, 90, 90, 90, 298, 298, 40, 40, 40, 400, 400, 30, 30, 30, 50, 50,
40, 40, 50, 40, 20, 20, 60, 40, 40, 40, 40, 50, 50, 20, 40, 40, 40, 50,
70, 40, 40, 40, 90, 40, 40, 40, 40, 300, 50, 40, 80, 80, 40, 40, 40, 40,
40, 90, 90, 40, 45, 45, 20, 0, 50, 10, 20, 50, 20, 40, 40, 300, 50, 50,
40, 30, 50, 20, 0, 40, 30, 40, 50, 40, 0, 0, 300, 200, 0, 20, 40, 40,
40, 50, 40, 60, 40, 40, 40, 45, 0, 0, 0, 50, 0, 0,
};

new Text:background, PlayerText:speedom[MAX_PLAYERS];
new Float:Fuel[MAX_VEHICLES];
new fill_TimerID[MAX_PLAYERS] = {0,...};

static IsEnabled = VEHICLE_FUEL_ENABLED;

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

	new Float:max_fuel, model;
	for (new i = 1; i < MAX_VEHICLES; i++) {
		if ((model = GetVehicleModel(i)) != 0) {
			max_fuel = FuelList[ model - 400 ] / 2;
			Fuel[i] = random(floatround( max_fuel )) + max_fuel;
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
	GameMSG(lang_texts[19][20]);
	return 1;
}

stock veh_fuel_OnVehicleSpawn(vehicleid)
{
	if (!IsEnabled) {
		return 0;
	}

	new Float:max_fuel = FuelList[ GetVehicleModel(vehicleid)-400 ] / 2;
	Fuel[vehicleid] = random(floatround( max_fuel )) + max_fuel;
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
	PlayerTextDrawFont(playerid, speedom[playerid], 2);
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
	switch (vehiclemodel) {
		case 481, 509, 510: {
			SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[19][24]);
			return 1;
		}
	}
	
	if (Fuel[vehicleid] >= FuelList[vehiclemodel-400]) {
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
		if (FuelList[vehiclemodel - 400] == 0) {
			continue;
		}
		
		if (Fuel[vehicleid] <= 0.1) {
			new engine, lights, alarm, doors, bonnet, boot, objective;
			GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
			SetVehicleParamsEx(vehicleid, VEHICLE_PARAMS_OFF, lights, alarm, doors, bonnet, boot, objective);

			Fuel[vehicleid] = 0.0;
		}
		
		if (Fuel[vehicleid] > FuelList[vehiclemodel - 400]) {
			Fuel[vehicleid] = FuelList[vehiclemodel - 400];
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
	if (vehicleid == 0 || !IsPlayerAtFuelStation(playerid) || Fuel[vehicleid] >= FuelList[ GetVehicleModel(vehicleid) - 400 ]) {
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

forward Float:GetVehicleFuel(vehicleid);
public Float:GetVehicleFuel(vehicleid)
{
	return Fuel[vehicleid];
}

forward Float:SetVehicleFuel(vehicleid, Float:amount);
public Float:SetVehicleFuel(vehicleid, Float:amount)
{
	if (amount == -1) {
		new Float:max_fuel = FuelList[ GetVehicleModel(vehicleid)-400 ] / 2;
		amount = random(floatround( max_fuel )) + max_fuel;
	}
	Fuel[vehicleid] = amount;
	return amount;
}

stock GetMaxVehicleFuel(vehiclemodel)
{
	return FuelList[vehiclemodel - 400];
}