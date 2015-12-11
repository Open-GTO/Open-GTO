/*

	About: vehicle user menu
	Author: ziggi
	
*/

#if defined _vehicle_menu_included
  #endinput
#endif

#define _vehicle_menu_included
#pragma library vehicle_menu


stock vh_menu_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if (PRESSED(KEY_NO)) {
		Dialog_Show(playerid, Dialog:VehicleMenu);
		return 1;
	}

	new engine, lights, alarm, doors, bonnet, boot, objective;
	new vehicleid = GetPlayerVehicleID(playerid);
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);

	// двигатель
	if (PRESSED(KEY_ANALOG_UP)) {
		if (engine == VEHICLE_PARAMS_ON) {
			SetVehicleParamsEx(vehicleid, VEHICLE_PARAMS_OFF, lights, alarm, doors, bonnet, boot, objective);
		} else {
			SetVehicleParamsEx(vehicleid, VEHICLE_PARAMS_ON, lights, alarm, doors, bonnet, boot, objective);
		}
		return 1;
	}

	// заправить
	if (PRESSED(KEY_ANALOG_DOWN)) {
		if (vehicleid == 0) {
			SendClientMessage(playerid, COLOR_RED, _(VEHICLE_FUEL_NOT_IN_VEHICLE));
			return 1;
		}
		
		if (!IsPlayerAtFuelStation(playerid)) {
			SendClientMessage(playerid, COLOR_RED, _(VEHICLE_FUEL_NOT_ON_FUEL_ST));
			return 1;
		}
		
		if (GetPVarInt(playerid, "Refueling")) {
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
		
		SendClientMessage(playerid, COLOR_YELLOW, _(VEHICLE_FUEL_IS_FUELING));
		FillVehicle(vehicleid, playerid);
		return 1;
	}

	// радио
	if (PRESSED(KEY_ANALOG_LEFT)) {
		Dialog_Show(playerid, Dialog:VehicleRadio);
		return 1;
	}

	// фары
	if (PRESSED(KEY_ANALOG_RIGHT)) {
		if (lights == VEHICLE_PARAMS_ON) {
			SetVehicleParamsEx(vehicleid, engine, VEHICLE_PARAMS_OFF, alarm, doors, bonnet, boot, objective);
		} else {
			SetVehicleParamsEx(vehicleid, engine, VEHICLE_PARAMS_ON, alarm, doors, bonnet, boot, objective);
		}
		return 1;
	}
	return 1;
}

COMMAND:vmenu(playerid, params[])
{
	if (!IsPlayerInAnyVehicle(playerid)) {
		SendClientMessage(playerid, COLOR_RED, _(VEHICLE_NOT_IN_CAR));
		return 0;
	}
	
	Dialog_Show(playerid, Dialog:VehicleMenu);
	return 1;
}

DialogCreate:VehicleMenu(playerid)
{
	new string[512], engine, lights, alarm, doors, bonnet, boot, objective;
	new vehicleid = GetPlayerVehicleID(playerid);
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);
	
	strcat(string, "Перевернуть\n", sizeof(string));
	strcat(string, "Заправить\n", sizeof(string));
	strcat(string, "Радио\n", sizeof(string));
	
	// фары
	if (lights == VEHICLE_PARAMS_ON) {
		strcat(string, "{FF0000}Выключить  фары\n", sizeof(string));
	} else {
		strcat(string, "{00FF00}Включить фары\n", sizeof(string));
	}
	
	// двигатель
	if (engine == VEHICLE_PARAMS_ON) {
		strcat(string, "{FF0000}Выключить двигатель\n", sizeof(string));
	} else {
		strcat(string, "{00FF00}Включить двигатель\n", sizeof(string));
	}

	// капот
	if (bonnet == VEHICLE_PARAMS_ON) {
		strcat(string, "{FF0000}Закрыть капот\n", sizeof(string));
	} else {
		strcat(string, "{00FF00}Открыть капот\n", sizeof(string));
	}
	
	// багажник
	if (boot == VEHICLE_PARAMS_ON) {
		strcat(string, "{FF0000}Закрыть багажник\n", sizeof(string));
	} else {
		strcat(string, "{00FF00}Открыть багажник\n", sizeof(string));
	}
		
	Dialog_Open(playerid, Dialog:VehicleMenu, DIALOG_STYLE_LIST, "Меню транспорта", string, "ОК", "Отмена");
}

