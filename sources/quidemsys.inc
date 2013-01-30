/*

	Название: QuidemSys v2.7
	Описание:
		Спидометр с системой бензина и заправок. У каждого вида транспорта свой объём бензобака.
		Расход топлива в зависимости от скорости. 
		Закрытие/открытие транспорта. 
		Постепенная заправка транспорта на заправке. 
		Рандомное количество топлива в баке при спавне транспорта и инициализации скрипта(не меньше половины бака).
	
	Автор: ZiGGi (ziggi.org, my@ziggi.org)
	Дата: 2012-03-04
	
	Платформа: sa-mp server версии 0.3c и выше
	Компилятор: Pawn 3.2.3664

*/

#if defined _quidemsys_included
	#endinput
#endif

#define _quidemsys_included
#pragma library quidemsys


new FuelStations[][CoordInfo] = {
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
	{1383.4221, 462.5385, 20.1506},
	{660.4590, -565.0394, 16.3359},
	{1381.7206, 459.1907, 20.3452},
	{-1605.7156, -2714.4573, 48.5335}
};

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

new Text:white, Text:black, Text:speedom[MAX_PLAYERS];
new Float:Fuel[MAX_VEHICLES];
new fill_TimerID[MAX_PLAYERS] = {0,...};

new QuidemSysEnabled = QUIDEMSYS_ENABLED;

stock QuidemSys_LoadConfig()
{
	if (!ini_Exist(ConfigDB)) return 0;
	new file_qs = ini_Open(ConfigDB);
	ini_GetInt(file_qs, "QuidemSys_Enabled", QuidemSysEnabled);
	ini_Close(file_qs);
	return 1;
}

stock QuidemSys_SaveConfig()
{
	new file_qs = (!ini_Exist(ConfigDB)) ? ini_Create(ConfigDB) : ini_Open(ConfigDB);
	ini_SetInt(file_qs, "QuidemSys_Enabled", QuidemSysEnabled);
	ini_Close(file_qs);
	return 1;
}

stock quidemsys_OnGameModeInit()
{
	QuidemSys_LoadConfig();
	if (QuidemSysEnabled != 1) return 0;
	for (new i = 0; i < sizeof(FuelStations); i++)
	{
		CreateStreamMapIcon(55, FuelStations[i][Coord_X], FuelStations[i][Coord_Y], FuelStations[i][Coord_Z]);
	}
	
	new Float:max_fuel, model;
	for (new i = 1; i < MAX_VEHICLES; i++)
	{
		if ((model = GetVehicleModel(i)) != 0)
		{
			max_fuel = FuelList[ model-400 ] / 2;
			Fuel[i] = random(floatround( max_fuel )) + max_fuel;
		}
	}
	black = TextDrawCreate(494.000000, 379.000000, "_");
	TextDrawUseBox(black, 1);
	TextDrawBoxColor(black, 0x00000085);
	TextDrawTextSize(black, 606.000000, 0.000000);
	TextDrawAlignment(black, 0);
	TextDrawBackgroundColor(black, 0x000000FF);
	TextDrawFont(black, 2);
	TextDrawLetterSize(black, 1.700000, 3.040000);
	TextDrawColor(black, 0x000000FF);
	TextDrawSetOutline(black, 1);
	TextDrawSetProportional(black, 1);
	TextDrawSetShadow(black, 1);

	white = TextDrawCreate(495.000000, 380.000000, "_");
	TextDrawUseBox(white, 1);
	TextDrawBoxColor(white, 0xAAAAAA77);
	TextDrawTextSize(white, 605.500000, 0.000000);
	TextDrawAlignment(white, 0);
	TextDrawBackgroundColor(white, 0x000000FF);
	TextDrawFont(white, 2);
	TextDrawLetterSize(white, 1.350000, 2.800000);
	TextDrawColor(white, 0xAAAAAA88);
	TextDrawSetOutline(white, 1);
	TextDrawSetProportional(white, 1);
	TextDrawSetShadow(white, 1);
	
	foreach (Player, playerid)
	{
		create_playerTD(playerid);
	}
	
	SetTimer("speed_Timer", 500, 1);
	GameMSG(lang_texts[19][20]);
	return 1;
}

