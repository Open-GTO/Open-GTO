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

	static
		temp[MAX_LANG_VALUE_STRING],
		string[MAX_LANG_VALUE_STRING * 8];

	string[0] = '\0';

	Lang_GetPlayerText(playerid, "VEHICLE_MENU_LIST_FLIP", temp);
	strcat(string, temp);

	Lang_GetPlayerText(playerid, "VEHICLE_MENU_LIST_RADIO", temp);
	strcat(string, temp);

	// двери
	Lang_GetPlayerText(playerid, "VEHICLE_MENU_LIST_DOORS", temp);
	format(string, sizeof(string), temp, string, ret_GetVehicleDoorsAccessName(Lang_GetPlayerLang(playerid), vehicleid));

	// номер
	Lang_GetPlayerText(playerid, "VEHICLE_MENU_LIST_NUMBER", temp);
	strcat(string, temp);

	// фары
	if (lights == VEHICLE_PARAMS_ON) {
		Lang_GetPlayerText(playerid, "VEHICLE_MENU_LIST_DISABLE_LIGHTS", temp);
	} else {
		Lang_GetPlayerText(playerid, "VEHICLE_MENU_LIST_ENABLE_LIGHTS", temp);
	}
	strcat(string, temp);

	// двигатель
	if (engine == VEHICLE_PARAMS_ON) {
		Lang_GetPlayerText(playerid, "VEHICLE_MENU_LIST_DISABLE_ENGINE", temp);
	} else {
		Lang_GetPlayerText(playerid, "VEHICLE_MENU_LIST_ENABLE_ENGINE", temp);
	}
	strcat(string, temp);

	new vehicle_type = GetVehicleModelType(GetVehicleModel(vehicleid));

	if (vehicle_type == VEHICLE_TYPE_CAR) {
		// капот
		if (bonnet == VEHICLE_PARAMS_ON) {
			Lang_GetPlayerText(playerid, "VEHICLE_MENU_LIST_CLOSE_BONNET", temp);
		} else {
			Lang_GetPlayerText(playerid, "VEHICLE_MENU_LIST_OPEN_BONNET", temp);
		}
		strcat(string, temp);

		// багажник
		if (boot == VEHICLE_PARAMS_ON) {
			Lang_GetPlayerText(playerid, "VEHICLE_MENU_LIST_CLOSE_BOOT", temp);
		} else {
			Lang_GetPlayerText(playerid, "VEHICLE_MENU_LIST_OPEN_BOOT", temp);
		}
		strcat(string, temp);
	}

	if (   vehicle_type == VEHICLE_TYPE_CAR
	    || vehicle_type == VEHICLE_TYPE_BOAT
	    || vehicle_type == VEHICLE_TYPE_TRAIN
	    || vehicle_type == VEHICLE_TYPE_HELICOPTER
	    || vehicle_type == VEHICLE_TYPE_PLANE) {
		// окна
		new window_state;
		GetVehicleParamsCarWindows(vehicleid, window_state, window_state, window_state, window_state);

		if (window_state == VEHICLE_WINDOW_OPENED) {
			Lang_GetPlayerText(playerid, "VEHICLE_MENU_LIST_CLOSE_WINDOWS", temp);
		} else {
			Lang_GetPlayerText(playerid, "VEHICLE_MENU_LIST_OPEN_WINDOWS", temp);
		}
		strcat(string, temp);
	}

	Dialog_Open(playerid, Dialog:VehicleMenu, DIALOG_STYLE_LIST, "VEHICLE_MENU_HEADER", string, "BUTTON_OK", "BUTTON_CANCEL", MDIALOG_NOTVAR_INFO);
}