DialogResponse:VehicleMenu(playerid, response, listitem, inputtext[])
{
	if (!response) {
		return 1;
	}

	new engine, lights, alarm, doors, bonnet, boot, objective;
	new vehicleid = GetPlayerVehicleID(playerid);
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);

	switch (listitem)
	{
		// перевернуть
		case 0: {
			new Float:float_tmp;
			GetVehicleZAngle(vehicleid, float_tmp);
			SetVehicleZAngle(vehicleid, float_tmp);
			return 1;
		}
		// заправить
		case 1: {
			if (vehicleid == 0) {
				Dialog_MessageEx(playerid, Dialog:VehicleReturnMenu, _(VEHICLE_FUEL_DIALOG_HEADER), _(VEHICLE_FUEL_NOT_IN_VEHICLE), "Назад", "Отмена");
				return 1;
			}
			
			if (!IsPlayerAtFuelStation(playerid)) {
				Dialog_MessageEx(playerid, Dialog:VehicleReturnMenu, _(VEHICLE_FUEL_DIALOG_HEADER), _(VEHICLE_FUEL_NOT_ON_FUEL_ST), "Назад", "Отмена");
				return 1;
			}
			
			if (GetPVarInt(playerid, "Refueling")) {
				Dialog_MessageEx(playerid, Dialog:VehicleReturnMenu, _(VEHICLE_FUEL_DIALOG_HEADER), _(VEHICLE_FUEL_IS_FUELING_ERROR), "Назад", "Отмена");
				return 1;
			}
			
			new vehiclemodel = GetVehicleModel(vehicleid);
			switch (vehiclemodel) {
				case 481, 509, 510: {
					Dialog_MessageEx(playerid, Dialog:VehicleReturnMenu, _(VEHICLE_FUEL_DIALOG_HEADER), _(VEHICLE_FUEL_WITHOUT_FUEL_ENGINE), "Назад", "Отмена");
					return 1;
				}
			}
			
			if (GetVehicleFuel(vehicleid) >= float(GetMaxVehicleModelFuel(vehiclemodel))) {
				Dialog_MessageEx(playerid, Dialog:VehicleReturnMenu, _(VEHICLE_FUEL_DIALOG_HEADER), _(VEHICLE_FUEL_FUEL_IS_FULL), "Назад", "Отмена");
				return 1;
			}
			
			SendClientMessage(playerid, COLOR_YELLOW, _(VEHICLE_FUEL_IS_FUELING));
			FillVehicle(vehicleid, playerid);
			return 1;
		}
		// радио
		case 2: {
			Dialog_Show(playerid, Dialog:VehicleRadio);
			return 1;
		}
		// фары
		case 3: {
			if (lights == VEHICLE_PARAMS_ON) {
				SetVehicleParamsEx(vehicleid, engine, VEHICLE_PARAMS_OFF, alarm, doors, bonnet, boot, objective);
			} else {
				SetVehicleParamsEx(vehicleid, engine, VEHICLE_PARAMS_ON, alarm, doors, bonnet, boot, objective);
			}
			return 1;
		}
		// двигатель
		case 4: {
			if (engine == VEHICLE_PARAMS_ON) {
				SetVehicleParamsEx(vehicleid, VEHICLE_PARAMS_OFF, lights, alarm, doors, bonnet, boot, objective);
			} else {
				SetVehicleParamsEx(vehicleid, VEHICLE_PARAMS_ON, lights, alarm, doors, bonnet, boot, objective);
			}
			return 1;
		}
		// капот
		case 5: {
			if (bonnet == VEHICLE_PARAMS_ON) {
				SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, VEHICLE_PARAMS_OFF, boot, objective);
			} else {
				SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, VEHICLE_PARAMS_ON, boot, objective);
			}
			return 1;
		}
		// багажник
		case 6: {
			if (boot == VEHICLE_PARAMS_ON) {
				SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, VEHICLE_PARAMS_OFF, objective);
			} else {
				SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, VEHICLE_PARAMS_ON, objective);
			}
			return 1;
		}
	}
	return 1;
}

DialogResponse:VehicleReturnMenu(playerid, response, listitem, inputtext[])
{
	if (response) {
		Dialog_Show(playerid, Dialog:VehicleMenu);
	}
	return 1;
}
