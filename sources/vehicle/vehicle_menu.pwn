/*

	About: vehicle user menu
	Author: ziggi

*/

#if defined _vehicle_menu_included
  #endinput
#endif

#define _vehicle_menu_included


VMenu_OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if (!PRESSED(KEY_NO)) {
		return 0;
	}

	if (GetPlayerState(playerid) == PLAYER_STATE_DRIVER) {
		Dialog_Show(playerid, Dialog:VehicleMenu);
		return 1;
	}

	return 0;
}

COMMAND:vmenu(playerid, params[])
{
	if (!IsPlayerInAnyVehicle(playerid)) {
		Lang_SendText(playerid, "VEHICLE_NOT_IN_CAR");
		return 0;
	}

	Dialog_Show(playerid, Dialog:VehicleMenu);
	return 1;
}

DialogCreate:VehicleMenu(playerid)
{
	new vehicleid = GetPlayerVehicleID(playerid);
	new engine, lights, alarm, doors, bonnet, boot, objective;
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);

	new
		temp[MAX_LANG_VALUE_STRING],
		string[MAX_LANG_VALUE_STRING * 8];

	Lang_GetPlayerText(playerid, "VEHICLE_MENU_LIST_FLIP", temp);
	strcat(string, temp, sizeof(string));

	Lang_GetPlayerText(playerid, "VEHICLE_MENU_LIST_FILL", temp);
	strcat(string, _temp, sizeof(string));

	Lang_GetPlayerText(playerid, "VEHICLE_MENU_LIST_RADIO", temp);
	strcat(string, temp, sizeof(string));

	// двери
	Lang_GetPlayerText(playerid, "VEHICLE_MENU_LIST_DOORS", temp);
	format(string, sizeof(string), temp, string, ReturnVehicleDoorsAccessName(Lang_GetPlayerLanguage(playerid), vehicleid));

	// фары
	if (lights == VEHICLE_PARAMS_ON) {
		Lang_GetPlayerText(playerid, "VEHICLE_MENU_LIST_DISABLE_LIGHTS", temp);
	} else {
		Lang_GetPlayerText(playerid, "VEHICLE_MENU_LIST_ENABLE_LIGHTS", temp);
	}
	strcat(string, temp, sizeof(string));

	// двигатель
	if (engine == VEHICLE_PARAMS_ON) {
		Lang_GetPlayerText(playerid, "VEHICLE_MENU_LIST_DISABLE_ENGINE", temp);
	} else {
		Lang_GetPlayerText(playerid, "VEHICLE_MENU_LIST_ENABLE_ENGINE", temp);
	}
	strcat(string, temp, sizeof(string));

	// капот
	if (bonnet == VEHICLE_PARAMS_ON) {
		Lang_GetPlayerText(playerid, "VEHICLE_MENU_LIST_CLOSE_BONNET", temp);
	} else {
		Lang_GetPlayerText(playerid, "VEHICLE_MENU_LIST_OPEN_BONNET", temp);
	}
	strcat(string, temp, sizeof(string));

	// багажник
	if (boot == VEHICLE_PARAMS_ON) {
		Lang_GetPlayerText(playerid, "VEHICLE_MENU_LIST_CLOSE_BOOT", temp);
	} else {
		Lang_GetPlayerText(playerid, "VEHICLE_MENU_LIST_OPEN_BOOT", temp);
	}
	strcat(string, temp, sizeof(string));

	// окна
	new window_state;
	GetVehicleParamsCarWindows(vehicleid, window_state, window_state, window_state, window_state);

	if (window_state == VEHICLE_WINDOW_OPENED) {
		Lang_GetPlayerText(playerid, "VEHICLE_MENU_LIST_CLOSE_WINDOWS", temp);
	} else {
		Lang_GetPlayerText(playerid, "VEHICLE_MENU_LIST_OPEN_WINDOWS", temp);
	}
	strcat(string, temp, sizeof(string));

	Dialog_Open(playerid, Dialog:VehicleMenu, DIALOG_STYLE_LIST, "VEHICLE_MENU_HEADER", string, "VEHICLE_MENU_BUTTON_OK", "VEHICLE_MENU_BUTTON_CANCEL", MDIALOG_NOTVAR_INFO);
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
				Dialog_MessageEx(playerid, Dialog:VehicleReturnMenu,
				                 "VEHICLE_FUEL_DIALOG_HEADER",
				                 "VEHICLE_FUEL_NOT_IN_VEHICLE",
				                 "VEHICLE_FUEL_DIALOG_BUTTON_BACK", "VEHICLE_FUEL_DIALOG_BUTTON_CANCEL");
				return 1;
			}

			if (!IsPlayerAtFuelStation(playerid)) {
				Dialog_MessageEx(playerid, Dialog:VehicleReturnMenu,
				                 "VEHICLE_FUEL_DIALOG_HEADER",
				                 "VEHICLE_FUEL_NOT_ON_FUEL_ST",
				                 "VEHICLE_FUEL_DIALOG_BUTTON_BACK", "VEHICLE_FUEL_DIALOG_BUTTON_CANCEL");
				return 1;
			}

			if (IsVehicleRefilling(vehicleid)) {
				Dialog_MessageEx(playerid, Dialog:VehicleReturnMenu,
				                 "VEHICLE_FUEL_DIALOG_HEADER",
				                 "VEHICLE_FUEL_IS_FUELING_ERROR",
				                 "VEHICLE_FUEL_DIALOG_BUTTON_BACK", "VEHICLE_FUEL_DIALOG_BUTTON_CANCEL");
				return 1;
			}

			new vehiclemodel = GetVehicleModel(vehicleid);
			switch (vehiclemodel) {
				case 481, 509, 510: {
					Dialog_MessageEx(playerid, Dialog:VehicleReturnMenu,
					                 "VEHICLE_FUEL_DIALOG_HEADER",
					                 "VEHICLE_FUEL_WITHOUT_FUEL_ENGINE",
					                 "VEHICLE_FUEL_DIALOG_BUTTON_BACK", "VEHICLE_FUEL_DIALOG_BUTTON_CANCEL");
					return 1;
				}
			}

			if (GetVehicleFuel(vehicleid) >= GetVehicleModelMaxFuel(vehiclemodel)) {
				Dialog_MessageEx(playerid, Dialog:VehicleReturnMenu,
				                 "VEHICLE_FUEL_DIALOG_HEADER",
				                 "VEHICLE_FUEL_FUEL_IS_FULL",
				                 "VEHICLE_FUEL_DIALOG_BUTTON_BACK", "VEHICLE_FUEL_DIALOG_BUTTON_CANCEL");
				return 1;
			}

			Lang_SendText(playerid, "VEHICLE_FUEL_IS_FUELING");
			FillVehicle(vehicleid, playerid);
			return 1;
		}
		// радио
		case 2: {
			Dialog_Show(playerid, Dialog:VehicleRadio);
			return 1;
		}
		// двери
		case 3: {
			new slot = GetPlayerVehicleSlotByID(playerid, vehicleid);
			if (slot == -1) {
				Dialog_Message(playerid, "VEHICLE_MENU_HEADER", "VEHICLE_MENU_DOORS_NOTOWNER", "VEHICLE_MENU_BUTTON_OK");
			} else {
				ChangePlayerVehicleDoorsAccess(vehicleid, playerid, slot);
				Dialog_Show(playerid, Dialog:VehicleMenu);
			}
			return 1;
		}
		// фары
		case 4: {
			if (lights == VEHICLE_PARAMS_ON) {
				SetVehicleParamsEx(vehicleid, engine, VEHICLE_PARAMS_OFF, alarm, doors, bonnet, boot, objective);
			} else {
				SetVehicleParamsEx(vehicleid, engine, VEHICLE_PARAMS_ON, alarm, doors, bonnet, boot, objective);
			}
			return 1;
		}
		// двигатель
		case 5: {
			if (engine == VEHICLE_PARAMS_ON) {
				SetVehicleParamsEx(vehicleid, VEHICLE_PARAMS_OFF, lights, alarm, doors, bonnet, boot, objective);
			} else {
				SetVehicleParamsEx(vehicleid, VEHICLE_PARAMS_ON, lights, alarm, doors, bonnet, boot, objective);
			}
			return 1;
		}
		// капот
		case 6: {
			if (bonnet == VEHICLE_PARAMS_ON) {
				SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, VEHICLE_PARAMS_OFF, boot, objective);
			} else {
				SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, VEHICLE_PARAMS_ON, boot, objective);
			}
			return 1;
		}
		// багажник
		case 7: {
			if (boot == VEHICLE_PARAMS_ON) {
				SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, VEHICLE_PARAMS_OFF, objective);
			} else {
				SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, VEHICLE_PARAMS_ON, objective);
			}
			return 1;
		}
		// окна
		case 8: {
			new window_state;
			GetVehicleParamsCarWindows(vehicleid, window_state, window_state, window_state, window_state);

			if (window_state != VEHICLE_WINDOW_OPENED) {
				SetVehicleParamsCarWindows(vehicleid, VEHICLE_WINDOW_OPENED, VEHICLE_WINDOW_OPENED, VEHICLE_WINDOW_OPENED, VEHICLE_WINDOW_OPENED);
			} else {
				SetVehicleParamsCarWindows(vehicleid, VEHICLE_WINDOW_CLOSED, VEHICLE_WINDOW_CLOSED, VEHICLE_WINDOW_CLOSED, VEHICLE_WINDOW_CLOSED);
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