DialogResponse:VehicleMenu(playerid, response, listitem, inputtext[])
{
	if (!response) {
		return 1;
	}

	new engine, lights, alarm, doors, bonnet, boot, objective;
	new vehicleid = GetPlayerVehicleID(playerid);
	GetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, boot, objective);

	switch (listitem) {
		// перевернуть
		case 0: {
			new Float:float_tmp;
			GetVehicleZAngle(vehicleid, float_tmp);
			SetVehicleZAngle(vehicleid, float_tmp);
			return 1;
		}
		// радио
		case 1: {
			Dialog_Show(playerid, Dialog:VehicleRadio);
			return 1;
		}
		// двери
		case 2: {
			new slot = GetPlayerVehicleSlotByID(playerid, vehicleid);
			if (slot == -1) {
				Dialog_Message(playerid, "VEHICLE_MENU_HEADER", "VEHICLE_MENU_DOORS_NOTOWNER", "BUTTON_OK");
			} else {
				ChangePlayerVehicleDoorsAccess(playerid, slot);
				Dialog_Show(playerid, Dialog:VehicleMenu);
			}
			return 1;
		}
		// номер
		case 3: {
			Dialog_Show(playerid, Dialog:VehicleNumber);
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
	}

	new vehicle_type = GetVehicleModelType(GetVehicleModel(vehicleid));

	if (vehicle_type == VEHICLE_TYPE_CAR) {
		// капот
		if (listitem == 6) {
			if (bonnet == VEHICLE_PARAMS_ON) {
				SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, VEHICLE_PARAMS_OFF, boot, objective);
			} else {
				SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, VEHICLE_PARAMS_ON, boot, objective);
			}
			return 1;
		}

		// багажник
		if (listitem == 7) {
			if (boot == VEHICLE_PARAMS_ON) {
				SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, VEHICLE_PARAMS_OFF, objective);
			} else {
				SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, VEHICLE_PARAMS_ON, objective);
			}
			return 1;
		}
	}

	if (   vehicle_type == VEHICLE_TYPE_CAR
	    || vehicle_type == VEHICLE_TYPE_BOAT
	    || vehicle_type == VEHICLE_TYPE_TRAIN
	    || vehicle_type == VEHICLE_TYPE_HELICOPTER
	    || vehicle_type == VEHICLE_TYPE_PLANE) {
		// окна
		if (   (listitem == 8 && vehicle_type == VEHICLE_TYPE_CAR)
		    || (listitem == 6 && vehicle_type != VEHICLE_TYPE_CAR)) {
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

DialogCreate:VehicleNumber(playerid)
{
	new
		string[MAX_LANG_VALUE_STRING * 2],
		number[VEHICLE_NUMBER_SIZE + 1],
		vehicleid,
		vehicle_type,
		slot;

	// vehicle
	vehicleid = GetPlayerVehicleID(playerid);
	if (vehicleid == 0) {
		Dialog_MessageEx(playerid, Dialog:VehicleReturnMenu,
		                 "VEHICLE_MENU_HEADER",
		                 "VEHICLE_MENU_NUMBER_INVALID",
		                 "BUTTON_BACK", "BUTTON_CANCEL");
		return 0;
	}

	// vehicle type
	vehicle_type = GetVehicleModelType(GetVehicleModel(vehicleid));
	if (vehicle_type != VEHICLE_TYPE_CAR) {
		Dialog_MessageEx(playerid, Dialog:VehicleReturnMenu,
		                 "VEHICLE_MENU_HEADER",
		                 "VEHICLE_MENU_NUMBER_INVALID",
		                 "BUTTON_BACK", "BUTTON_CANCEL");
		return 0;
	}

	// slot
	slot = GetPlayerVehicleSlotByID(playerid, vehicleid);
	if (slot == -1) {
		Dialog_MessageEx(playerid, Dialog:VehicleReturnMenu,
		                 "VEHICLE_MENU_HEADER",
		                 "VEHICLE_MENU_NUMBER_NOTOWNER",
		                 "BUTTON_BACK", "BUTTON_CANCEL");
		return 0;
	}

	SetPVarInt(playerid, "vmenu_number_slot", slot);

	// make dialog info string
	Lang_GetPlayerText(playerid, "VEHICLE_MENU_NUMBER_INFO", string);

	GetPlayerVehicleNumber(playerid, slot, number);
	if (strlen(number) != 0) {
		Lang_GetPlayerText(playerid, "VEHICLE_MENU_NUMBER_INFO_CURRENT", string, _, number, string);
	}

	Dialog_Open(playerid, Dialog:VehicleNumber, DIALOG_STYLE_INPUT,
	            "VEHICLE_MENU_NUMBER_HEADER",
	            string,
	            "BUTTON_OK", "BUTTON_BACK",
	            MDIALOG_NOTVAR_INFO);
	return 1;
}

DialogResponse:VehicleNumber(playerid, response, listitem, inputtext[])
{
	if (!response) {
		Dialog_Show(playerid, Dialog:VehicleMenu);
		return 1;
	}

	new slot = GetPVarInt(playerid, "vmenu_number_slot");
	DeletePVar(playerid, "vmenu_number_slot");

	SetPlayerVehicleNumber(playerid, slot, inputtext);

	Dialog_MessageEx(playerid, Dialog:VehicleReturnMenu,
	                 "VEHICLE_MENU_HEADER",
	                 "VEHICLE_MENU_NUMBER_CHANGED",
	                 "BUTTON_BACK", "BUTTON_CANCEL");
	return 1;
}