stock create_playerTD(playerid)
{
	speedom[playerid] = TextDrawCreate(495.000000, 377.700000, "-");
	TextDrawAlignment(speedom[playerid], 0);
	TextDrawBackgroundColor(speedom[playerid], 0x000000FF);
	TextDrawFont(speedom[playerid], QUIDEMSYS_TD_FONT);
	TextDrawLetterSize(speedom[playerid], 0.299999, 1.000000);
	TextDrawColor(speedom[playerid], 0xFFFFFFFF);
	TextDrawSetOutline(speedom[playerid], 1);
	TextDrawSetProportional(speedom[playerid], 1);
	return 1;
}
/*
stock quidemsys_OnGameModeExit()
{
	if (QuidemSysEnabled != 1) return 0;
	TextDrawHideForAll(black);
	TextDrawHideForAll(white);
	
	TextDrawDestroy(black);
	TextDrawDestroy(white);
	
	foreach (Player, playerid)
	{
		TextDrawHideForPlayer(playerid, speedom[playerid]);
		TextDrawDestroy(speedom[playerid]);
	}
	if (SpeedTimer != 0) KillTimer(SpeedTimer);
	return 1;
}*/

stock quidemsys_OnVehicleSpawn(vehicleid)
{
	if (QuidemSysEnabled != 1) return 0;
	new Float:max_fuel = FuelList[ GetVehicleModel(vehicleid)-400 ] / 2;
	Fuel[vehicleid] = random(floatround( max_fuel )) + max_fuel;
	return 1;
}

stock quidemsys_OnPlayerCommandText(playerid, cmdtext[])
{
	if (QuidemSysEnabled != 1) return 0;
	new vehicleid = GetPlayerVehicleID(playerid);

	if (!strcmp(cmdtext, "/fill", true))
	{
		if (vehicleid == 0)
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[19][21]);
		}
		
		if (!IsPlayerAtFuelStation(playerid))
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[19][22]);
		}
		
		if (GetPVarInt(playerid, "Refueling"))
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[19][23]);
		}
		
		new vehiclemodel = GetVehicleModel(vehicleid);
		switch (vehiclemodel)
		{
			case 481, 509, 510:
			{
				return SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[19][24]);
			}
		}
		
		if (Fuel[vehicleid] >= FuelList[vehiclemodel-400])
		{
			return SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[19][25]);
		}
		
		SetPVarFloat(playerid, "OldFuel", Fuel[vehicleid]);
		fill_TimerID[playerid] = SetTimerEx("fill_Timer", 1000, 1, "i", playerid);
		SetPVarInt(playerid, "Refueling", 1);
		return 1;
	}
	
	if (!strcmp(cmdtext, "/engine", true))
	{
		if (vehicleid == 0)
		{
			return SendClientMessage(playerid, COLOUR_RED, lang_texts[19][21]);
		}
		
		if (Fuel[vehicleid] <= 0.1)
		{
			SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[19][26]);
		}
		else
		{
			new engine, lights, alarm, doors, bonnet, boot, objective;
			GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
			SetVehicleParamsEx(vehicleid, VEHICLE_PARAMS_ON, lights, alarm, doors, bonnet, boot, objective);
		}
		return 1;
	}

	return 0;
}

forward fill_Timer(playerid);
public fill_Timer(playerid)
{
   	new vehicleid = GetPlayerVehicleID(playerid);
	
	Fuel[vehicleid] += QUIDEMSYS_FUEL_SPEED;
	if (vehicleid == 0 || !IsPlayerAtFuelStation(playerid) || Fuel[vehicleid] >= FuelList[ GetVehicleModel(vehicleid)-400 ])
	{
		new RefueledAmount = floatround( Fuel[vehicleid] - GetPVarFloat(playerid, "OldFuel") ) * QUIDEMSYS_FUEL_TARIF;
		GivePlayerMoney(playerid, -RefueledAmount);
		
		new string[MAX_STRING];
		format(string, sizeof(string), lang_texts[19][27], RefueledAmount);
		SendClientMessage(playerid, COLOUR_YELLOW, string);
		
		if (fill_TimerID[playerid] != 0) KillTimer(fill_TimerID[playerid]);
		SetPVarInt(playerid, "Refueling", 0);
	}
	return 0;
}

