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
		SendClientMessage(playerid, COLOR_RED, _(playerid, VEHICLE_NOT_IN_CAR));
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

	new string[MAX_LANG_VALUE_STRING * 8];
	strcat(string, _(playerid, VEHICLE_MENU_LIST_FLIP), sizeof(string));
	strcat(string, _(playerid, VEHICLE_MENU_LIST_FILL), sizeof(string));
	strcat(string, _(playerid, VEHICLE_MENU_LIST_RADIO), sizeof(string));

	// �����
	format(string, sizeof(string), _(playerid, VEHICLE_MENU_LIST_DOORS), string, ReturnVehicleDoorsAccessName(Lang_GetPlayerLanguage(playerid), vehicleid));

	// ����
	if (lights == VEHICLE_PARAMS_ON) {
		strcat(string, _(playerid, VEHICLE_MENU_LIST_DISABLE_LIGHTS), sizeof(string));
	} else {
		strcat(string, _(playerid, VEHICLE_MENU_LIST_ENABLE_LIGHTS), sizeof(string));
	}

	// ���������
	if (engine == VEHICLE_PARAMS_ON) {
		strcat(string, _(playerid, VEHICLE_MENU_LIST_DISABLE_ENGINE), sizeof(string));
	} else {
		strcat(string, _(playerid, VEHICLE_MENU_LIST_ENABLE_ENGINE), sizeof(string));
	}

	// �����
	if (bonnet == VEHICLE_PARAMS_ON) {
		strcat(string, _(playerid, VEHICLE_MENU_LIST_CLOSE_BONNET), sizeof(string));
	} else {
		strcat(string, _(playerid, VEHICLE_MENU_LIST_OPEN_BONNET), sizeof(string));
	}

	// ��������
	if (boot == VEHICLE_PARAMS_ON) {
		strcat(string, _(playerid, VEHICLE_MENU_LIST_CLOSE_BOOT), sizeof(string));
	} else {
		strcat(string, _(playerid, VEHICLE_MENU_LIST_OPEN_BOOT), sizeof(string));
	}

	// ����
	new window_state;
	GetVehicleParamsCarWindows(vehicleid, window_state, window_state, window_state, window_state);

	if (window_state == VEHICLE_WINDOW_OPENED) {
		strcat(string, _(playerid, VEHICLE_MENU_LIST_CLOSE_WINDOWS), sizeof(string));
	} else {
		strcat(string, _(playerid, VEHICLE_MENU_LIST_OPEN_WINDOWS), sizeof(string));
	}

	Dialog_Open(playerid, Dialog:VehicleMenu, DIALOG_STYLE_LIST, _(playerid, VEHICLE_MENU_HEADER), string, _(playerid, VEHICLE_MENU_BUTTON_OK), _(playerid, VEHICLE_MENU_BUTTON_CANCEL));
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
		// �����������
		case 0: {
			new Float:float_tmp;
			GetVehicleZAngle(vehicleid, float_tmp);
			SetVehicleZAngle(vehicleid, float_tmp);
			return 1;
		}
		// ���������
		case 1: {
			if (vehicleid == 0) {
				Dialog_MessageEx(playerid, Dialog:VehicleReturnMenu, _(playerid, VEHICLE_FUEL_DIALOG_HEADER), _(playerid, VEHICLE_FUEL_NOT_IN_VEHICLE), "�����", "������");
				return 1;
			}

			if (!IsPlayerAtFuelStation(playerid)) {
				Dialog_MessageEx(playerid, Dialog:VehicleReturnMenu, _(playerid, VEHICLE_FUEL_DIALOG_HEADER), _(playerid, VEHICLE_FUEL_NOT_ON_FUEL_ST), "�����", "������");
				return 1;
			}

			if (IsVehicleRefilling(vehicleid)) {
				Dialog_MessageEx(playerid, Dialog:VehicleReturnMenu, _(playerid, VEHICLE_FUEL_DIALOG_HEADER), _(playerid, VEHICLE_FUEL_IS_FUELING_ERROR), "�����", "������");
				return 1;
			}

			new vehiclemodel = GetVehicleModel(vehicleid);
			switch (vehiclemodel) {
				case 481, 509, 510: {
					Dialog_MessageEx(playerid, Dialog:VehicleReturnMenu, _(playerid, VEHICLE_FUEL_DIALOG_HEADER), _(playerid, VEHICLE_FUEL_WITHOUT_FUEL_ENGINE), "�����", "������");
					return 1;
				}
			}

			if (GetVehicleFuel(vehicleid) >= GetVehicleModelMaxFuel(vehiclemodel)) {
				Dialog_MessageEx(playerid, Dialog:VehicleReturnMenu, _(playerid, VEHICLE_FUEL_DIALOG_HEADER), _(playerid, VEHICLE_FUEL_FUEL_IS_FULL), "�����", "������");
				return 1;
			}

			SendClientMessage(playerid, COLOR_YELLOW, _(playerid, VEHICLE_FUEL_IS_FUELING));
			FillVehicle(vehicleid, playerid);
			return 1;
		}
		// �����
		case 2: {
			Dialog_Show(playerid, Dialog:VehicleRadio);
			return 1;
		}
		// �����
		case 3: {
			new slot = GetPlayerVehicleSlotByID(playerid, vehicleid);
			if (slot == -1) {
				Dialog_Message(playerid, _(playerid, VEHICLE_MENU_HEADER), _(playerid, VEHICLE_MENU_DOORS_NOTOWNER), _(playerid, VEHICLE_MENU_BUTTON_OK));
			} else {
				ChangePlayerVehicleDoorsAccess(vehicleid, playerid, slot);
				Dialog_Show(playerid, Dialog:VehicleMenu);
			}
			return 1;
		}
		// ����
		case 4: {
			if (lights == VEHICLE_PARAMS_ON) {
				SetVehicleParamsEx(vehicleid, engine, VEHICLE_PARAMS_OFF, alarm, doors, bonnet, boot, objective);
			} else {
				SetVehicleParamsEx(vehicleid, engine, VEHICLE_PARAMS_ON, alarm, doors, bonnet, boot, objective);
			}
			return 1;
		}
		// ���������
		case 5: {
			if (engine == VEHICLE_PARAMS_ON) {
				SetVehicleParamsEx(vehicleid, VEHICLE_PARAMS_OFF, lights, alarm, doors, bonnet, boot, objective);
			} else {
				SetVehicleParamsEx(vehicleid, VEHICLE_PARAMS_ON, lights, alarm, doors, bonnet, boot, objective);
			}
			return 1;
		}
		// �����
		case 6: {
			if (bonnet == VEHICLE_PARAMS_ON) {
				SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, VEHICLE_PARAMS_OFF, boot, objective);
			} else {
				SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, VEHICLE_PARAMS_ON, boot, objective);
			}
			return 1;
		}
		// ��������
		case 7: {
			if (boot == VEHICLE_PARAMS_ON) {
				SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, VEHICLE_PARAMS_OFF, objective);
			} else {
				SetVehicleParamsEx(vehicleid, engine, lights, alarm, doors, bonnet, VEHICLE_PARAMS_ON, objective);
			}
			return 1;
		}
		// ����
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