forward speed_Timer();
public speed_Timer()
{
	new string[MAX_STRING], Float:health, vehicleid, vehiclemodel, speed[16], speed_temp[5], speed_count=0;
	
	foreach (Player, playerid)
	{
		vehicleid = GetPlayerVehicleID(playerid);
		
		if (vehicleid == 0)
		{
			TextDrawHideForPlayer(playerid, black);
			TextDrawHideForPlayer(playerid, white);
			TextDrawHideForPlayer(playerid, speedom[playerid]);
			continue;
		}
		
		vehiclemodel = GetVehicleModel(vehicleid);
		
		if (FuelList[vehiclemodel-400] == 0)
		{
			continue;
		}
		
		if (Fuel[vehicleid] <= 0.1)
		{
			new engine, lights, alarm, doors, bonnet, boot, objective;
			GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
			SetVehicleParamsEx(vehicleid, VEHICLE_PARAMS_OFF, lights, alarm, doors, bonnet, boot, objective);
			Fuel[vehicleid] = 0.0;
		}
		
		GetVehicleHealth(vehicleid, health);
		
		if (Fuel[vehicleid] > FuelList[vehiclemodel-400])
		{
			Fuel[vehicleid] = FuelList[vehiclemodel-400];
		}
		
		speed_count = GetVehicleSpeed(vehicleid);
		speed[0] = '\0';
		if (Fuel[vehicleid] == 0.0)
		{
			strcat(speed, "~W~");
		}
		else
		{
			switch (speed_count)
			{
				case 0:
				{
					strcat(speed, "~G~");
					Fuel[vehicleid] -= 0.0002;
				}
				case 1..60:
				{
					strcat(speed, "~G~");
					Fuel[vehicleid] -= 0.003;
				}
				case 61..90:
				{
					strcat(speed, "~Y~");
					Fuel[vehicleid] -= 0.006;
				}
				default:
				{
					strcat(speed, "~R~");
					Fuel[vehicleid] -= 0.010;
				}
			}
		}
		valstr(speed_temp, speed_count);
		strcat(speed, speed_temp);
		
		format(string, sizeof(string), lang_texts[19][30], speed, Fuel[vehicleid], health/10);
		
		TextDrawSetString(speedom[playerid], string);
		TextDrawShowForPlayer(playerid, speedom[playerid]);
		TextDrawShowForPlayer(playerid, black);
		TextDrawShowForPlayer(playerid, white);
	}
	return 1;
}

stock qudemsys_OnPlayerConnect(playerid)
{
	if (QuidemSysEnabled != 1) return 0;
	create_playerTD(playerid);
	return 1;
}

stock qudemsys_OnPlayerDisconnect(playerid, reason)
{
	if (QuidemSysEnabled != 1) return 0;
	#pragma unused reason
	TextDrawDestroy(speedom[playerid]);
	return 1;
}

stock qudemsys_OnPlayerStateChange(playerid, newstate, oldstate)
{
	if (QuidemSysEnabled != 1) return 0;
	new vehicleid = GetPlayerVehicleID(playerid);
	
	if (FuelList[ GetVehicleModel(vehicleid)-400 ] == 0)
	{
		return 0;
	}
	
	if (newstate == PLAYER_STATE_DRIVER)
	{
		new engine, lights, alarm, doors, bonnet, boot, objective;
		GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
		if (Fuel[vehicleid] <= 0.1)
		{
			SetVehicleParamsEx(vehicleid, VEHICLE_PARAMS_OFF, lights, alarm, doors, bonnet, boot, objective);
			SendClientMessage(playerid, COLOUR_YELLOW, lang_texts[19][26]);
		}
		else
		{
			SetVehicleParamsEx(vehicleid, VEHICLE_PARAMS_ON, lights, alarm, doors, bonnet, boot, objective);
		}
		return 1;
	}
	if (oldstate == PLAYER_STATE_DRIVER)
	{
		TextDrawHideForPlayer(playerid, black);
		TextDrawHideForPlayer(playerid, white);
		TextDrawHideForPlayer(playerid, speedom[playerid]);
		return 1;
	}
	return 1;
}

stock IsPlayerAtFuelStation(playerid)
{
	for (new fuelid = 0; fuelid < sizeof(FuelStations); fuelid++)
	{
		if (IsPlayerInRangeOfPoint(playerid, 10, FuelStations[fuelid][Coord_X], FuelStations[fuelid][Coord_Y], FuelStations[fuelid][Coord_Z]))
		{
			return 1;
		}
	}
	return 0;
}
